import shutil
import os
import serial
import serial.tools.list_ports
import FieldUpdater as FU 
import CardDB as CDB
import time
from selenium import webdriver

# --------------------------------------------------------------------------------------------------------------------------------
# Function Declarations
# --------------------------------------------------------------------------------------------------------------------------------

def Clean_Field():
	cwd = os.getcwd() 
	src = cwd 
	current_field = cwd + '/Field_MR5.html'
	clean_field = cwd + '/Field_MR5.html'
	shutil.move(os.path.join(src, clean_field), os.path.join(src, current_field))

def Connect_DD():
	print("Connecting to Duel Disk System")

	Ser = serial.Serial(
		port='COM12',\
		baudrate=9600,\
		parity=serial.PARITY_NONE,\
		stopbits=serial.STOPBITS_ONE,\
		bytesize=serial.EIGHTBITS,\
		timeout=1)
	return Ser

def Verify_Serial(ser):
	connected = Check_Connection(ser)
	if connected:
		pass	
	else:
		while not connected:
			try:
				ser.close()
				ser = Connect_DD()

			except: 
				pass	
			connected = Check_Connection(ser)
			time.sleep(1)
	return ser

def Check_Connection(ser):
	DD_Port = ser.portstr
	myports = [tuple(p) for p in list(serial.tools.list_ports.comports())]
	# print(myports)
	# print(DD_Port)
	connected = False
	for port in myports:
		if DD_Port not in port[0]:
			print("Duel Disk Disconnected")
			connected =  connected or False
		else:
			connected = connected or True
	return connected

def Process_Data(data): # Processses serial data according to action

	action_str = ''
	action_str+=chr(data[0])
	action_str+=chr(data[1])
	action_str+=chr(data[2])

	if action_str == "MON":
		# Normal Summon a monster or Spell/Trap
		# MON 12345678 Z2 ATK
		cardid_str = ''
		cardid_str+=chr(data[3])
		cardid_str+=chr(data[4])
		cardid_str+=chr(data[5])
		cardid_str+=chr(data[6])
		cardid_str+=chr(data[7])
		cardid_str+=chr(data[8])
		cardid_str+=chr(data[9])
		cardid_str+=chr(data[10])

		zone_str = ''
		zone_str+=chr(data[11])
		zone_str+=chr(data[12])

		pos_str = ''
		pos_str+=chr(data[13])
		pos_str+=chr(data[14])
		pos_str+=chr(data[15])

		FU.Add_Monster_Spell_Trap(Deck,cardid_str,zone_str,pos_str)
		driver.get("file:///C:/Users/Augusto/Documents/Proyectos/Duel Disk/Bleeding Edge Duel Disk/Virtual Field/Field_MR5.html")

	if action_str == "SET":
		# Set Card
		# SET 12345678 Z1
		
		cardid_str = ''
		cardid_str+=chr(data[3])
		cardid_str+=chr(data[4])
		cardid_str+=chr(data[5])
		cardid_str+=chr(data[6])
		cardid_str+=chr(data[7])
		cardid_str+=chr(data[8])
		cardid_str+=chr(data[9])
		cardid_str+=chr(data[10])

		zone_str = ''
		zone_str+=chr(data[11])
		zone_str+=chr(data[12])

		FU.Add_Set_Card(Deck,cardid_str,zone_str)

		driver.get("file:///C:/Users/Augusto/Documents/Proyectos/Duel Disk/Bleeding Edge Duel Disk/Virtual Field/Field_MR5.html")

	if action_str == "PEN":
		# Pendulum Place
		# PEN 12345678 Z1
		
		cardid_str = ''
		cardid_str+=chr(data[3])
		cardid_str+=chr(data[4])
		cardid_str+=chr(data[5])
		cardid_str+=chr(data[6])
		cardid_str+=chr(data[7])
		cardid_str+=chr(data[8])
		cardid_str+=chr(data[9])
		cardid_str+=chr(data[10])

		zone_str = ''
		zone_str+=chr(data[11])
		zone_str+=chr(data[12])

		FU.Add_Pendulum(Deck,cardid_str,zone_str)

		driver.get("file:///C:/Users/Augusto/Documents/Proyectos/Duel Disk/Bleeding Edge Duel Disk/Virtual Field/Field_MR5.html")

	if action_str == "GRA":
		# Send Monster to Graveyard
		# GRA Z1

		cardid_str = ''
		cardid_str+=chr(data[3])
		cardid_str+=chr(data[4])
		cardid_str+=chr(data[5])
		cardid_str+=chr(data[6])
		cardid_str+=chr(data[7])
		cardid_str+=chr(data[8])
		cardid_str+=chr(data[9])
		cardid_str+=chr(data[10])

		zone_str = ''
		zone_str+=chr(data[11])
		zone_str+=chr(data[12])

		FU.Remove_Card(Deck,cardid_str,zone_str)

		driver.get("file:///C:/Users/Augusto/Documents/Proyectos/Duel Disk/Bleeding Edge Duel Disk/Virtual Field/Field_MR5.html")

	if action_str == "SPS":
		# Special Summon a Monster
		# SPS 12345678 Z1 Z2 87654321 12345678
		length = len(data)
		ss_monster = ''
		ss_monster+=chr(data[3])
		ss_monster+=chr(data[4])
		ss_monster+=chr(data[5])
		ss_monster+=chr(data[6])
		ss_monster+=chr(data[7])
		ss_monster+=chr(data[8])
		ss_monster+=chr(data[9])
		ss_monster+=chr(data[10])

		for material_zone in material_zones:
			FU.Remove_Monster(material_zone)

		zone_str = ''
		zone_str+=chr(length-1)
		zone_str+=chr(length)

		FU.Add_Monster(Deck,ss_monster,zone_str,"ATK")
		driver.get("file:///C:/Users/Augusto/Documents/Proyectos/Duel Disk/Bleeding Edge Duel Disk/Virtual Field/Field_MR5.html")

# --------------------------------------------------------------------------------------------------------------------------------
# Code Start
# --------------------------------------------------------------------------------------------------------------------------------

driver = webdriver.Chrome(r'C:\Users\Augusto\Documents\Unmovable\chromedriver.exe')
driver.fullscreen_window()
driver.get("file:///C:/Users/Augusto/Documents/Proyectos/Duel Disk/Bleeding Edge Duel Disk/Virtual Field/Field_MR5.html")

Clean_Field()

ser = Connect_DD()

print("Duel Disk on port: " + ser.portstr)

# Empty the Board
MonsterZones = [None,None,None,None,None,None,None]
SpellTrapZones = [None,None,None,None,None]
FieldZone = [None]
Graveyard = []
Vanished = []
Deck = []
ExtraDeck = [] 

# Load Decks to local machine before duel
print("Loading Deck profile")
Deck = CDB.Get_Deck_Data("MainDeck.csv")
Deck = [i for i in Deck if i is not None]
print("Loading Extra Deck")
ExtraDeck = CDB.Get_Deck_Data("ExtraDeck.csv")
ExtraDeck = [i for i in ExtraDeck if i is not None]

# Set life-points
print("Setting Life-points")
LP = 8000

print("Ready for Duel")

while True:
	ser = Verify_Serial(ser)
	data = ser.readline()
	if data :
		print(data)
		Process_Data(data)

ser.close()



