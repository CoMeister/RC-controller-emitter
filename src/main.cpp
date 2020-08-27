/**
 * TODO list:
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

#define TRIMSTEPS 2
#define BTNDELAY 100

RF24 radio(9,10); //CE, CSN

const uint64_t txAddr = 0xABCDABCD71;
const uint64_t rxAddr = 0x544d52687C;

const byte gimballAddr[4] = {A0, A1, A2, A3}; //roll, pitch, throttle, yaw
const byte btnValue = A4;

const int btnArray[12][2] = {{800,843}, {851,856}, {860,868}, {879,883}, {890,898}, {907,915}, {920,929}, {934,948}, {950,962}, {970,982}, {989,1000}, {1009,1023}};
int trims[4] = {0,0,0,0};
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
  screen.setFont(ucg_font_fur11_hf);
  screen.setColor(230, 57, 70);
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

void menuNav(){
  screen.setColor(241, 250, 238);
  screen.drawFrame(0, currentMenuPos*17+2, screen.getWidth(), 15);
}

void menuNav(bool way){
  byte menuElemLength = *(&menuElem + 1) - menuElem;
  screen.setColor(0,0,0);
  screen.drawFrame(0, currentMenuPos*17+2, screen.getWidth(), 15);
  if(way){
    if(currentMenuPos < menuElemLength-1){
      currentMenuPos++;
    }else{
      currentMenuPos=0;
    }
  }else{
    if(currentMenuPos > 0){
      currentMenuPos--;
    }else{
      currentMenuPos = menuElemLength-1;
    }
  }
  menuNav();
}

void menu(){
  screen.clearScreen();
  screen.setFont(ucg_font_fur11_hf);
  screen.setColor(241, 250, 238);
  byte pos = 15;
  //int sizeOfanArray = *(&array + 1) - array;
  byte menuElemLength = *(&menuElem + 1) - menuElem;
  for(byte i = 0; i < menuElemLength; i++){
    screen.setPrintPos(5, pos);
    screen.print(menuElem[i]);
    pos+=17;
  }
  menuNav();
}

void showTrims(){
  screen.clearScreen();
  screen.setFont(ucg_font_fur11_hf);
  screen.setColor(241, 250, 238);

  String trimNames[4] = {"Roll", "Pitch", "Throttle", "Yaw"};
  byte pos = 15;
  //int sizeOfanArray = *(&array + 1) - array;
  byte trimNamesLength = *(&trimNames + 1) - trimNames;
  for(byte i = 0; i < trimNamesLength; i++){
    screen.setPrintPos(5, pos);
    screen.print(trimNames[i]);
    screen.setPrintPos(100, pos);
    screen.print(trims[i]);

    pos+=(screen.getWidth()-30)/trimNamesLength;
  }
}

void updateTrimVal(int trimID){
  screen.setFont(ucg_font_fur11_hf);
  screen.setColor(178, 247, 239);
  byte pos = 15;
  byte trimsLength = *(&trims + 1) - trims;
  for(byte i = 0; i < trimsLength; i++){
     if(trimID == i){
        screen.setPrintPos(100, pos);
        screen.print(String(trims[i]) + "    ");
        return;
     }
     pos+=(screen.getWidth()-30)/trimsLength;
  }
}

void setup() {
  Serial.begin(9600);
  for(byte i = 0; i < 4; i++){
    pinMode(gimballAddr[i], INPUT);
  }

  pinMode(btnValue, INPUT);
  
  radioConfiguration();
  printf_begin();
  radio.printDetails();
  Serial.end();

  byte count = 0;

  while(!radio.isChipConnected() && count < 5){
    radioConfiguration();
    count ++;
  }
  

  screen.begin(UCG_FONT_MODE_SOLID);
  screen.setRotate270();
  
  showInfosScreen();
  //menu();
}

long printDel = 0;

void loop() {

  commands.roll = mapJoyVal(analogRead(gimballAddr[0])+trims[0], 206, 688, 1023, false);
  commands.pitch = mapJoyVal(analogRead(gimballAddr[1])+trims[1], 71, 562, 1023, false);
  commands.throttle = mapJoyVal(analogRead(gimballAddr[2])+trims[2], 222, 669, 1023, false);
  commands.yaw = mapJoyVal(analogRead(gimballAddr[3])+trims[3], 133, 585, 1023, false);

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
            trims[0]+=TRIMSTEPS;
            if(menuLevel == 1 && currentMenuPos == 1){
              updateTrimVal(0);
            }
            //showTrim(0, trims[0]);
            break;
          case 1:
            trims[0]-=TRIMSTEPS;
            if(menuLevel == 1 && currentMenuPos == 1){
              updateTrimVal(0);
            }
            //showTrim(0, trims[0]);
            break;
          case 2:
            trims[1]+=TRIMSTEPS;
            if(menuLevel == 1 && currentMenuPos == 1){
              updateTrimVal(1);
            }
            //showTrim(1, trims[1]);
            break;
          case 3:
            trims[1]-=TRIMSTEPS;
            if(menuLevel == 1 && currentMenuPos == 1){
              updateTrimVal(1);
            }
            //showTrim(1, trims[1]);
            break;
          case 4:
            trims[2]+=TRIMSTEPS;
            if(menuLevel == 1 && currentMenuPos == 1){
              updateTrimVal(2);
            }
            //showTrim(2, trims[2]);
            break;
          case 5:
            trims[2]-=TRIMSTEPS;
            if(menuLevel == 1 && currentMenuPos == 1){
              updateTrimVal(2);
            }
            break;
          case 6:
            trims[3]+=TRIMSTEPS;
            if(menuLevel == 1 && currentMenuPos == 1){
              updateTrimVal(3);
            }
            //showTrim(3, trims[3]);
            break;
          case 7:
            trims[3]-=TRIMSTEPS;
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
              menuNav(false);
            }
            break;
          case 10:
            //menu+
            if(menuLevel == 0){
              menuNav(true);
            }
            break;
          case 11:
            //menu ok
            if(currentMenuPos == 1 && menuLevel > -1){
              showTrims();
            }
            menuLevel++;
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
  
  
  /*if((millis()-printDel) >= 10){
    screen.setPrintPos(0,50);
    screen.print("roll: " + String(commands.roll) + "  ");
    screen.setPrintPos(0,60);
    screen.print("pitch: " + String(commands.pitch) + "  ");
    screen.setPrintPos(0,70);
    screen.print("throttle: " + String(commands.throttle) + "  ");
    screen.setPrintPos(0,80);
    screen.print("yaw: " + String(commands.yaw) + "  ");
    screen.setPrintPos(0,100);
    screen.print("lastAckReceived : " + String(millis() - lastAckReceived) + "     ");
    printDel=millis();
  }*/
  delay(10);
}
