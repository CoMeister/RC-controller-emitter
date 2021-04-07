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
#include <EEPROM.h>
#include "LiquidCrystal.h"

#define TRIMSTEP 2
#define BTNDELAY 250
#define MODEL_NAME_LENGTH 6

#define ROLL_LESS 1
#define ROLL_MORE 2
#define PITCH_LESS 4
#define PITCH_MORE 8
#define THROTTLE_LESS 16
#define THROTTLE_MORE 32
#define YAW_LESS 64
#define YAW_MORE 128
#define MENU_B 256
#define OK 512
#define MENU_LESS 1024
#define MENU_MORE 2048
#define OPT0 4096
#define OPT1 8192
#define OPT2 16384
#define OPT3 32768

RF24 radio(9, 10); //CE, CSN

const uint8_t txAddr = 0x02;
const uint8_t rxAddr = 0x01;

const uint8_t gimballAddr[4] = {A1, A0, A2, A3}; //roll, pitch, throttle, yaw
HC165 serialInBtns;

//                           roll -         +         pitch -       +         yaw -         +         thr -         +            OK       menu/back     menu -      menu +
//const int btnArray[12][2] = {{835, 845}, {850, 863}, {865, 870}, {877, 887}, {890, 903}, {906, 925}, {917, 934}, {936, 948}, {952, 963}, {967, 983}, {985, 1000}, {1003, 1025}};

Model models[6] = {Model((char *)"MODEL0"), Model((char *)"MODEL1"), Model((char *)"MODEL2"), Model((char *)"MODEL3"), Model((char *)"MODEL4"), Model((char *)"MODEL5")};
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
void showMenuNav(uint8_t pos);
void universalMenuNave(bool way, uint8_t menuLength, uint8_t &pos); //& pos --> pass a reference
void menu();
void menuModels();
void setModelName(uint8_t index);
void incrementTrim(uint8_t index, int8_t trimStep);
void showTrims();
void trimInfos(uint8_t index);
void showRevs();
void saveModels();
void loadModels();

void setup()
{
  for (uint8_t i = 0; i < sizeof(gimballAddr) / sizeof(int); i++)
  {
    pinMode(gimballAddr[i], INPUT);
  }

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

  /*Serial.print("Size of Model array: ");
  Serial.println(sizeof(models), DEC);

  Serial.print("Size of one Model: ");
  Serial.println(sizeof(models[0]));*/

  /*Model eepromModels[6];
  delay(200);

  EEPROM.get(0, eepromModels);

  Serial.print("Model 2 name: ");
  Serial.println(eepromModels[2].getName());

  Serial.println(eepromModels[2].getTrim(2).getAmount());*/

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

  Serial.print("Pitch: ");
  Serial.println(analogRead(gimballAddr[1]) + models[currentModel].getTrim(1).getAmount());

  Serial.print("Throttle: ");
  Serial.println(analogRead(gimballAddr[2]) + models[currentModel].getTrim(2).getAmount());

  if (radio.isAckPayloadAvailable())
  {
    lastAckReceived = millis();
  }

  radio.write(&commands, sizeof(commands));

  int btnVal = serialInBtns.readByte();
  if (btnVal != 0 && millis() - lastPress > BTNDELAY)
  {
    Serial.print(btnVal, BIN);
    Serial.print(" = ");
    Serial.println(btnVal);

    switch (btnVal)
    {
    case ROLL_LESS: //roll--
      incrementTrim(0, -TRIMSTEP);
      break;
    case ROLL_MORE: //roll++
      incrementTrim(0, TRIMSTEP);
      break;
    case PITCH_LESS: //pitch--
      incrementTrim(1, -TRIMSTEP);
      break;
    case PITCH_MORE: //pitch++
      incrementTrim(1, TRIMSTEP);
      break;
    case THROTTLE_LESS: //throttle--
      incrementTrim(2, -TRIMSTEP);
      break;
    case THROTTLE_MORE: //throttle++
      incrementTrim(2, TRIMSTEP);
      break;
    case YAW_LESS: //yaw--
      incrementTrim(3, -TRIMSTEP);
      break;
    case YAW_MORE: //yaw++
      incrementTrim(3, TRIMSTEP);
      break;
    case MENU_B: //Menu/back
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
    case OK: //ok

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
        /*Serial.print("menuLevel = ");
        Serial.print(menuLevel);
        Serial.print(" CurrentMenuPos = ");
        Serial.println(currentMenuPos);*/
      }
      else if (menuLevel == 1)
      {
        if (currentMenuPos == 0)
        {

          if (currentMenuModelPos == currentModel)
          {
            setModelName(currentMenuModelPos);
          }
          else
          {
            currentModel = currentMenuModelPos;
            menuModels();
          }
        }
        else if (currentMenuPos == 2)
        {
          models[currentModel].reverseTrim(currentTrimPos);
          //delay(10);
          showRevs();
        }
      }
      break;
    case MENU_LESS: //Menu--

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
    case MENU_MORE:       //Menu++
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

    case OPT2:
      saveModels();
      break;
    case OPT1:
      Serial.println(models[2].getName());
      Serial.println(models[2].getTrim(2).getAmount());

      loadModels();
      delay(100);

      Serial.println(models[2].getName());
      Serial.println(models[2].getTrim(2).getAmount());
      break;

      /*case OPT0:  //
        break;
      case OPT1:  //
        break;
      case OPT2:  //
        break;
      case OPT3:  //
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
  lcd.setCursor(0, 2);
  lcd.print(models[currentModel].getName());
}

void radioConfiguration()
{
  radio.begin(); //activer le module
  radio.setChannel(0x7E);
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
  //Serial.println("----- menuModels -----");

  //for (uint8_t i = lastElemToShow - 4; i < lastElemToShow + 1; ++i)
  //Serial.println("Loop to print models");
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
          //Serial.print("selected model: ");
          //Serial.println(models[i].getName());
        }
        else
        {
          lcd.print(models[i].getName());
          //Serial.print("No selected model: ");
          //Serial.println(models[i].getName());
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
          //Serial.print("selected model: ");
          //Serial.println(models[i].getName());
        }
        else
        {
          lcd.print(models[i].getName());
          //Serial.print("No selected model: ");
          //Serial.println(models[i].getName());
        }
      }
    }
  }

  showMenuNav(currentMenuModelPos);
}

void setModelName(uint8_t index)
{
  char *name = models[index].getName();
  uint8_t indexName = 255;
  lastPress = 0;
  int btnVal = 0;
  while (btnVal != MENU_B)
  {
    btnVal = serialInBtns.readByte();
    if (btnVal != 0 && (millis() - lastPress) > 250)
    {
      switch (btnVal)
      {
        Serial.print("OK: ");
      case OK:
        if (indexName < MODEL_NAME_LENGTH - 1)
        {
          ++indexName;
        }
        else
        {
          indexName = 0;
        }

        Serial.println(indexName);

        break;
      case MENU_MORE:
        if (name[indexName] == 90)
        {
          name[indexName] = 48;
        }
        else if (name[indexName] == 57)
        {
          name[indexName] = 65;
        }
        else
        {
          ++name[indexName];
        }
        break;
      case MENU_LESS:
        if (name[indexName] == 48)
        {
          name[indexName] = 90;
        }
        else if (name[indexName] == 65)
        {
          name[indexName] = 57;
        }
        else
        {
          --name[indexName];
        }
        break;
      }

      menuModels();
      lastPress = millis();
      Serial.println(millis());
      Serial.println(lastPress);
    }
  }
  menuModels();
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

void saveModels()
{
  Serial.println("----- EEPROM Save -----");
  uint8_t eepromIndex = sizeof(models) + 1;
  Serial.println(eepromIndex - 1);
  EEPROM.put(0, models);
  for (uint8_t i = 0; i < sizeof(models) / sizeof(models[0]); i++)
  {
    for (uint8_t j = 0; j < MODEL_NAME_LENGTH; j++)
    {
      EEPROM.put(eepromIndex, models[i].getName()[j]);
      eepromIndex++;
      Serial.print(eepromIndex);
      Serial.print(" => ");
      Serial.println(models[i].getName()[j]);
    }
    EEPROM.put(eepromIndex, '\0');
    eepromIndex++;
    Serial.print(eepromIndex);
    Serial.print(" => ");
    Serial.println("\0");
  }
  EEPROM.put(eepromIndex, currentModel);
  eepromIndex++;
}

void loadModels()
{
  EEPROM.get(0, models);
  uint8_t eepromIndex = sizeof(models) + 1;
  Serial.println("----- EEPROM Load -----");

  for (uint8_t i = 0; i < sizeof(models) / sizeof(models[0]); i++)
  {
    char name[MODEL_NAME_LENGTH + 1]; //+1 for "\0"
    for (uint8_t j = 0; j < MODEL_NAME_LENGTH + 1; j++)
    {
      EEPROM.get(eepromIndex, name[j]);
      eepromIndex++;
      Serial.print(eepromIndex);
      Serial.print(" => ");
      Serial.println(name[j]);
    }
    Serial.println("Name loaded: ");
    Serial.println(name);
    models[i].setName(name);
  }

  EEPROM.get(eepromIndex, currentModel);
}
