# Bleeding-Edge-Duel-Disk-System

Field and Disk for one player

### Prerequisites

Dependencies instalation

```
 pip install -r requirements.txt 
```

### Installing

Upload the code to the Arduino Uno and follow the connection diagram. The code can be found in Disk Code/BleedingEdgeDD/BleedingEdgeDD.ino

Modify line 25 of FieldCode/MainReader.py to fit the connected port of your Arduino
```
 port='COM12',
```

Add the cards used in your Main and Extra Deck into the CSV Files in the FieldCode Folder. The format should be as follows: Card Name, Card ID, Quantity

Example:
```
Blue Eyes White Dragon,	89631139,	3
```

For individual NFC Cards the Card ID has to be written as the first 8 characters in the Block 4 of the Sticker. A bulk uploading code can be achieved with the Arduino Code but is not yet implemented.

## Running
Connect the Duel Disk To the PC in the COM port modified in line 25. Run the script MainReader.py will trigger the Virtual field. The field is a MR5 Field, and the buttons of the Disk are arranged in a similar manner.
```
+---+---+---+----+---+
| - | 3 | - | 5  | - |
+---+---+---+----+---+
| 1 | 2 | 4 |  6 | 7 |
+---+---+---+----+---+

Button 1: Face-Down Mode
Button 2: Place Pendulum Scale Mode
Button 4: Activate Monster, Trap or Effect (Not implemented)
Button 6: Grave/Banish Mode
Button 7: Special Summon Mode
```
Summoning:
To summon a Monster scan the NFC card, then select the zone to be placed with buttons 1-7, then select battle position with buttons 3 and 5.
```
3: Attack
5: Defense
```

To summon a monster in Attack Position into Monster Zone 4: 
* Scan the Card
* Press Button 4 (Selects Monster Zone 4)
* Press Button 3 (Selects Attack)

Setting a Face-Down:
Long Press Button 1 this will trigger the Face Down mode. Scan the Card to be placed down, either Spell/Trap/Monster the program will determine if the card to be placed goes to a Monster or to a Spell and Trap Zone. And then select the position, note that for spell and trap cards only Zones 1,2,4,6,7 are available as in the field.

Face-Down Trap Example:
* Long Press Button 1 (Enter FAce-Down Mode)
* Scan the Card
* Press Button 4 (Select the middle Spell and Trap Card Zone)

Removing Cards From Field 
Example:
* Long Press Button 6 to enter Grave/Banish Mode
* Scan the Card to be removed
* Select the Zone of removal (in case two of the same cards are present)

Special Summon:
Example:
* Long Press Button 7 to enter Special Summon Mode
* Scan Card to Special Summon
* Scan Cards From Hand to use as Materials
* Select Monsters From Field to use as Materials
* Scan again the Card to Special Summon
* Select Monster Zone to be Placed with Buttons 1-7
* Select battle psition with Buttons 3 or 5


## Built With

* [YGO Pro Api](https://db.ygoprodeck.com/api-guide/) - Api used
* [Selenium](https://www.selenium.dev/) - Library used


## Reference

* [Hack a Day Project](https://hackaday.io/project/169942-duel-disk-system)- Project on Hack a Day

## Authors

* **Augusto Masetti** - *Initial work* - [AMasetti](https://github.com/AMasetti)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE) file for details
