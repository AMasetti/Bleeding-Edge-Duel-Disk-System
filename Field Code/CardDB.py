import json
import requests
import shutil
import csv

def Download_Img(Card_img_url,id_card): # Downloads the image of a Card ID using the API
	resp = requests.get(Card_img_url, stream=True)
	local_file = open('img/'+id_card+'.jpg', 'wb')
	resp.raw.decode_content = True
	shutil.copyfileobj(resp.raw, local_file)
	del resp

def Get_Card_Data(id_card): # Retrieves all information from every card in the game registered in the API, Modern Magic and awesome Work
	# Normalize IDs
	id_card = str(int(id_card))
	# Open Json DB
	with open('cardinfo.json', 'r') as json_file:
		data = json.load(json_file)
	# Search for Card
	for card in data:

		if card["id"] == id_card:
			
			Card = {} 
			Card["url"] = card["card_images"][0]["image_url"]
			Card["id"] = card["id"]
			Card["name"] = card["name"]
			Card["typ"] = card["type"]
			Card["des"] = card["desc"]
			Card["race"] = card["race"]

			if Card["typ"] != "Spell Card" and Card["typ"] != "Trap Card":
				Card["ATK"] = card["atk"]
				Card["DEF"] = card["def"]
				Card["lvl"] = card["level"]
				Card["atr"] = card["attribute"]
			
			# get image if not downloaded before
			try:
				f = open('img/'+id_card+".jpg")
				# print("Card Image exists on local machine")
				f.close()
			except IOError:

				print("Information on " +card["name"]+ " not found, downloading...")
				Download_Img(Card["url"],id_card)
				print("Done")

			return Card

def Get_Deck_Data(csvfile): # Retrieves all information from Deck File
	deck = []
	with open(csvfile, "r") as csv_file:
		csv_reader = csv.reader(csv_file, delimiter=',')
		# Skip First Line
		next(csv_reader)
		for line in csv_reader:
			deck.append(Get_Card_Data(line[1]))
		return deck

def Find_In_Deck(deck,id_card): # Returns information from card in the Deck
	for card in deck:
		if card['id'] == id_card:
			return card

# Card = Get_Card_Data(id_card = '59793705')
# print(Card)





