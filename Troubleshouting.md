# How to program ESP8266 Module in Arduino IDE ? 

## IDE Config

For development you need:
- [Arduino IDE](https://www.arduino.cc/en/Main/Software) If on windows install it manually not from windows store
- Visual Studio code with Arduino extension, should work out of the box

## Install support for esp8266 in Arduino

Open the board manager in settings
Search for **esp8266** by ESP8266 and install it.

See https://github.com/esp8266/Arduino for more details

## Select Node MCU 1.0 in board manager

- Select : Node MCU 1.0

## Compile and use a particular file

See the settings of VSCode extension in .vscode/arduino.json, you can only have one .ino file set.
