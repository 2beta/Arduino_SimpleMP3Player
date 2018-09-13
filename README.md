# SimpleMP3Player


## Simple MP3 player for my children, with Arduino Uno and dfplayer modul.


The word "Simple" is children side not code side :). 
The goal is to have 6 collored buttons, each button start a specific play list. (2 more buttons for volume)


## Libraries:

- need 2 libraries (could be installed via Arduino IDE LIbrary Manager)

  - DFRobot, DFRobotDFplayerMini : https://github.com/DFRobot/DFRobotDFPlayerMini
  - Mokolea, InputDebounce : https://github.com/Mokolea/InputDebounce/


## Bill of materials:

- 1x Arduino  Uno
- 1x Dfplayer from Dfrobot
- 8x push buttons (momentary)
- 1x push buttons (lock)
- 1x resistance 1kOhm
- 1x Battery 18650
- 1x Battery charger
- 1x speaker 3W (4-8Ohm)
- some wires
- 1x wood box

## uSD card folder structure


- 01
  - 0001.mp3
  - 0002.mp3
  - 0003.mp3
  - ...
- 02
  - or 0001_MySong.mp3
  - or 0002_MyOtherSong.mp3 
  - ...
- 03
  - ...
- 04
- 05
- 06

