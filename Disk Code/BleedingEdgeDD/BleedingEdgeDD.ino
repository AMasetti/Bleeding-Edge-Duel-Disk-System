#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

#define PN532_IRQ   (3)
#define PN532_RESET (23)
#define Long_Press_Time 1000

// Hardware Config
const int  speaker = 11;   
const int  buttonPin_SetCard = 10; 
const int  buttonPin_PlacePendulum = 9; 
const int  buttonPin_ATK = 8; 
const int  buttonPin_ActivateMTE = 6; 
const int  buttonPin_DEF = 7; 
const int  buttonPin_GraveBanish = 5; 
const int  buttonPin_SpecialSummon = 4;   

// NFC Declarations
// Timeout has to be commented out in the Adafruit Library by hand
Adafruit_PN532 nfc(PN532_IRQ,PN532_RESET);
#if defined(ARDUINO_ARCH_SAMD)
   #define Serial SerialUSB
#endif

String Materials = "";

// Can be done better but works
int buttonState_SetCard = 0;    
int buttonState_PlacePendulum = 0; 
int buttonState_ATK = 0; 
int buttonState_ActivateMTE = 0; 
int buttonState_DEF = 0; 
int buttonState_GraveBanish = 0; 
int buttonState_SpecialSummon = 0;      

//  Conters for long presses
long time_counter_SetCard; 
long time_counter_PlacePendulum; 
long time_counter_ATK; 
long time_counter_ActivateMTE; 
long time_counter_DEF; 
long time_counter_GraveBanish; 
long time_counter_SpecialSummon; 

void beep_sound(){
  digitalWrite(11, HIGH);   
  delay(200);                       
  digitalWrite(11, LOW);
}

void Reset_Timers(){
  time_counter_SetCard = millis();
  time_counter_PlacePendulum= millis(); 
  time_counter_ATK= millis(); 
  time_counter_ActivateMTE= millis(); 
  time_counter_DEF= millis(); 
  time_counter_GraveBanish= millis(); 
  time_counter_SpecialSummon= millis(); 
}

String Select_Zone(){
  // It's blocking meaning it will only scan for changes on the buttons ignoring the possible scan of an NFC card
  String Zone = "";
  bool pressed = false;
  while(!pressed){
    buttonState_SetCard = digitalRead(buttonPin_SetCard);    
    buttonState_PlacePendulum = digitalRead(buttonPin_PlacePendulum);    
    buttonState_ATK = digitalRead(buttonPin_ATK);    
    buttonState_ActivateMTE = digitalRead(buttonPin_ActivateMTE);    
    buttonState_DEF = digitalRead(buttonPin_DEF);    
    buttonState_GraveBanish = digitalRead(buttonPin_GraveBanish);    
    buttonState_SpecialSummon = digitalRead(buttonPin_SpecialSummon);

    if(buttonState_SetCard == HIGH){
      pressed = true;
      Zone = "Z1";
      }
    if(buttonState_PlacePendulum == HIGH){
      pressed = true;
      Zone = "Z2";
      }
    if(buttonState_ATK == HIGH){
      pressed = true;
      Zone = "Z3";
      }
    if(buttonState_ActivateMTE == HIGH){
      pressed = true;
      Zone = "Z4";
      }
    if(buttonState_DEF == HIGH){
      pressed = true;
      Zone = "Z5";
      }
    if(buttonState_GraveBanish == HIGH){
      pressed = true;
      Zone = "Z6";
      }
    if(buttonState_SpecialSummon == HIGH){
      pressed = true;
      Zone = "Z7";
      }
    }
  beep_sound();
  return Zone;
}

String Select_Zone_Non_Blocking(){
  // Only used for Special Summoning, not yet implemented
  String Zone = "Z0";
  buttonState_SetCard = digitalRead(buttonPin_SetCard);    
  buttonState_PlacePendulum = digitalRead(buttonPin_PlacePendulum);    
  buttonState_ATK = digitalRead(buttonPin_ATK);    
  buttonState_ActivateMTE = digitalRead(buttonPin_ActivateMTE);    
  buttonState_DEF = digitalRead(buttonPin_DEF);    
  buttonState_GraveBanish = digitalRead(buttonPin_GraveBanish);    
  buttonState_SpecialSummon = digitalRead(buttonPin_SpecialSummon);

  if(buttonState_SetCard == HIGH){
    beep_sound();
    Zone = "Z1";
    }
  if(buttonState_PlacePendulum == HIGH){
    beep_sound();
    Zone = "Z2";
    }
  if(buttonState_ATK == HIGH){
    beep_sound();
    Zone = "Z3";
    }
  if(buttonState_ActivateMTE == HIGH){
    beep_sound();
    Zone = "Z4";
    }
  if(buttonState_DEF == HIGH){
    beep_sound();
    Zone = "Z5";
    }
  if(buttonState_GraveBanish == HIGH){
    beep_sound();
    Zone = "Z6";
    }
  if(buttonState_SpecialSummon == HIGH){
    beep_sound();
    Zone = "Z7";
    }
  return Zone;
}

String Select_Pos(){
  // Selection for Monster Attack or Defense postion
  String Pos = "none";
  bool pressed = false;
  while(!pressed){
    buttonState_ATK = digitalRead(buttonPin_ATK);        
    buttonState_DEF = digitalRead(buttonPin_DEF);    
    if(buttonState_ATK == HIGH){
      pressed = true;
      Pos = "ATK";
      }
    if(buttonState_DEF == HIGH){
      pressed = true;
      Pos = "DEF";
      }
    }
  beep_sound();
  return Pos;
}

String Select_Pendulum(){
  // Selection for Pendulum Monster Zone placement
  String Pos = "none";
  bool pressed = false;
  while(!pressed){
    buttonState_SetCard = digitalRead(buttonPin_SetCard);        
    buttonState_SpecialSummon = digitalRead(buttonPin_SpecialSummon);    

    if(buttonState_SetCard == HIGH){
      pressed = true;
      Pos = "Z1";
      }

    if(buttonState_SpecialSummon == HIGH){
      pressed = true;
      Pos = "Z2";
      }

    }
  return Pos;
}

void Summon_Monster_Spell_Trap_Directly(){
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength,10);
  if (success) {  
    if (uidLength == 4){
      uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);
      if (success){  
        uint8_t data[16];          
        success = nfc.mifareclassic_ReadDataBlock(4, data);
        if (success){
          String ReadBlock;
          String ReadCard = "11111111";
          for (char c : data) ReadBlock += c;
          ReadCard[0] = ReadBlock[0];
          ReadCard[1] = ReadBlock[1];
          ReadCard[2] = ReadBlock[2];
          ReadCard[3] = ReadBlock[3];
          ReadCard[4] = ReadBlock[4];
          ReadCard[5] = ReadBlock[5];
          ReadCard[6] = ReadBlock[6];
          ReadCard[7] = ReadBlock[7];
          beep_sound();
          String Zone,Pos;
          Zone = Select_Zone();
          Pos = Select_Pos();
          Serial.println("MON"+ReadCard+Zone+Pos);
          delay(100);
        }
      }
    }
  }
}

String Read_NFC_Content_Non_Blocking(){
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength,100);
  if (success) {  
    if (uidLength == 4){
      uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);
      if (success){  
        uint8_t data[16];          
        success = nfc.mifareclassic_ReadDataBlock(4, data);
        if (success){
          String ReadBlock;
          String ReadCard = "11111111";
          for (char c : data) ReadBlock += c;
          ReadCard[0] = ReadBlock[0];
          ReadCard[1] = ReadBlock[1];
          ReadCard[2] = ReadBlock[2];
          ReadCard[3] = ReadBlock[3];
          ReadCard[4] = ReadBlock[4];
          ReadCard[5] = ReadBlock[5];
          ReadCard[6] = ReadBlock[6];
          ReadCard[7] = ReadBlock[7];
          beep_sound();
          return ReadCard;
        }
      }
    }
  }
}

String Read_NFC_Content_Blocking(){
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  while(!success){
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  }
  if (success) {  
    if (uidLength == 4){
      uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);
      if (success){  
        uint8_t data[16];          
        success = nfc.mifareclassic_ReadDataBlock(4, data);
        if (success){
          String ReadBlock;
          String ReadCard = "11111111";
          for (char c : data) ReadBlock += c;
          ReadCard[0] = ReadBlock[0];
          ReadCard[1] = ReadBlock[1];
          ReadCard[2] = ReadBlock[2];
          ReadCard[3] = ReadBlock[3];
          ReadCard[4] = ReadBlock[4];
          ReadCard[5] = ReadBlock[5];
          ReadCard[6] = ReadBlock[6];
          ReadCard[7] = ReadBlock[7];
          beep_sound();
          return ReadCard;
        }
      }
    }
  }
}

void process_SetCard(){
  int buttonState = digitalRead(buttonPin_SetCard);    
  if (buttonState == LOW ) {
    time_counter_SetCard = millis();
  }
  if (buttonState == HIGH && millis()-time_counter_SetCard >= Long_Press_Time) {
    
    // Action for long press detected on certain button
    beep_sound();
    String ReadCard = "";
    ReadCard = Read_NFC_Content_Blocking();
    String Zone = "";
    Zone = Select_Zone();
    Serial.println("SET"+ReadCard+Zone);
    Reset_Timers();
    
  }
}

void process_PlacePendulum(){
  int buttonState = digitalRead(buttonPin_PlacePendulum);    
  if (buttonState == LOW ) {
    time_counter_PlacePendulum = millis();
  }
  if (buttonState == HIGH && millis()-time_counter_PlacePendulum >= Long_Press_Time) {
    
    // Action for long press detected on certain button
    beep_sound();
    String ReadCard = "";
    ReadCard = Read_NFC_Content_Blocking();
    String Zone = "";
    Zone = Select_Pendulum();
    Serial.println("PEN"+ReadCard+Zone);
    beep_sound();
    Reset_Timers();
    

  }
}

void process_ATK(){
  int buttonState = digitalRead(buttonPin_ATK);    
  if (buttonState == LOW ) {
    time_counter_ATK = millis();
  }
  if (buttonState == HIGH && millis()-time_counter_ATK >= Long_Press_Time) {
    
    // Action for long press detected on certain button
    beep_sound();
    time_counter_SetCard = millis();
    time_counter_PlacePendulum= millis(); 
    time_counter_ATK= millis(); 
    time_counter_ActivateMTE= millis(); 
    time_counter_DEF= millis(); 
    time_counter_GraveBanish= millis(); 
    time_counter_SpecialSummon= millis(); 
  }
}

void process_ActivateMTE(){
  int buttonState = digitalRead(buttonPin_ActivateMTE);    
  if (buttonState == LOW ) {
    time_counter_ActivateMTE = millis();
  }
  if (buttonState == HIGH && millis()-time_counter_ActivateMTE >= Long_Press_Time) {
   
    // Action for long press detected on certain button
    beep_sound();
    String ReadCard = "";
    String Zone_aux = "";
    String Zone = "";
    delay(500);
    //ReadCard = Read_NFC_Content_Blocking();
    Zone = Select_Zone();
    Serial.println("MTE"+Zone);
    Reset_Timers();
    
    
  }
}

void process_DEF(){
  int buttonState = digitalRead(buttonPin_DEF);
  if (buttonState == LOW ) {
    time_counter_DEF = millis();
  }
  if (buttonState == HIGH && millis()-time_counter_DEF >= Long_Press_Time) {
    
    // Action for long press detected on certain button
    beep_sound();
    Reset_Timers();
  }
}

void process_GraveBanish(){
  // Send a card from the field to the grave
  // TBD Pendulum
  // Idea: use atk and def buttons to select pendulum in spell/trap zones
  int buttonState = digitalRead(buttonPin_GraveBanish);    
  if (buttonState == LOW ) {
    time_counter_GraveBanish = millis();
  }
  if (buttonState == HIGH && millis()-time_counter_GraveBanish >= 1000) {
    // Action for long press detected on certain button
    beep_sound();
    delay(500); // Wait before pressing another button to prevent debounce
    String ReadCard = "";
    ReadCard = Read_NFC_Content_Blocking(); // Scan the card to be retired
    String Zone = "";
    Zone = Select_Zone(); // Select the zone of the card 
    Serial.println("GRA"+ReadCard+Zone);
    delay(500); // Wait before pressing another button to prevent debounce
    Reset_Timers();
  }
  
}

void process_SpecialSummon(){
  // TBD
  int buttonState = digitalRead(buttonPin_SpecialSummon);    

  if (buttonState == LOW ) {
    time_counter_SpecialSummon = millis();
  }
  if (buttonState == HIGH && millis()-time_counter_SpecialSummon >= Long_Press_Time) {
    
    // Action for long press detected on certain button
    beep_sound();
    
    // Select Result Monster
    String ReadCardSS = "";
    String Materials = "";
    ReadCardSS = Read_NFC_Content_Blocking();
    delay(500);
    // Select Material Monsters
    
    String ScanNFC = "00000000";
    
    while(ScanNFC!=ReadCardSS){
      ScanNFC = "00000000";
      uint8_t success;
      uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
      uint8_t uidLength;
      success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength,10);
      if (success) {  
        if (uidLength == 4){
          uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
          success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);
          if (success){  
            uint8_t data[16];          
            success = nfc.mifareclassic_ReadDataBlock(4, data);
            if (success){
              String ReadBlock;
              
              for (char c : data) ReadBlock += c;
              ScanNFC[0] = ReadBlock[0];
              ScanNFC[1] = ReadBlock[1];
              ScanNFC[2] = ReadBlock[2];
              ScanNFC[3] = ReadBlock[3];
              ScanNFC[4] = ReadBlock[4];
              ScanNFC[5] = ReadBlock[5];
              ScanNFC[6] = ReadBlock[6];
              ScanNFC[7] = ReadBlock[7];
              delay(100);
              beep_sound();
            }
          }
        }
      }
    
      buttonState_SetCard = digitalRead(buttonPin_SetCard);
      if (buttonState_SetCard && !(Materials.indexOf("Z1") >= 0)){
            beep_sound();
            Materials = Materials + "Z1";
            delay(250);
      }
      buttonState_PlacePendulum = digitalRead(buttonPin_PlacePendulum);
      if (buttonState_PlacePendulum && !(Materials.indexOf("Z2") >= 0)){
            beep_sound();
            Materials = Materials + "Z2";
            delay(250);
      }
      buttonState_ATK = digitalRead(buttonPin_ATK);
      if (buttonState_ATK && !(Materials.indexOf("Z3") >= 0)){
            beep_sound();
            Materials = Materials + "Z3";
            delay(250);
      }
      buttonState_ActivateMTE = digitalRead(buttonPin_ActivateMTE);
      if (buttonState_ActivateMTE && !(Materials.indexOf("Z4") >= 0)){
            beep_sound();
            Materials = Materials + "Z4";
            delay(250);
      }
      buttonState_DEF = digitalRead(buttonPin_DEF);
      if (buttonState_DEF && !(Materials.indexOf("Z5") >= 0)){
            beep_sound();
            Materials = Materials + "Z5";
            delay(250);
      }
      buttonState_GraveBanish = digitalRead(buttonPin_GraveBanish);
      if (buttonState_GraveBanish && !(Materials.indexOf("Z6") >= 0)){
            beep_sound();
            Materials = Materials + "Z6";
            delay(250);
      }
      buttonState_SpecialSummon = digitalRead(buttonPin_SpecialSummon);
      if (buttonState_SpecialSummon && !(Materials.indexOf("Z7") >= 0)){
            beep_sound();
            Materials = Materials + "Z7";
            delay(250);
      }      
    }
    
    String Zone = "";
    Zone = Select_Zone();
    Serial.println("SPS"+ReadCardSS+Materials+Zone);
    Reset_Timers();
  }
}



void setup() {

  // Set input buttons
  pinMode(buttonPin_SetCard, INPUT);
  pinMode(buttonPin_PlacePendulum, INPUT);
  pinMode(buttonPin_ATK, INPUT);
  pinMode(buttonPin_ActivateMTE, INPUT);
  pinMode(buttonPin_DEF, INPUT);
  pinMode(buttonPin_GraveBanish, INPUT);
  pinMode(buttonPin_SpecialSummon, INPUT);
  pinMode(speaker, OUTPUT);

  // Configure NFC and serial monitors
  #ifndef ESP8266
    while (!Serial); // for Leonardo/Micro/Zero
  #endif
  Serial.begin(9600);
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    //Serial.print("No board");
    beep_sound();
    delay(200);
    beep_sound();
    delay(200);
    beep_sound();
    setup(); // Retry setup if error occurs
  }  
  nfc.SAMConfig();
  //Serial.print("Configuration Complete");
}

void loop() {
  
  
  Summon_Monster_Spell_Trap_Directly();      
  process_SetCard();
  process_PlacePendulum();
  process_ActivateMTE();
  process_GraveBanish();
  process_SpecialSummon(); 
}
