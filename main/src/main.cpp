/**
 * TODO list:
 * maj selection model3
 * add a display object
 * menuModel check the for loop and if
 * 
 * button show 74hc165 X2
 * (74hc595)
 **/

#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <printf.h>
#include <Model.h>
#include <Trim.h>
//#include <EEPROM.h>
#include <LiquidCrystal.h>

#define TRIMSTEP 2
#define BTNDELAY 3000

RF24 radio(9, 10); //CE, CSN

const uint64_t txAddr = 0xABCDABCD71;
const uint64_t rxAddr = 0x544d52687C;

const uint8_t gimballAddr[4] = {A1, A0, A2, A3}; //roll, pitch, throttle, yaw
const uint8_t btnValue = A4;
//                           roll -         +         pitch -       +         yaw -         +         thr -         +            OK       menu/back     menu -      menu +
const int btnArray[12][2] = {{835, 845}, {850, 863}, {865, 870}, {877, 887}, {890, 903}, {906, 925}, {917, 934}, {936, 948}, {952, 963}, {967, 983}, {985, 1000}, {1003, 1025}};
Model models[6] = {Model((char *)"Model-0"), Model((char *)"Model-1"), Model((char *)"Model-2"), Model((char *)"Model-3"), Model((char *)"Model-4"), Model((char *)"Model-5")};
uint8_t currentMenuModelPos = 0;
uint8_t currentModel = 0;
uint8_t currentTrimPos = 0;
uint8_t menuLCDpos = 0;
int timeAtLastBtnPressed = 0;
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
void showInfosScreen();
void radioConfiguration();
void radioConfiguration();
void showMenuNav(uint8_t pos);
void universalMenuNave(bool way, uint8_t menuLength, uint8_t & pos); //& pos --> pass a reference
void menu();
void menuModels();
void updateCurrentModel(uint8_t index);
void showTrims();
void showRevs();
void updateTrimVal(int trimID);

void setup()
{
  lcd.begin(20, 4);
  Serial.begin(9600);

  //models[currentModel].setName((char *)"Model de test");

  pinMode(btnValue, INPUT);

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

  showInfosScreen();
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

  int val = analogRead(btnValue);
  if (val > 200)
  {
    Serial.print("Valeur boutton analogue = ");
    Serial.println(String(val));
  }

  if (val > 200 && press)
  {

    Serial.print("millis() - (long)timeAtLastBtnPressed) = ");
    Serial.print(millis() - (long)timeAtLastBtnPressed);
    Serial.print("\t BTNDELAY = ");
    Serial.println(BTNDELAY);

    uint8_t indexPressedBtn = -1;
    for (uint8_t i = 0; i < 12; i++)
    { //watch all btns
      if (val >= btnArray[i][0] && val <= btnArray[i][1])
      {                      //check which button is pressed
        indexPressedBtn = i; //set button pressed indexPressedBtn
      }
      //Serial.println(" --> " + String(indexPressedBtn));
      switch (indexPressedBtn)
      {       //button action list
      case 0: //Trim roll-
        models[currentModel].incTrim(0, -TRIMSTEP);
        if (menuLevel == 1 && currentMenuPos == 1)
        {
          updateTrimVal(0);
        }
        break;
      case 1: //Trim roll+
        models[currentModel].incTrim(0, TRIMSTEP);
        if (menuLevel == 1 && currentMenuPos == 1)
        {
          updateTrimVal(0);
        }
        //showTrim(0, trims[0]);
        break;
      case 2: //Trim pitch -
        models[currentModel].incTrim(1, -TRIMSTEP);
        if (menuLevel == 1 && currentMenuPos == 1)
        {
          updateTrimVal(1);
        }
        //showTrim(1, trims[1]);
        break;
      case 3: //Trim pitch +
        models[currentModel].incTrim(1, TRIMSTEP);
        if (menuLevel == 1 && currentMenuPos == 1)
        {
          updateTrimVal(1);
        }
        //showTrim(1, trims[1]);
        break;
      case 4: //Trim throtle -
        models[currentModel].incTrim(2, -TRIMSTEP);
        if (menuLevel == 1 && currentMenuPos == 1)
        {
          updateTrimVal(2);
        }
        //showTrim(2, trims[2]);
        break;
      case 5: //Trim throtle +
        models[currentModel].incTrim(2, TRIMSTEP);
        if (menuLevel == 1 && currentMenuPos == 1)
        {
          updateTrimVal(2);
        }
        break;
      case 6: //Trim yaw -
        models[currentModel].incTrim(3, -TRIMSTEP);
        if (menuLevel == 1 && currentMenuPos == 1)
        {
          updateTrimVal(3);
        }
        //showTrim(3, trims[3]);
        break;
      case 7: //Trim yaw +
        models[currentModel].incTrim(3, TRIMSTEP);
        if (menuLevel == 1 && currentMenuPos == 1)
        {
          updateTrimVal(3);
        }
        //showTrim(3, trims[3]);
        break;
      case 8:
        //menu ok
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
            delay(10);
            showRevs();

            //screen.setPrintPos(70, 20 + currentTrimPos*(//screen.getWidth()-40)/models[currentModel].getTrimLength());
            /*if (models[currentModel].getTrim(currentTrimPos).isRev())
              {
                //screen.setColor(red.r, red.g, red.b);
                //screen.print("Reversed");
              }
              else
              {
                //screen.setColor(wblue.r, wblue.g, wblue.b);
                //screen.print("Normal    ");
              }*/
          }
          //showMenuNav(currentTrimPos, (//screen.getWidth()-30)/models[currentModel].getTrimLength());
        }

        break;
      case 9:
        //menu/back
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
          showInfosScreen();
        }
        else
        {
          menu();
        }

        break;
      case 10:
        //menu-
        Serial.println("Menu -");
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
      case 11:
        //menu+
        Serial.println("Menu +");
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
      }
    }
    timeAtLastBtnPressed = millis();
    press = false;
  }
  else
  {
    press = true;
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

void showInfosScreen()
{
  lcd.clear();
  lcd.print("Cob's radio");
  lcd.setCursor(0, 1);
  lcd.print(models[currentModel].getName());
  lcd.setCursor(0, 2);
  lcd.print("bla bla");
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

void universalMenuNave(bool way, uint8_t menuLength, uint8_t & pos) //manage pointer/addr update
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
  for (uint8_t i = 0; i < menuElemLength; i++)
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

  //for (uint8_t i = lastElemToShow - 4; i < lastElemToShow + 1; i++)
  Serial.println("Loop to print models");
  int j = 0;
  if (currentMenuModelPos > 3)
  {
    for (uint8_t i = currentMenuModelPos - 3; i < currentMenuModelPos + 1; i++)
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
    for (uint8_t i = 0; i < 4; i++)
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

void showTrims()
{
  lcd.clear();
  lcd.setCursor(0, 0);

  //char*trimNames[4] = {"Roll", "Pitch", "Throttle", "Yaw"};
  //uint8_t pos = 20;
  //int sizeOfanArray = *(&array + 1) - array;
  uint8_t trimsLength = models[currentModel].getTrimLength();
  for (uint8_t i = 0; i < trimsLength; i++)
  {
    lcd.setCursor(0, i);
    lcd.print(models[currentModel].getTrim(i).getName());
    lcd.setCursor(17, i);
    lcd.print(models[currentModel].getTrim(i).getAmount());

    //pos+=(screen.getWidth()-40)/trimsLength;
  }
}

void showRevs()
{
  lcd.clear();
  lcd.setCursor(0, 0);

  //char*trimNames[4] = {"Roll", "Pitch", "Throttle", "Yaw"};
  //uint8_t pos = 20;
  //int sizeOfanArray = *(&array + 1) - array;
  uint8_t trimsLength = models[currentModel].getTrimLength();
  for (uint8_t i = 0; i < trimsLength; i++)
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

void updateTrimVal(int trimID)
{
  //uint8_t pos = 20;
  uint8_t triml = models[currentModel].getTrimLength();
  for (uint8_t i = 0; i < triml; i++)
  {
    if (trimID == i)
    {
      return;
    }
    //pos+=(screen.getWidth()-30)/triml;
  }
}
