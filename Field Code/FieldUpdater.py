from bs4 import BeautifulSoup
import CardDB as CDB


def Remove_Card (Deck,cardid,zone): # Only 5 Zones implemented at the moment

	Card = CDB.Find_In_Deck(Deck,cardid)
	
	if Card["typ"] != "Spell Card" and Card["typ"] != "Trap Card":

		if zone == "Z1": place_zone="P1MZ1"
		if zone == "Z2": place_zone="P1MZ2"
		if zone == "Z3": place_zone="P1MZ3"
		if zone == "Z4": place_zone="P1MZ4"
		if zone == "Z5": place_zone="P1MZ5"
		if zone == "Z6": place_zone="P1MZ6"
		if zone == "Z7": place_zone="P1MZ7"
	else:
		# Spell or Trap Card
		if zone == "Z3" or zone == "Z5": 
			pass
		else:
			if zone == "Z1": place_zone="P1STZ1"
			if zone == "Z2": place_zone="P1STZ2"
			if zone == "Z4": place_zone="P1STZ3"
			if zone == "Z6": place_zone="P1STZ4"
			if zone == "Z7": place_zone="P1STZ5"
			
	Modify_Field_Value(None,place_zone,'img/None.png',"ATK",Show_Values=True)

def Add_Monster_Spell_Trap(Deck,cardid,zone,pos): # Only 5 Zones implemented at the moment
	
	Card = CDB.Find_In_Deck(Deck,cardid)

	if Card["typ"] != "Spell Card" and Card["typ"] != "Trap Card":
		if zone == "Z1": place_zone="P1MZ1"
		if zone == "Z2": place_zone="P1MZ2"
		if zone == "Z3": place_zone="P1MZ3"
		if zone == "Z4": place_zone="P1MZ4"
		if zone == "Z5": place_zone="P1MZ5"
		if zone == "Z6": place_zone="P1MZ6"
		if zone == "Z7": place_zone="P1MZ7"

		Modify_Field_Value(Card,place_zone,'img/'+Card["id"]+".jpg",pos,Show_Values=False)

	else:
		# Spell or Trap Card
		if zone == "Z3" or zone == "Z5": 
			pass
		else:
			if zone == "Z1": place_zone="P1STZ1"
			if zone == "Z2": place_zone="P1STZ2"
			if zone == "Z4": place_zone="P1STZ3"
			if zone == "Z6": place_zone="P1STZ4"
			if zone == "Z7": place_zone="P1STZ5"
		Modify_Field_Value_Spells(place_zone,'img/'+Card["id"]+".jpg")


def Add_Set_Card(Deck,cardid,zone): # Works for Monsters in DEF position and Spell/Trap Cards in Magic/Trap Zones

	Card = CDB.Find_In_Deck(Deck,cardid)

	if Card["typ"] != "Spell Card" and Card["typ"] != "Trap Card":
		# It's a monster place it in Monster Zone Face Down Defense Position
		if zone == "Z1": place_zone="P1MZ1"
		if zone == "Z2": place_zone="P1MZ2"
		if zone == "Z3": place_zone="P1MZ3"
		if zone == "Z4": place_zone="P1MZ4"
		if zone == "Z5": place_zone="P1MZ5"
		if zone == "Z6": place_zone="P1MZ6"
		if zone == "Z7": place_zone="P1MZ7"

		Modify_Field_Value(cardid,place_zone,'img/Facedown.png',"DEF",Show_Values=True)

	else:
		# Spell or Trap Card
		if zone == "Z3" or zone == "Z5": 
			pass
		else:
			if zone == "Z1": place_zone="P1STZ1"
			if zone == "Z2": place_zone="P1STZ2"
			if zone == "Z4": place_zone="P1STZ3"
			if zone == "Z6": place_zone="P1STZ4"
			if zone == "Z7": place_zone="P1STZ5"
		Modify_Field_Value_Spells(place_zone,'img/Facedown.png')

def Add_Pendulum(Deck,cardid_str,zone_str): # Adds a Pendulum Monster as a Pendulum
	Card = CDB.Find_In_Deck(Deck,cardid_str)
	if zone_str == "Z1": 
		place_zone="P1STZ1"
		Modify_Field_Value_Spells(place_zone,"img/"+Card["id"]+".jpg")
	if zone_str == "Z2": 
		place_zone="P1STZ7"
		Modify_Field_Value_Spells(place_zone,"img/"+Card["id"]+".jpg")




def Modify_Field_Value(Card,zone,card_img,position,Show_Values): # Places an image of a Card or None in a certain orientation and with some information if needed
	Dict_correspondence = { 'P1MZ1': 'P1info1', 
							'P1MZ2': 'P1info2',  
							'P1MZ3': 'P1info3',  
							'P1MZ4': 'P1info4',  
							'P1MZ5': 'P1info5',  
							'P1MZ6': 'P1info6',  
							'P1MZ7': 'P1info7',  
							'P2MZ1': 'P2info1', 
							'P2MZ2': 'P2info2',  
							'P2MZ3': 'P2info3',  
							'P2MZ4': 'P2info4',  
							'P2MZ5': 'P2info5',  
							'P2MZ6': 'P2info6',  
							'P2MZ7': 'P2info7',  
	}
	P2infos = ['P2info1','P2info2','P2info3','P2info4','P2info5','P2info6','P2info7']

	with open('Field_MR5.html') as html_file:
		soup = BeautifulSoup(html_file.read(), features='html.parser')

		new_tag = soup.new_tag("img")
		new_tag['src'] = card_img
		new_tag['class'] = position
		new_tag['id'] = zone

		for tag in soup.find_all(id=zone):
			tag.replace_with(new_tag)

		new_tag = soup.new_tag("p")
		new_tag['class'] = "futurepanel__title"
		new_tag['id'] = Dict_correspondence[zone]
		if not Show_Values:
			new_tag.string = str(Card['atk'])+' / '+str(Card['def'])
		else:
			new_tag.string ='ATK/DEF'

		if Dict_correspondence[zone] in P2infos:
			new_tag['style'] = "transform: rotate(180deg);"

		for tag in soup.find_all(id=Dict_correspondence[zone]):
			tag.replace_with(new_tag)

		new_text = soup.prettify()

	with open('Field_MR5.html', mode='w') as new_html_file:
		new_html_file.write(new_text)

def Modify_Field_Value_Spells(zone,card_img): # Much simpler funtion to facilitate Spell/Trap casting

	with open('Field_MR5.html') as html_file:
		soup = BeautifulSoup(html_file.read(), features='html.parser')

		new_tag = soup.new_tag("img")
		new_tag['src'] = card_img
		new_tag['class'] = "ATK"
		new_tag['id'] = zone
		print(new_tag)

		for tag in soup.find_all(id=zone):
			tag.replace_with(new_tag)

		new_text = soup.prettify()

	with open('Field_MR5.html', mode='w') as new_html_file:
		new_html_file.write(new_text)