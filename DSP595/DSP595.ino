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
	//Dsp.setMask(0b11100011);
	
	Dsp.displayIntArray(nums);
	//Dsp.showInt(69420);
	
	//Dsp.enableMask = 0b11001100;

}

void loop() {
	static uint32_t cMicros, cMillis;
	static uint32_t refreshMicros;
	static uint32_t testMillis;
	
	
	cMicros = micros();
	cMillis = millis();
	
	/*
	if(cMillis - testMillis > 100) {
		static uint32_t val = 0;
		
		static uint32_t eMicros, sMicros;
		
		sMicros = micros();
		
		Dsp.displayInt(val);
		
		eMicros = micros();
		
		Serial.println(eMicros - sMicros);
		
		
		
		val += 999;
		
		testMillis = cMillis;
	}
	*/
	
	//Display refresh
	if(cMicros - refreshMicros > 1000) {		
		Dsp.refresh();
		
		refreshMicros = cMicros;
	}
	
}
