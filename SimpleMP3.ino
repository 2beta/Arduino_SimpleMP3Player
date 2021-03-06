/***************************************************
 Author : Cedric Debetaz
 Date : 08.08.2018

 Project:
 MP3 Simple player base on
 - DFPlayer : https://github.com/DFRobot/DFRobotDFPlayerMini
 - debounce : https://github.com/Mokolea/InputDebounce/

 Version:
 00 : 2018.08.08 - Initial release
 01 : 2018.09.05 - Change debounce library
 02 : 2018.09.09 - play next song if already in the folder

 ****************************************************/

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "InputDebounce.h"

#define BUTTON_DEBOUNCE_DELAY   100   // [ms]

// Declaration ------------------------------------------------------
void printDetail(uint8_t type, int value);  // Print some details of DFplayer module
void SerialPrint (char str1[], int value);
void ToggleLed();

SoftwareSerial mySoftwareSerial(10, 11);    // RX, TX, for communication with DFPLayer
DFRobotDFPlayerMini myDFPlayer;             // DFplayer

int volume;                                 // Volume for Speaker
int folder;                                 // Actual folder
unsigned long now;                          // Actual time
int dfStatus;                               // Status of the dfplayer

const int ledPin = LED_BUILTIN;             // the number of the LED pin // 13;
static const int pinSwitch1 = 2;
static const int pinSwitch2 = 3;
static const int pinSwitch3 = 4;
static const int pinSwitch4 = 5;
static const int pinSwitch5 = 6;
static const int pinSwitch6 = 7;
static const int pinSwitch7 = 8;            // Volume-
static const int pinSwitch8 = 9;            // Volume+

static InputDebounce buttonTest1;           // Not enabled yet, setup has to be called first, see setup() below
static InputDebounce buttonTest2;
static InputDebounce buttonTest3;
static InputDebounce buttonTest4;
static InputDebounce buttonTest5;
static InputDebounce buttonTest6;
static InputDebounce buttonTest7;
static InputDebounce buttonTest8;


//-----------------------------------------------------
void setup()
{
  // initialize digital pin as an output
  pinMode(ledPin, OUTPUT);
  
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  // register callback functions (shared, used by all buttons)
  buttonTest1.registerCallbacks(buttonTest_pressedCallback, NULL, NULL, NULL);
  buttonTest2.registerCallbacks(buttonTest_pressedCallback, NULL, NULL, NULL);
  buttonTest3.registerCallbacks(buttonTest_pressedCallback, NULL, NULL, NULL);
  buttonTest4.registerCallbacks(buttonTest_pressedCallback, NULL, NULL, NULL);
  buttonTest5.registerCallbacks(buttonTest_pressedCallback, NULL, NULL, NULL);
  buttonTest6.registerCallbacks(buttonTest_pressedCallback, NULL, NULL, NULL);
  buttonTest7.registerCallbacks(buttonTest_pressedCallback, NULL, NULL, NULL);
  buttonTest8.registerCallbacks(buttonTest_pressedCallback, NULL, NULL, NULL);  
  
  // setup input buttons (debounced)
  buttonTest1.setup(pinSwitch1, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
  buttonTest2.setup(pinSwitch2, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
  buttonTest3.setup(pinSwitch3, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
  buttonTest4.setup(pinSwitch4, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
  buttonTest5.setup(pinSwitch5, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
  buttonTest6.setup(pinSwitch6, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
  buttonTest7.setup(pinSwitch7, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
  buttonTest8.setup(pinSwitch8, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
    
  Serial.println();
  Serial.println(F("Simple MP3 for my children"));
  Serial.println("**************************");
  Serial.println();
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));

  //----Set paramters----
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  myDFPlayer.volume(15);  //Set volume value (0~30).
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL); //----Set different EQ----
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);//----Set device we use SD as default----

  //----Read imformation----
  Serial.println ("---------------- DFplayer Status -------------------");
  SerialPrint("state = ", myDFPlayer.readState()); //read mp3 state
  SerialPrint("Volume = ",myDFPlayer.readVolume()); //read current volume
  SerialPrint("EQ = ",myDFPlayer.readEQ()); //read EQ setting
  SerialPrint("File counts = ",myDFPlayer.readFileCounts()); //read all file counts in SD card
  SerialPrint("Current play file number =",myDFPlayer.readCurrentFileNumber()); //read current play file number
  SerialPrint("file counts in folder SD:/01 = ",myDFPlayer.readFileCountsInFolder(1)); //read file counts in folder SD:/01
  SerialPrint("file counts in folder SD:/02 = ",myDFPlayer.readFileCountsInFolder(2)); //read file counts in folder SD:/02
  SerialPrint("file counts in folder SD:/03 = ",myDFPlayer.readFileCountsInFolder(3)); //read file counts in folder SD:/03
  SerialPrint("file counts in folder SD:/04 = ",myDFPlayer.readFileCountsInFolder(4)); //read file counts in folder SD:/04
  SerialPrint("file counts in folder SD:/05 = ",myDFPlayer.readFileCountsInFolder(5)); //read file counts in folder SD:/05
  SerialPrint("file counts in folder SD:/06 = ",myDFPlayer.readFileCountsInFolder(6)); //read file counts in folder SD:/06
  Serial.println ("---------------------------------------------------");

  folder = 0;
  
}

//------------------------------------------------------------
void loop()
{
  now = millis();

  // Led for status
  dfStatus = myDFPlayer.readState();
  //SerialPrint("Dfstatus = ",dfStatus);
  if(dfStatus==512){//state wait
    digitalWrite(ledPin, Toggle());
  }
  else if(dfStatus==-1){//state error
    digitalWrite(ledPin, Toggle());
  }
  else if(dfStatus==513){//read mp3
    digitalWrite(ledPin, true);
  }
  
  // poll button state
  buttonTest1.process(now); // callbacks called in context of this function
  buttonTest2.process(now);
  buttonTest3.process(now);
  buttonTest4.process(now);
  buttonTest5.process(now);
  buttonTest6.process(now);
  buttonTest7.process(now);
  buttonTest8.process(now);
  
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }


}


//---------------------------------------------------
void SerialPrint (char Str1[], int value)
{
   Serial.print(Str1);
   Serial.println(value);
}

//-----------------------------------------------------
bool Toggle (void){
  static unsigned long   ulPreviousMillis = 0;        // will store last time
  const long lInterval = 250;           // interval at wich to stay in config mode (milliseconds)
  static bool bToggle;

  if (millis() - ulPreviousMillis >= lInterval){
    ulPreviousMillis=millis();
    bToggle=!bToggle;
  }
  return bToggle;
}


//---------------------------------------------------
void buttonTest_pressedCallback(uint8_t pinIn)
{
  switch (pinIn){
      case 2: // Folder 1
        if (folder==1){ // next song
          myDFPlayer.next(); //next song.
          SerialPrint ("Next in folder 1, pin= ",pinIn);
          SerialPrint ("File read = ", myDFPlayer.readCurrentFileNumber());
        }
        else { // play folder 1
          myDFPlayer.loopFolder(1); //loop all mp3 files in folder SD:/01.
          SerialPrint ("Play folder 1, pin= ",pinIn);
        }
        folder=1;
        break;
      case 3: // Folder 2
        if (folder==2){ // next song
          myDFPlayer.next(); //next song.
          SerialPrint ("Next in folder 2, pin= ",pinIn);
          SerialPrint ("File read = ", myDFPlayer.readCurrentFileNumber());
        }
        else { // play folder 2
          myDFPlayer.loopFolder(2); //loop all mp3 files in folder SD:/02.
          SerialPrint ("Play folder 2, pin= ",pinIn);
        }
        folder=2;
        break;
      case 4: // Folder 3
        if (folder==3){ // next song
          myDFPlayer.next(); //next song.
          SerialPrint ("Next in folder 3, pin= ",pinIn);
          SerialPrint ("File read = ", myDFPlayer.readCurrentFileNumber());
        }
        else { // play folder 3
          myDFPlayer.loopFolder(3); //loop all mp3 files in folder SD:/03.
          SerialPrint ("Play folder 3, pin= ",pinIn);
        }
        folder=3;
        break;
      case 5: // Folder 4
        if (folder==4){ // next song
          myDFPlayer.next(); //next song.
          SerialPrint ("Next in folder 4, pin= ",pinIn);
          SerialPrint ("File read = ", myDFPlayer.readCurrentFileNumber());
        }
        else { // play folder 4
          myDFPlayer.loopFolder(4); //loop all mp3 files in folder SD:/04.
          SerialPrint ("Play folder 4, pin= ",pinIn);
        }
        folder=4;
        break;
      case 6: // Folder 5
        if (folder==5){ // next song
          myDFPlayer.next(); //next song.
          SerialPrint ("Next in folder 5, pin= ",pinIn);
          SerialPrint ("File read = ", myDFPlayer.readCurrentFileNumber());
        }
        else { // play folder 5
          myDFPlayer.loopFolder(5); //loop all mp3 files in folder SD:/05.
          SerialPrint ("Play folder 5, pin= ",pinIn);
        }
        folder=5;
        break;
      case 7: // Folder 6
        if (folder==6){ // next song
          myDFPlayer.next(); //next song.
          SerialPrint ("Next in folder 6, pin= ",pinIn);
          SerialPrint ("File read = ", myDFPlayer.readCurrentFileNumber());
        }
        else { // play folder 6
          myDFPlayer.loopFolder(6); //loop all mp3 files in folder SD:/06.
          SerialPrint ("Play folder 6, pin= ",pinIn);
        }
        folder=6;
        break;
      case 8:
        volume=myDFPlayer.readVolume();
        if (volume>=3){
          volume-=3;
          myDFPlayer.volume(volume);
        }
        SerialPrint ("Volume-, volume = ",volume);
        break;
      case 9:
        volume=myDFPlayer.readVolume();
        if (volume<=27){
          volume+=3;
          myDFPlayer.volume(volume);
        }
        SerialPrint ("Volume+, volume = ",volume);
        break;
    }
}




//----Read status on DFplayer-------------------------------
void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
