# The Synthesis ToolKit in C++ (STK) Library for Arduino

# Overview

The [Synthesis ToolKit in C++ (STK)](https://ccrma.stanford.edu/software/stk/) is a set of open source audio
signal processing and algorithmic synthesis classes written in the C++
programming language. You can find further informarmation in the original [Readme of the STK Project](/STK.md)

Like many other sound libraries it originates from an University (Princeton) and can look back at a very long history: it was created in __1995__. In the 90s the computers had limited processor power and memory available. In todays world we
can get some cheap Microcontrollers, which provide almost the same capabilities.    

This project has been implemented as Arduino Library and runs on ESP32 processors. 

## Further information
Further information can be found in my Blogs
- [Overview](https://www.pschatzmann.ch/home/2020/09/24/the-synthesis-toolkit-skt-library-for-the-arduino-esp32/)
- [Sound without Files](https://www.pschatzmann.ch/home/2020/09/26/the-synthesis-toolkit-stk-w-o-files/)
- [Using Midi](https://www.pschatzmann.ch/home/2020/09/28/the-synthesis-toolkit-skt-library-for-the-arduino-esp32-midi/) 

## Installation
Download the project as zip and install the file in the Arduino IDE via -> Sketch -> Include Library -> Add ZIP Library or execute the following command in the Arduino Library Folder

```
git clone https://github.com/pschatzmann/Arduino-STK.git
```

## Supported Processors

- ESP32 - The deveopment and testing has been done with the ESP32 so this will be the most stable and complete platform. 
- ESP8266 - I managed to have the code base compiling with the ESP8266 as well. This environment does not support Bluetooth. 

You can consult the ArdConig.h file for the available functionality for your processor!

Standard Arduinos are not supported yet because the STK framwork is using the following (missing) classes which are not available
in the Standard Arduino implementation:

- std::vector
- std::string
- std::ostringstream
- std::cout
- std::queue

I tried to compile the code with Mike Materas STL Library, but I unfortunatly there are some [open issues](https://github.com/arduino/ArduinoCore-avr/issues/287)


