/*
 * This is a simple example file of the DSP595 to show all (most) of its 
 * abilities and uses. 
 * ADBeta 05 Jul 2022
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

#include "DSP595.h"

#define RCLK 9
#define SCLK 8
#define SER 10
DSP595 Dsp(SCLK, RCLK, SER);

void setup() {
	Serial.begin(9600);
	
	uint8_t nums[8] = {6,9,0,0,0,4,2,0};
	
	//Set the refresh rate. arg is how often the WHOLE screen refreshes
	//Dsp.setRefreshRate(120);
	
	
	//Dsp.setMask(0b11100011);
	
	//Dsp.displayIntArray(nums);
	//Dsp.displayInt(25974176);
	
	//Dsp.displayFloat(69.420, 10);
	
	//Dsp.enableMask = 0b11001100;

}

void loop() {
	static uint32_t cMillis;
	static uint32_t testMillis;
	
	cMillis = millis();
	
	
	if(cMillis - testMillis > 1) {
		static uint32_t val = 0;
				
		Dsp.displayInt(val);
				
		val += 3;
		
		testMillis = cMillis;
	}
	
	
	//Display refresh	
	Dsp.refresh();
	
}
