/**
 * TODO list:
 * problem with vibration, connection's lost
 * TEST
 **/


#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include "Ucglib.h"
#include <printf.h>

#define TRIMSTEPS 1
#define BTNDELAY 100

RF24 radio(9,10); //CE, CSN

const uint64_t txAddr = 0xABCDABCD71;
const uint64_t rxAddr = 0x544d52687C;

const int gimballAddr[4] = {A0, A1, A2, A3}; //roll, pitch, throttle, yaw
const int btnValue = A4;

const int btnArray[12][2] = {{840,843}, {851,856}, {860,868}, {879,883}, {890,898}, {907,915}, {920,930}, {937,948}, {954,962}, {970,982}, {989,1000}, {1009,1023}};
int trims[4] = {0,0,0,0};
int timeAtLastBtnPressed = 0;

int indexPressedBtn = -1;

int lastAckReceived = 0;
int ledDelay = 0;

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



/*void searchRemoteMachine(){
  if(radio.isAckPayloadAvailable()){
    char data[32] = "";
    radio.read(&data,sizeof(data));
    //Serial.println("Acknolewge: " + String(data)); //name
  }
}*/
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

void showTrim(int trimID, int trimsVal){
  volatile int width = 100;
  volatile int height = 40;
  volatile int x = screen.getWidth()/2-width/2;
  volatile int y = screen.getHeight()/2-height/2;
  screen.setColor(0,0,0);
  screen.drawBox(x, y, width, height);
  screen.setColor(255,255,255);
  screen.drawFrame(x-1, y-1, width+2, height+2);

  screen.setPrintPos(x+5,screen.getHeight()/2);
  screen.print("Trims " + String(trimID) + ": " + String(trimsVal));
  
}

void setup() {
  Serial.begin(9600);
  for(int i = 0; i < 4; i++){
    pinMode(gimballAddr[i], INPUT);
  }

  pinMode(btnValue, INPUT);
  
  radioConfiguration();
  printf_begin();
  radio.printDetails();
  //Serial.end();

  volatile int count = 0;

  while(!radio.isChipConnected() && count < 5){
    radioConfiguration();
    count ++;
  }

  screen.begin(UCG_FONT_MODE_SOLID);
  screen.setRotate270();
  screen.clearScreen();

  screen.setFont(ucg_font_helvB08_hf);
  screen.setPrintPos(0,25);
  screen.setColor(255, 255, 255);
  screen.print("Hello World!");
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

  
  volatile int val = analogRead(btnValue);
  if(val > 0 && (millis() - timeAtLastBtnPressed) >= BTNDELAY){
    for(int i = 0; i < 12; i++){  //watch all btns
      if(val >= btnArray[i][0] && val <= btnArray[i][1]){ //check which button is pressed
        indexPressedBtn = i;  //set button pressed indexPressedBtn
        Serial.println(indexPressedBtn);
        switch (indexPressedBtn) {  //button action list
          case 0:
            trims[0]+=TRIMSTEPS;
            showTrim(0, trims[0]);
            break;
          case 1:
            trims[0]-=TRIMSTEPS;
            showTrim(0, trims[0]);
            break;
          case 2:
            trims[1]+=TRIMSTEPS;
            showTrim(1, trims[1]);
            break;
          case 3:
            trims[1]-=TRIMSTEPS;
            showTrim(1, trims[1]);
            break;
          case 4:
            trims[2]+=TRIMSTEPS;
            showTrim(2, trims[2]);
            break;
          case 5:
            trims[2]-=TRIMSTEPS;
            showTrim(2, trims[2]);
            break;
          case 6:
            trims[3]+=TRIMSTEPS;
            showTrim(3, trims[3]);
            break;
          case 7:
            trims[3]-=TRIMSTEPS;
            showTrim(3, trims[3]);
            break;
          case 8:
            //menu/return
            break;
          case 9:
            //menu+
            break;
          case 10:
            //menu-
            break;
          case 11:
            //menu ok
            break;
          indexPressedBtn = -1;
          break;
        }
      }
    }
    timeAtLastBtnPressed = millis();
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
