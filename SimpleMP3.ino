/***************************************************
 Authoer : Cedric Debetaz
 Date : 08.08.2018

 Project:
 MP3 Simple player base on
 - DFPlayer - A Mini MP3 Player For Arduino

 Version:
 00 : Initial release

 ****************************************************/

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "Pushbutton.h"

// constants won't change. They're used here to set pin numbers ------------------------
const int button1Pin = A0;     // the number of the pushbutton Playlist 1 pin
const int button2Pin = A1;     // the number of the pushbutton Playlist 2 pin
const int button3Pin = A2;     // the number of the pushbutton Playlist 3 pin
const int button4Pin = A3;     // the number of the pushbutton Playlist 4 pin
const int button5Pin = A4;     // the number of the pushbutton Playlist 5 pin
const int button6Pin = A5;     // the number of the pushbutton Playlist 6 pin
const int buttonVolDecPin = 0;      // the number of the pushbutton Volume - pin
const int buttonVolIncPin = 1;     // the number of the pushbutton Volume + pin
const int ledPin =  13;       // the number of the LED pin

//Declaration ----------------------------------
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
int ReadButtonId (void);
int Volume;            //Volume for Speaker
Pushbutton button1;
Pushbutton button2;
Pushbutton button3;
Pushbutton button4;
Pushbutton button5;
Pushbutton button6;
Pushbutton buttonVolDec;
Pushbutton buttonVolInc;

//-----------------------------------------------------
void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  button1.begin(button1Pin, PullUp, 1000, 500);
  button2.begin(button2Pin, PullUp, 1000, 500);
  button3.begin(button3Pin, PullUp, 1000, 500);
  button4.begin(button4Pin, PullUp, 1000, 500);
  button5.begin(button5Pin, PullUp, 1000, 500);
  button6.begin(button6Pin, PullUp, 1000, 500);
  buttonVolDec.begin(buttonVolDecPin, PullUp, 1000, 500);
  buttonVolInc.begin(buttonVolIncPin, PullUp, 1000, 500);
    
  Serial.println();
  Serial.println(F("Simple MP3 for my children"));
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
  myDFPlayer.volume(10);  //Set volume value (0~30).
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL); //----Set different EQ----
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);//----Set device we use SD as default----

  //----Read imformation----
  Serial.println(myDFPlayer.readState()); //read mp3 state
  Serial.println(myDFPlayer.readVolume()); //read current volume
  Serial.println(myDFPlayer.readEQ()); //read EQ setting
  Serial.println(myDFPlayer.readFileCounts()); //read all file counts in SD card
  Serial.println(myDFPlayer.readCurrentFileNumber()); //read current play file number
  Serial.println(myDFPlayer.readFileCountsInFolder(1)); //read fill counts in folder SD:/01
  Serial.println(myDFPlayer.readFileCountsInFolder(2)); //read fill counts in folder SD:/02
  Serial.println(myDFPlayer.readFileCountsInFolder(3)); //read fill counts in folder SD:/03
  Serial.println(myDFPlayer.readFileCountsInFolder(4)); //read fill counts in folder SD:/04
  Serial.println(myDFPlayer.readFileCountsInFolder(5)); //read fill counts in folder SD:/05
}

//------------------------------------------------------------
void loop()
{
  switch (ReadButtonId()){
    case 1:
      myDFPlayer.loopFolder(1); //loop all mp3 files in folder SD:/01.
      break;
    case 2:
      myDFPlayer.loopFolder(2); //loop all mp3 files in folder SD:/02.
      break;
    case 3:
      myDFPlayer.loopFolder(3); //loop all mp3 files in folder SD:/03.
      break;
    case 4:
      myDFPlayer.loopFolder(4); //loop all mp3 files in folder SD:/04.
      break;
    case 5:
      myDFPlayer.loopFolder(5); //loop all mp3 files in folder SD:/05.
      break;
    case 6:
      myDFPlayer.loopFolder(6); //loop all mp3 files in folder SD:/06.
      break;
    case 10:
      myDFPlayer.volume(15); //Volume-
      Serial.println(myDFPlayer.readVolume()); //read current volume
      break;
    case 11:
      myDFPlayer.volume(301); //Volume+
      Serial.println(myDFPlayer.readVolume()); //read current volume1
      break;
  }
  
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

//----Read if button is pressed and his id
int ReadButtonId (void){
  int sensorVal=0; 

  if (button1.update()==ButtonStateHold){
    sensorVal=1;
  }
  else if (button2.update()==ButtonStateHold){
    sensorVal=2;
  }
  else if (button3.update()==ButtonStateHold){
    sensorVal=3;
  }
  else if (button4.update()==ButtonStateHold){
    sensorVal=4;
  }
  else if (button5.update()==ButtonStateHold){
    sensorVal=5;
  }
  else if (button6.update()==ButtonStateHold){
    sensorVal=6;
  }
  else if (buttonVolDec.update()==ButtonStateHold){
    sensorVal=10;
  }
  else if (buttonVolInc.update()==ButtonStateHold){
    sensorVal=11;
  }

  //----------------------DEBUG-------------
  int inByte=0;
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte=(Serial.read());
    switch (inByte) {
      case ('1'):
        sensorVal=1;
        Serial.write("un");
        break;
      case ('2'):
        sensorVal=2;
        Serial.write("deux");
        break;
      case ('3'):
        sensorVal=3;
        Serial.write("trois");
        break;
      case ('4'):
        sensorVal=4;
        Serial.write("quatre");
        break;
      case ('5'):
        sensorVal=5;
        Serial.write("cinq");
        break;
      case ('8'):
        sensorVal=10;
        Serial.write("vol-");
        break;
      case ('9'):
        sensorVal=11;
        Serial.write("vol+");
        break;
    }
  }
  //----------------------END DEBUG------------- 
  return sensorVal;
}

//----Read status on DFplayer----
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
