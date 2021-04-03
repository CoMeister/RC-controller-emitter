/**
 * TODO list:
 * maj selection model3
 * add a display object
 * menuModel check the for loop and if
 * 
 * button show 74hc165 X2
 * (74hc595)
 **/

#include "Arduino.h"
#include "SPI.h"
#include "RF24.h"
#include "printf.h"
#include "Model.h"
#include "Trim.h"
#include "HC165.h"
//#include <EEPROM.h>
#include "LiquidCrystal.h"

#define TRIMSTEP 2
#define BTNDELAY 3000

RF24 radio(9, 10); //CE, CSN

const uint64_t txAddr = 0xABCDABCD71;
const uint64_t rxAddr = 0x544d52687C;

const uint8_t gimballAddr[4] = {A1, A0, A2, A3}; //roll, pitch, throttle, yaw
HC165 serialInBtns;

//                           roll -         +         pitch -       +         yaw -         +         thr -         +            OK       menu/back     menu -      menu +
//const int btnArray[12][2] = {{835, 845}, {850, 863}, {865, 870}, {877, 887}, {890, 903}, {906, 925}, {917, 934}, {936, 948}, {952, 963}, {967, 983}, {985, 1000}, {1003, 1025}};
Model models[6] = {Model((char *)"Model-0"), Model((char *)"Model-1"), Model((char *)"Model-2"), Model((char *)"Model-3"), Model((char *)"Model-4"), Model((char *)"Model-5")};
uint8_t currentMenuModelPos = 0;
uint8_t currentModel = 0;
uint8_t currentTrimPos = 0;
uint8_t menuLCDpos = 0;

long lastPress = 0;
long showTrimTime = 0;

bool showTrim = false;
bool infoScreen = false;

int menuLevel = -1; //-1 --> no into the menu
bool press = true;

int lastAckReceived = 0;

const char *menuElem[4] = {"Models", "Trims", "Reverse command", "Channel"};
uint8_t currentMenuPos = 0;

//char*dataPre = "";

struct Commands
{
  uint8_t roll;     //roulis
  uint8_t pitch;    //tangage
  uint8_t throttle; //gaz
  uint8_t yaw;      //lacet
};

Commands commands;

const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 7, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int mapJoyVal(int val, int min, int mid, int max, bool rev);
void infosScreen();
void radioConfiguration();
void radioConfiguration();
void showMenuNav(uint8_t pos);
void universalMenuNave(bool way, uint8_t menuLength, uint8_t &pos); //& pos --> pass a reference
void menu();
void menuModels();
void updateCurrentModel(uint8_t index);
void incrementTrim(uint8_t index, int8_t trimStep);
void showTrims();
void trimInfos(uint8_t index);
void showRevs();

void setup()
{
  lcd.begin(20, 4);
  Serial.begin(9600);

  //models[currentModel].setName((char *)"Model de test");

  serialInBtns = HC165(A4, 2, A5, 2);

  radioConfiguration();
  printf_begin();
  radio.printDetails();
  //Serial.end();

  uint8_t count = 0;

  while (!radio.isChipConnected() && count < 5)
  {
    radioConfiguration();
    count++;
  }

  infosScreen();
  //menu();
  /*if(EEPROM.read(0) != 0x00){
    //models = EEPROM.read(0);
    Serial.println(EEPROM.read(0));
  }else{
    EEPROM.put(0, models);
  }*/

  // --------- testScripts ----------
}

void loop()
{

  commands.roll = mapJoyVal(analogRead(gimballAddr[0]) + models[currentModel].getTrim(0).getAmount(), 206, 688, 1023, models[currentModel].getTrim(0).isRev());
  commands.pitch = mapJoyVal(analogRead(gimballAddr[1]) + models[currentModel].getTrim(1).getAmount(), 71, 562, 1023, models[currentModel].getTrim(1).isRev());
  commands.throttle = mapJoyVal(analogRead(gimballAddr[2]) + models[currentModel].getTrim(2).getAmount(), 222, 669, 1023, models[currentModel].getTrim(2).isRev());
  commands.yaw = mapJoyVal(analogRead(gimballAddr[3]) + models[currentModel].getTrim(3).getAmount(), 133, 585, 1023, models[currentModel].getTrim(3).isRev());

  if (radio.isAckPayloadAvailable())
  {
    lastAckReceived = millis();
  }

  radio.write(&commands, sizeof(commands));

  int btnVal = serialInBtns.readByte();
  if (btnVal != 0 && millis() - lastPress > 250)
  {
    Serial.print(btnVal, BIN);
    Serial.print(" = ");
    Serial.println(btnVal);

    switch (btnVal)
    {
    case 1: //roll--
      incrementTrim(0, -TRIMSTEP);
      break;
    case 2: //roll++
      incrementTrim(0, TRIMSTEP);
      break;
    case 4: //pitch--
      incrementTrim(1, -TRIMSTEP);
      break;
    case 8: //pitch++
      incrementTrim(1, TRIMSTEP);
      break;
    case 16: //throttle--
      incrementTrim(2, -TRIMSTEP);
      break;
    case 32: //throttle++
      incrementTrim(2, TRIMSTEP);
      break;
    case 64: //yaw--
      incrementTrim(3, -TRIMSTEP);
      break;
    case 128: //yaw++
      incrementTrim(3, TRIMSTEP);
      break;
    case 256: //Menu/back
      if (menuLevel < 0)
      {
        menuLevel++;
      }
      else
      {
        menuLevel--;
      }
      if (menuLevel < 0)
      {
        infosScreen();
      }
      else
      {
        menu();
      }
      break;
    case 512: //ok
      if (menuLevel == 0)
      {
        if (currentMenuPos == 0)
        {
          menuModels();
        }
        else if (currentMenuPos == 1)
        {
          showTrims();
        }
        else if (currentMenuPos == 2)
        {
          showRevs();
        }
        menuLevel++;
        Serial.print("menuLevel = ");
        Serial.print(menuLevel);
        Serial.print(" CurrentMenuPos = ");
        Serial.println(currentMenuPos);
      }
      else if (menuLevel == 1)
      {
        if (currentMenuPos == 0)
        {
          updateCurrentModel(currentMenuModelPos);
          menuModels();
        }
        else if (currentMenuPos == 2)
        {
          models[currentModel].reverseTrim(currentTrimPos);
          //delay(10);
          showRevs();
        }
      }
      break;
    case 1024: //Menu--
      if (menuLevel == 0)
      {
        //menuNav(false);
        universalMenuNave(false, *(&menuElem + 1) - menuElem, currentMenuPos);
      }
      else if (menuLevel == 1 && currentMenuPos == 0)
      {
        //menuModelsNav(false);
        universalMenuNave(false, *(&models + 1) - models, currentMenuModelPos);
        menuModels();
      }
      else if (menuLevel == 1 && currentMenuPos == 2)
      {
        //universalMenuNave(false, models[currentModel].getTrimLength(), (screen.getWidth()-40)/models[currentModel].getTrimLength(), currentTrimPos);
        universalMenuNave(false, models[currentModel].getTrimLength(), currentTrimPos);
      }
      break;
    case 2048:            //Menu++
      if (menuLevel == 0) //menulevel = 0 => 1st menu, menulevel = 1 => subMenu
      {
        //menuNav(true);
        universalMenuNave(true, *(&menuElem + 1) - menuElem, currentMenuPos);
      }
      else if (menuLevel == 1 && currentMenuPos == 0)
      {
        //menuModelsNav(true);
        universalMenuNave(true, *(&models + 1) - models, currentMenuModelPos);
        menuModels();
      }
      else if (menuLevel == 1 && currentMenuPos == 2)
      {
        //universalMenuNave(true, models[currentModel].getTrimLength(), (screen.getWidth()-40)/models[currentModel].getTrimLength(), currentTrimPos);
        universalMenuNave(true, models[currentModel].getTrimLength(), currentTrimPos);
      }
      break;

      /*case 4096:  //
        break;
      case 8192:  //
        break;
      case 16384:  //
        break;
      case 32768:  //
        break;*/
    }

    lastPress = millis();
  }

  if (showTrim && millis() - showTrimTime > 2500)
  {
    showTrimTime = millis();
    infoScreen = true;
  }
  else if (infoScreen)
  {
    showTrim = false;
    infosScreen();
    infoScreen = false;
  }
}

//47 60 75
int mapJoyVal(int val, int min, int mid, int max, bool rev)
{
  //constrain(val, min, max);
  if (val < min)
  {
    val = min;
  }
  else if (val > max)
  {
    val = max;
  }

  if (val < mid)
  {
    val = map(val, min, mid, 0, 128);
  }
  else
  {
    val = map(val, mid, max, 128, 255);
  }

  return (rev ? 255 - val : val); //comparison operator
}

void infosScreen()
{
  lcd.clear();
  lcd.print("Mega RC");
  lcd.setCursor(0, 1);
  lcd.print(models[currentModel].getName());
}

void radioConfiguration()
{
  radio.begin(); //activer le module
  radio.setChannel(0x01);
  radio.setRetries(15, 15);      //setter pour réessayer d'envoyer une requète 1er arguments -> fréquence *250millis du retry | 2em arguments -> combien d'essaye avant l'abandon
  radio.openWritingPipe(txAddr); //setter l'adresse du récepteur (tableau)
  radio.openReadingPipe(1, rxAddr);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setCRCLength(RF24_CRC_8);
  radio.setAutoAck(true);
  radio.startListening();
  radio.stopListening();
  radio.powerUp();
}

void showMenuNav(uint8_t pos)
{
  lcd.setCursor(0, pos);
  lcd.print(">");
}

void universalMenuNave(bool way, uint8_t menuLength, uint8_t &pos) //manage pointer/addr update
{
  lcd.setCursor(0, pos);
  lcd.print(" ");

  if (way)
  {
    if (pos < menuLength - 1)
    {
      pos += 1;
    }
    else
    {
      pos = 0;
    }
  }
  else
  {
    if (pos > 0)
    {
      pos -= 1;
    }
    else
    {
      pos = menuLength - 1;
    }
  }
  /*if (way)
  {
    if (*pos < menuLength - 1)
    {
      *pos++;
      if (menuLCDpos < 4)
      {
        menuLCDpos++;
      }
    }
    else
    {
      *pos = 0;
      menuLCDpos = 0;
    }
  }
  else
  {
    if (*pos > 0)
    {
      *pos--;
      if (menuLCDpos > 0)
      {
        menuLCDpos--;
      }
    }
    else
    {
      *pos = menuLength - 1;
      menuLCDpos = 3;
    }
  }*/
  showMenuNav(pos);
}

void menu()
{
  lcd.clear();
  //char *strBuff = (char *)"";
  //lcd.setCursor(0,0);
  //uint8_t pos = 0;
  //int sizeOfanArray = *(&array + 1) - array;
  uint8_t menuElemLength = sizeof(menuElem) / sizeof(menuElem[0]);
  for (uint8_t i = 0; i < menuElemLength; ++i)
  {
    lcd.setCursor(2, i);
    lcd.print(menuElem[i]);
  }
  //"Models", "Trims", "Reverse command", "Channel"
  /*lcd.setCursor(2, 0);
  lcd.print("Models");
  lcd.setCursor(2, 1);
  lcd.print("Trims");

  lcd.setCursor(2, 2);
  lcd.print("Reverse command");

  lcd.setCursor(2, 3);
  lcd.print("Channel");*/

  showMenuNav(currentMenuPos);
}

void menuModels()
{
  lcd.clear();
  Serial.println("----- menuModels -----");

  //for (uint8_t i = lastElemToShow - 4; i < lastElemToShow + 1; ++i)
  Serial.println("Loop to print models");
  int j = 0;
  if (currentMenuModelPos > 3)
  {
    for (uint8_t i = currentMenuModelPos - 3; i < currentMenuModelPos + 1; ++i)
    {
      lcd.setCursor(2, j);
      j++;

      if (i < sizeof(models) / sizeof(models[i]))
      {
        if (currentModel == i)
        {
          //lcd.setCursor(1, i - (lastElemToShow - 3));
          lcd.print(models[i].getName());
          lcd.print("-");
          Serial.print("selected model: ");
          Serial.println(models[i].getName());
        }
        else
        {
          lcd.print(models[i].getName());
          Serial.print("No selected model: ");
          Serial.println(models[i].getName());
        }
      }
    }
  }
  else
  {
    for (uint8_t i = 0; i < 4; ++i)
    {
      lcd.setCursor(2, i);
      if (i < sizeof(models) / sizeof(models[i]))
      {
        if (currentModel == i)
        {
          //lcd.setCursor(1, i - (lastElemToShow - 3));
          lcd.print(models[i].getName());
          lcd.print("-");
          Serial.print("selected model: ");
          Serial.println(models[i].getName());
        }
        else
        {
          lcd.print(models[i].getName());
          Serial.print("No selected model: ");
          Serial.println(models[i].getName());
        }
      }
    }
  }

  showMenuNav(currentMenuModelPos);
}

void updateCurrentModel(uint8_t index)
{
  //uint8_t pos;// = 20 + 20 * currentModel;

  currentModel = currentMenuModelPos;
  //pos = 20 + 20 * currentModel;
  //showMenuNav(currentMenuModelPos, 20);
}

void incrementTrim(uint8_t index, int8_t trimStep)
{
  models[currentModel].incTrim(index, trimStep);
  if (currentMenuPos == 1 && menuLevel == 1)
  {
    showTrims();
  }
  else
  {
    trimInfos(index);
  }
}

void showTrims()
{
  lcd.clear();
  lcd.setCursor(0, 0);

  //char*trimNames[4] = {"Roll", "Pitch", "Throttle", "Yaw"};
  //uint8_t pos = 20;
  //int sizeOfanArray = *(&array + 1) - array;
  uint8_t trimsLength = models[currentModel].getTrimLength();
  for (uint8_t i = 0; i < trimsLength; ++i)
  {
    lcd.setCursor(0, i);
    lcd.print(models[currentModel].getTrim(i).getName());
    lcd.setCursor(17, i);
    lcd.print(models[currentModel].getTrim(i).getAmount());

    //pos+=(screen.getWidth()-40)/trimsLength;
  }
}

void trimInfos(uint8_t index)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(models[currentModel].getTrim(index).getName());
  lcd.setCursor(2, 0);
  lcd.print(models[currentModel].getTrim(index).getAmount());
  showTrim = true;
}

void showRevs()
{
  lcd.clear();
  lcd.setCursor(0, 0);

  //char*trimNames[4] = {"Roll", "Pitch", "Throttle", "Yaw"};
  //uint8_t pos = 20;
  //int sizeOfanArray = *(&array + 1) - array;
  uint8_t trimsLength = models[currentModel].getTrimLength();
  for (uint8_t i = 0; i < trimsLength; ++i)
  {
    lcd.setCursor(2, i);
    lcd.print(models[currentModel].getTrim(i).getName());
    lcd.setCursor(16, i);
    if (models[currentModel].getTrim(i).isRev())
    {
      lcd.print("rev");
    }
    else
    {
      lcd.print("nor");
    }
  }
  showMenuNav(currentTrimPos);
}
