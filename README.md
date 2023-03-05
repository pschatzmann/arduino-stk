# The Synthesis ToolKit in C++ (STK) Library for Arduino

# Overview

The [Synthesis ToolKit in C++ (STK)](https://ccrma.stanford.edu/software/stk/) is a set of open source audio
signal processing and algorithmic synthesis classes written in the C++
programming language. You can find further informarmation in the original [Readme of the STK Project](/STK.md)

Like many other sound libraries it originates from an University (Princeton) and can look back at a very long history: it was created in __1995__. In the 90s the computers had limited processor power and memory available. In todays world we
can get some cheap Microcontrollers, which provide almost the same capabilities.    

This project has been implemented as Arduino Library and runs on ESP32 processors. 

## Getting Help

Here is the link to the [generated class documentation](https://pschatzmann.github.io/arduino-stk/html/index.html).

Further information can be found in [the Wiki](https://github.com/pschatzmann/arduino-stk/wiki) and in [my Blogs](https://www.pschatzmann.ch/home/tag/stk/)

## Namespaces

The library uses the namespace __stk__: This is autamatically defined if you start your sketch with 
```#import "StkAll.h"```. If you use individual imports instead, you need to set the namespace with ```using namespace stk;```


## Installation

Download the project as zip and install the file in the Arduino IDE via -> Sketch -> Include Library -> Add ZIP Library or execute the following command in the Arduino Library Folder

```
git clone https://github.com/pschatzmann/arduino-stk.git
```


## Additional Libraries

If you want to use the Midi integration please install the related [arduino-midi](https://github.com/pschatzmann/arduino-midi) library. You find additional examples in this project.

To output the sound in a consistent way I recommend to use my [Arduino Audio Tools](https://github.com/pschatzmann/arduino-audio-tools).

If you want to use the Audio Output to Bluetooth, you need to install the [ESP32-A2DP library](https://github.com/pschatzmann/ESP32-A2DP) and activate the functionality in [ArdConfig.h](src/ArdConfig.h)!


## Supported Processors

- ESP32 - The deveopment and testing has been done with the ESP32 so this will be the most stable and complete platform. 
- I managed to have the code to compile with the following processors the as well:
  - ESP8266
  - RP2040

You can consult the ArdConig.h file for the available functionality for your processor!

If you to use the library on an other Arduino platform please check out [my related Blog](https://www.pschatzmann.ch/home/2020/09/29/the-synthesis-toolkit-stk-library-for-arduino-running-on-a-nano/)
