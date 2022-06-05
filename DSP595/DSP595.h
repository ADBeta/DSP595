/* 
 * This is a re-implimentation of my 74HC595 IC driver library, but stripped
 * down to only include the 16 bit transmission function, but with a bunch more
 * helpful display functions. 
 *
 * This display is a bit janky because it needs constant updating, but it is 
 * cheap and effective, so here it is.
 *
 * Version 0.6
 * ADBeta 27 May 2022
********************************************************************************
 This file is part of DSP595.
 DSP595 is free software: you can redistribute it and/or modify it under the 
 terms of the GNU General Public License as published by the 
 Free Software Foundation, either version 3 of the License, or (at your option)
 any later version.
 
 DSP595 is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 for more details.

 You should have received a copy of the 
 GNU General Public License along with DSP595 
*/

#include "Arduino.h"

#ifndef DSP595_H
#define DSP595_H

class DSP595 {
	public:
	//Initialize display pins and blanks the display
	DSP595(uint8_t, uint8_t, uint8_t);
	
	//Display data array. 
	uint8_t dspArr[8];
	
	/** Fonts and charsets ****************************************************/
	uint8_t fontNumeral[10] = {
		0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
	}; // 0 1 2 3 4 5 6 7 8 9
	
	/** High level display functions ******************************************/
	//Displays any intager up to 99999999. Right aligned
	void displayInt(uint32_t);
	//Display a foating point number, optinal selectable significant digits
	void displayFloat(float, uint8_t = 0);
	//Converts an array of numbers to int font data, copies it to main array
	void displayIntArray(uint8_t *);
	//Display a custom data array.
	void displayCustomData(uint8_t *);
	//TODO displayFloat		displayCustom
	
	/** Lower level display functions *****************************************/
	//Refresh the display
	void refresh();
	//Set the enable/disable mask
	void setMask(uint8_t);
	//Set the refresh rate of the display, frame rate is measured by the whole
	//screen updating fully. eg 60 = 60 * 8 timing calls
	void setRefreshRate(uint16_t);
	
	private:
	/** Hardware Pins and registers *******************************************/
	int8_t hw_SCLK, hw_RCLK, hw_SER; //Arduino pin
	uint8_t hw_SCLKasm, hw_RCLKasm, hw_SERasm; //Assembly pin
	volatile uint8_t *hw_SCLKport, *hw_RCLKport, *hw_SERport; //Assembly port
	
	/** Hardware controlling variables ****************************************/
	//Enable mask used to blank certain digits.
	uint8_t enableMask = 0xFF;
	//class global timing micros
	uint32_t cMicros;
	//The amount of us between each update. Set by setRefreshRate()
	uint16_t refreshMicros = 1041; //Default is 120fps
	
	//Writes the data we want to the displays. uses two 8 bit vars for speed
	void writeDat(uint8_t, uint8_t);
	
	/** Array Managment Functions *********************************************/
	void clrArr();
	void cpyArr(uint8_t *, uint8_t *);

}; //class 595DSP

#endif
