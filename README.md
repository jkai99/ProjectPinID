# ProjectPinID
INF2004 Embedded Systems ProjectPinID

Done by: 
2202574 Ng Jing Kai
2201930 Eugene Tan Jun Yu
2201940 Brandon Loo Jia Le
2201594 Danzel Lim
2201428 Eugene Tan Le Xuan

Description of Project Pin Finder Pico 

Project Context
Pins on the Raspberry Pi Pico has been scratched and only VCC, GND, VBUS are visible. The project team set out to aid people in identifying the pico pins by doing a series of test to determine the communication protocol to allow the pico to be of use instead of replacing the pico.
Project Objectives
Our project proposal outlines a set of clear objectives:

Development of a pico tool for pin identification using pico_w and pico maker board: 
The tool when used in conjunction with the pico board allows toggling of 3 different communication protocols to detect a transmit or receive signal from the scratched pico.

Example use cases
1. Scratched Pins UART testing
When the Pico Tool is running receive signal code, it should be tested with the scratched pico to test for any transmit of UART signals.


2. Scratched Pins I2C  testing
Testing of finding CLK signals allows identification of I2C protocol as pins are initialized to be high in pairs. 

After identification of CLK, doing a bus scan would determine if I2C is running as slave addresses are stored in memory.

3. Scratched Pins SPI testing
After process of elimination from single pin that is high, if it fails the UART test, it is deemed as SPI C/S as it is initialized as high as well. 

Afterwhich, with the determination of CLK signals, testing of 4 pins with the CLK signal and C/S would allow testing of SPI protocol. 

Pin Identification:
Testing for Pin identifications. The pins should correspond to certain testing to affirm the tester that the pin could be used for a multitude of purpose such as (I2C, UART, SPI)

Block Diagram:
