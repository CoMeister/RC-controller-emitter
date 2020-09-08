/**
 * TODO list:
 * maj selection model3
 * 
 * 
 * colorpalette
 * red: 230, 57, 70
 * white: 241, 250, 238
 * wblue: 168, 218, 220
 * blue: 69, 123, 157
 * dblue: 29, 53, 87
 * 
 **/

#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include "Ucglib.h"
#include <printf.h>
#include <Model.h>
#include <Trim.h>
#include <EEPROM.h>

#define TRIMSTEP 2
#define BTNDELAY 500

RF24 radio(9,10); //CE, CSN

const uint64_t txAddr = 0xABCDABCD71;
const uint64_t rxAddr = 0x544d52687C;

const byte gimballAddr[4] = {A0, A1, A2, A3}; //roll, pitch, throttle, yaw
const byte btnValue = A4;

const int btnArray[12][2] = {{800,843}, {851,856}, {860,868}, {879,883}, {890,898}, {907,915}, {920,929}, {934,948}, {950,962}, {970,982}, {989,1000}, {1009,1023}};
Model models[8] = {Model("Model - 0"), Model("Model - 1"), Model("Model - 2"), Model("Model - 3"), Model("Model - 4"), Model("Model - 5"), Model("Model - 6"), Model("Model - 7")};
byte currentMenuModelPos = 0;
byte currentModel = 0;
byte currentTrimPos = 0;
byte timeAtLastBtnPressed = 0;
int menuLevel = -1; //-1 --> no into the menu
bool press = true;

byte indexPressedBtn = -1;

int lastAckReceived = 0;

const String menuElem[4] = {"Models", "Trims", "Reverse command", "Channel"};
byte currentMenuPos = 0;

//String dataPre = "";

struct Commands
{
  byte roll;  //roulis
  byte pitch; //tangage
  byte throttle; //gaz
  byte yaw; //lacet
};


struct Colors{
  byte r;
  byte g;
  byte b;
};

Colors red = {230, 57, 70};
Colors white = {241, 250, 238};
Colors wblue = {168, 218, 220};
Colors blue = {69, 123, 157};
Colors dblue = {29, 53, 87};

Commands commands;
Ucglib_ST7735_18x128x160_HWSPI screen(/*cd=*/ 9 , /*cs=*/ 8, /*reset=*/ 7);

//47 60 75
int mapJoyVal(int val, int min, int mid, int max, bool rev){
  //constrain(val, min, max);
  if(val<min){
    val=min;
  }else if(val>max){
    val=max;
  }

  if(val < mid){
    val = map(val, min, mid, 0, 128);
  }else{
    val = map(val, mid, max, 128, 255);
  }
  
  return ( rev ? 255 - val : val );   //j'ai pas encore compris comment ça marche
}

void showInfosScreen(){
  screen.clearScreen();
  screen.setFont(ucg_font_fur11_hr);
  screen.setColor(red.r, red.g, red.b);
  screen.setPrintPos(5,15);
  screen.print("Radio v1.0");
}

void radioConfiguration(){
  radio.begin();                    //activer le module
  radio.setChannel(0x01);
  radio.setRetries(15, 15);         //setter pour réessayer d'envoyer une requète 1er arguments -> fréquence *250millis du retry | 2em arguments -> combien d'essaye avant l'abandon
  radio.openWritingPipe(txAddr);    //setter l'adresse du récepteur (tableau)
  radio.openReadingPipe(1, rxAddr);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setCRCLength(RF24_CRC_8);
  radio.setAutoAck(true);
  radio.startListening();
  radio.stopListening();
  radio.powerUp();
}

void showMenuNav(byte pos, byte gap){
  screen.setColor(white.r, white.g, white.b);
  screen.drawFrame(0, pos*gap+4, screen.getWidth(), 21);
}

void universalMenuNave(bool way, byte menuLength, byte gap, byte &pos){
  screen.setColor(0,0,0);
  screen.drawFrame(0, pos*gap+4, screen.getWidth(), 21);
  if(way){
    if(pos < menuLength-1){
      pos++;
    }else{
      pos=0;
    }
  }else{
    if(pos > 0){
      pos--;
    }else{
      pos = menuLength-1;
    }
  }
  showMenuNav(pos, gap);
}

void menu(){
  screen.clearScreen();
  screen.setFont(ucg_font_fur11_hr);
  screen.setColor(white.r, white.g, white.b);
  byte pos = 20;
  //int sizeOfanArray = *(&array + 1) - array;
  byte menuElemLength = *(&menuElem + 1) - menuElem;
  for(byte i = 0; i < menuElemLength; i++){
    screen.setPrintPos(5, pos);
    screen.print(menuElem[i]);
    pos+=20;
  }
  showMenuNav(currentMenuPos, 20);
}

void menuModels(){
  screen.clearScreen();
  screen.setFont(ucg_font_fur11_hr);
  screen.setColor(white.r, white.g, white.b);
  byte pos = 20;
  //int sizeOfanArray = *(&array + 1) - array;
  byte modelsLength = *(&models + 1) - models;
  for(byte i = 0; i < modelsLength; i++){
    screen.setPrintPos(5, pos);
    if(currentModel == i){
      screen.setColor(blue.r, blue.g, blue.b);
      screen.print(models[i].getName());
      pos+=20;
      screen.setColor(white.r, white.g, white.b);
    }else{
      
      screen.print(models[i].getName());
      pos+=20;
    }
  }
  showMenuNav(currentMenuModelPos, 20);
}

void updateCurrentModel(byte index){
  screen.setColor(white.r, white.g, white.b);
  byte pos = 20 + 20*currentModel;
  screen.setPrintPos(5, pos);
  screen.print(models[currentModel].getName());

  currentModel = currentMenuModelPos;

  screen.setColor(blue.r, blue.g, blue.b);
  pos = 20 + 20*currentModel;
  screen.setPrintPos(5, pos);
  screen.print(models[currentModel].getName());
  //showMenuNav(currentMenuModelPos, 20);
}

void showTrims(){
  screen.clearScreen();
  screen.setFont(ucg_font_fur11_hr);
  screen.setColor(white.r, white.g, white.b);

  //String trimNames[4] = {"Roll", "Pitch", "Throttle", "Yaw"};
  byte pos = 20;
  //int sizeOfanArray = *(&array + 1) - array;
  byte trimsLength = models[currentModel].getTrimLength();
  for(byte i = 0; i < trimsLength; i++){
    screen.setPrintPos(5, pos);
    screen.print(models[currentModel].getTrim(i).getName());
    screen.setPrintPos(100, pos);
    screen.print(models[currentModel].getTrim(i).getAmount());

    pos+=(screen.getWidth()-40)/trimsLength;
  }
}

void showRevs(){
  screen.clearScreen();
  screen.setFont(ucg_font_fur11_hr);

  //String trimNames[4] = {"Roll", "Pitch", "Throttle", "Yaw"};
  byte pos = 20;
  //int sizeOfanArray = *(&array + 1) - array;
  byte trimsLength = models[currentModel].getTrimLength();
  for(byte i = 0; i < trimsLength; i++){
    screen.setColor(white.r, white.g, white.b);
    screen.setPrintPos(5, pos);
    screen.print(models[currentModel].getTrim(i).getName());
    screen.setPrintPos(70, pos);
    if(models[currentModel].getTrim(i).isRev()){
      screen.setColor(red.r, red.g, red.b);
      screen.print("Reversed");
    }else{
      screen.setColor(wblue.r, wblue.g, wblue.b);
      screen.print("Normal");
    }

    pos+=(screen.getWidth()-40)/trimsLength;
  }
  showMenuNav(currentTrimPos, (screen.getWidth()-40)/trimsLength);
}

void updateTrimVal(int trimID){
  screen.setFont(ucg_font_fur11_hr);
  screen.setColor(178, 247, 239);
  byte pos = 20;
  byte triml = models[currentModel].getTrimLength();
  for(byte i = 0; i < triml; i++){
     if(trimID == i){
        screen.setPrintPos(100, pos);
        screen.print(String(models[currentModel].getTrim(i).getAmount()) + "    ");
        return;
     }
     pos+=(screen.getWidth()-30)/triml;
  }
}

void setup() {
  Serial.begin(9600);

  models[currentModel].setName("Model de test");

  pinMode(btnValue, INPUT);
  
  radioConfiguration();
  printf_begin();
  radio.printDetails();
  //Serial.end();

  byte count = 0;

  while(!radio.isChipConnected() && count < 5){
    radioConfiguration();
    count ++;
  }
  

  screen.begin(UCG_FONT_MODE_SOLID);
  screen.setRotate270();
  
  showInfosScreen();
  //menu();
  /*if(EEPROM.read(0) != 0x00){
    //models = EEPROM.read(0);
    Serial.println(EEPROM.read(0));
  }else{
    EEPROM.put(0, models);
  }*/
  
}

void loop() {

  commands.roll = mapJoyVal(analogRead(gimballAddr[0])+models[currentModel].getTrim(0).getAmount(), 206, 688, 1023, models[currentModel].getTrim(0).isRev());
  commands.pitch = mapJoyVal(analogRead(gimballAddr[1])+models[currentModel].getTrim(1).getAmount(), 71, 562, 1023, models[currentModel].getTrim(1).isRev());
  commands.throttle = mapJoyVal(analogRead(gimballAddr[2])+models[currentModel].getTrim(2).getAmount(), 222, 669, 1023, models[currentModel].getTrim(2).isRev());
  commands.yaw = mapJoyVal(analogRead(gimballAddr[3])+models[currentModel].getTrim(3).getAmount(), 133, 585, 1023, models[currentModel].getTrim(3).isRev());

  if(radio.isAckPayloadAvailable()){
    lastAckReceived = millis();
  }

  radio.write(&commands, sizeof(Commands));

  
  int val = analogRead(btnValue);
  //Serial.print(String(val));
  if(val > 0 && (millis() - timeAtLastBtnPressed) >= BTNDELAY && press){
    for(byte i = 0; i < 12; i++){  //watch all btns
      if(val >= btnArray[i][0] && val <= btnArray[i][1]){ //check which button is pressed
        indexPressedBtn = i;  //set button pressed indexPressedBtn
        //Serial.println(" --> " + String(indexPressedBtn));
        switch (indexPressedBtn) {  //button action list
          case 0:
            models[currentModel].incTrim(0, TRIMSTEP);
            if(menuLevel == 1 && currentMenuPos == 1){
              updateTrimVal(0);
            }
            break;
          case 1:
            models[currentModel].incTrim(0, -TRIMSTEP);
            if(menuLevel == 1 && currentMenuPos == 1){
              updateTrimVal(0);
            }
            //showTrim(0, trims[0]);
            break;
          case 2:
            models[currentModel].incTrim(1, TRIMSTEP);
            if(menuLevel == 1 && currentMenuPos == 1){
              updateTrimVal(1);
            }
            //showTrim(1, trims[1]);
            break;
          case 3:
            models[currentModel].incTrim(1, -TRIMSTEP);
            if(menuLevel == 1 && currentMenuPos == 1){
              updateTrimVal(1);
            }
            //showTrim(1, trims[1]);
            break;
          case 4:
            models[currentModel].incTrim(2, TRIMSTEP);
            if(menuLevel == 1 && currentMenuPos == 1){
              updateTrimVal(2);
            }
            //showTrim(2, trims[2]);
            break;
          case 5:
            models[currentModel].incTrim(2, -TRIMSTEP);
            if(menuLevel == 1 && currentMenuPos == 1){
              updateTrimVal(2);
            }
            break;
          case 6:
            models[currentModel].incTrim(3, TRIMSTEP);
            if(menuLevel == 1 && currentMenuPos == 1){
              updateTrimVal(3);
            }
            //showTrim(3, trims[3]);
            break;
          case 7:
            models[currentModel].incTrim(0, -TRIMSTEP);
            if(menuLevel == 1 && currentMenuPos == 1){
              updateTrimVal(3);
            }
            //showTrim(3, trims[3]);
            break;
          case 8:
            //menu/return
            if(menuLevel < 0){
              menuLevel++;
            }else{
              menuLevel--;
            }
            if(menuLevel < 0){
              showInfosScreen();
            }else{
              menu();
            }
            break;
          case 9:
            //menu-
            if(menuLevel == 0){
              //menuNav(false);
              universalMenuNave(false, *(&menuElem + 1) - menuElem, 20, currentMenuPos);
            }else if(menuLevel == 1 && currentMenuPos == 0){
              //menuModelsNav(false);
              universalMenuNave(false, *(&models + 1) - models, 20, currentMenuModelPos);
            }else if(menuLevel == 1 && currentMenuPos == 2){
              universalMenuNave(false, models[currentModel].getTrimLength(), (screen.getWidth()-40)/models[currentModel].getTrimLength(), currentTrimPos);
            }
            break;
          case 10:
            //menu+
            if(menuLevel == 0){
              //menuNav(true);
              universalMenuNave(true, *(&menuElem + 1) - menuElem, 20, currentMenuPos);
            }else if(menuLevel == 1 && currentMenuPos == 0){
              //menuModelsNav(true);
              universalMenuNave(true, *(&models + 1) - models, 20, currentMenuModelPos);
            }else if(menuLevel == 1 && currentMenuPos == 2){
              universalMenuNave(true, models[currentModel].getTrimLength(), (screen.getWidth()-40)/models[currentModel].getTrimLength(), currentTrimPos);
            }
            break;
          case 11:
            //menu ok
            if(menuLevel == 0){
              if(currentMenuPos == 0){
                menuModels();
              }else if(currentMenuPos == 1){
                showTrims();
              }else if(currentMenuPos == 2){
                showRevs();
              }
              menuLevel++;
            }else if(menuLevel == 1){
              if(currentMenuPos == 0){
                updateCurrentModel(currentMenuModelPos);
              }else if(currentMenuPos == 2){
                models[currentModel].reverseTrim(currentTrimPos);

                screen.setPrintPos(70, 20 + currentTrimPos*(screen.getWidth()-40)/models[currentModel].getTrimLength());
                if(models[currentModel].getTrim(currentTrimPos).isRev()){
                  screen.setColor(red.r, red.g, red.b);
                  screen.print("Reversed");
                }else{
                  screen.setColor(wblue.r, wblue.g, wblue.b);
                  screen.print("Normal    ");
                }
              }
              //showMenuNav(currentTrimPos, (screen.getWidth()-30)/models[currentModel].getTrimLength());
            }            
            break;
        }
        indexPressedBtn = -1;
      }
    }
    timeAtLastBtnPressed = millis();
    press = false;
  }else{
    press = true;
  }
  delay(10);
}
