/*******************************************************************************
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
#include "DSP595.h"

DSP595::DSP595(uint8_t sclk, uint8_t rclk, uint8_t ser) {
	//Copy passed values into class vars 
	hw_SCLK = sclk;
	hw_RCLK = rclk;
	hw_SER = ser;
	
	//Convert arduino pin number to asm
	hw_SCLKasm = digitalPinToBitMask(hw_SCLK);
	hw_RCLKasm = digitalPinToBitMask(hw_RCLK);
	hw_SERasm = digitalPinToBitMask(hw_SER);
	
	//Allocate ports for each pin
	hw_SCLKport = portOutputRegister(digitalPinToPort(hw_SCLK));
	hw_RCLKport = portOutputRegister(digitalPinToPort(hw_RCLK));
	hw_SERport = portOutputRegister(digitalPinToPort(hw_SER));
	
	//Set the pins to outputs
	pinMode(hw_RCLK, OUTPUT);
	pinMode(hw_SCLK, OUTPUT);
	pinMode(hw_SER, OUTPUT);
	
}

/** Display managment *********************************************************/
void DSP595::displayInt(uint32_t val) {
	/* Timing: 
		160us 4 digits		200us 5 digits		240us 6 digits
		280us 7 digits		320us 8 digits
	*/

	clrArr(); //Clear the array before beginning
	
	//if the value is too high, display OL
	if(val > 99999999) {
		dspArr[6] = 0x3F; //O
		dspArr[7] = 0x38; //L
		return;
	}
	
	//If the input is 0, show a 0
	if(val == 0) {
		dspArr[7] = fontNumeral[0];
		return;
	}
	
	int8_t arrIndex = 7; //Starting index position. To be decremented
	while(val != 0) {
		uint8_t digit = val % 10; //Current (bottom) digit
		val = val / 10; //Move the significant digits down by 1
		
		//Set the current array index to a value in the numeral font
		dspArr[arrIndex] = fontNumeral[digit];
		
		--arrIndex; //Next array position
	}
}

void DSP595::displayFloat(float val, uint8_t sigDig) {
	
}

void DSP595::displayIntArray(uint8_t *arrPtr) {
	//Clear the main array
	clrArr();
	
	//Go through all ints in array passed to function
	for(uint8_t index = 0; index < 8; index++) {
		//Safety check. if value is above 9 then blank it.
		if(arrPtr[index] > 9) {
			dspArr[index] = 0x40;
			
			continue;
		}
		
		//Normal operation
		dspArr[index] = fontNumeral[arrPtr[index]];
	}
}

void DSP595::setMask(uint8_t mask) {
	this->enableMask = mask;
}

/** Array Managment ***********************************************************/
void DSP595::clrArr() {
	for(uint8_t index = 0; index < 8; index++) {
		dspArr[index] = 0x00;
	}
}


/** Hardware Control **********************************************************/
void DSP595::setRefreshRate(uint16_t rate) {
	//Multiply rate by 8, because 1fps = 8 calls per sec
	rate = rate * 8;
	
	//Convert call rate to us, this is a simple hz to us conversion
	long conv = 1000000 / rate;
	refreshMicros = conv;
}

void DSP595::refresh() { //72 Micros! very good
	//Timing stuff
	static uint32_t refreshLast;
	this->cMicros = micros();
		
	if(this->cMicros - refreshLast > refreshMicros) {
		static uint8_t digit = 0;
		//Current mask bit check. If enableMask @ bit is 0, 0 is output
		uint8_t mask = (0x01 << digit) & enableMask;
		
		//Mask is inverted, data is the current index in the main array
		writeDat(~mask, dspArr[digit]);
		
		++digit; //Incriment digit position
		if(digit == 8) digit = 0; //Reset if it overflows	
			
		//Reset timing	
		refreshLast = this->cMicros;
	}
}

void DSP595::writeDat(uint8_t mask, uint8_t data) {
	//Shift all the bits in the mask first
	for(int8_t bitMask = 7; bitMask >= 0; bitMask--) {
		//current bit to be sent
		bool curBit = (mask >> bitMask) & 0x01;
		
		//Output that bit to the SER Pin
		if(curBit) {
			*hw_SERport |= hw_SERasm;
		}
		
		if(!curBit) {
			*hw_SERport &= ~hw_SERasm;
		}
		
		//Serial clock toggle
		*hw_SCLKport |= hw_SCLKasm;
		*hw_SCLKport &= ~hw_SCLKasm;
	}
	
	//Shift all the bits for data second
	for(int8_t bitMask = 7; bitMask >= 0; bitMask--) {
		//current bit to be sent
		bool curBit = (data >> bitMask) & 0x01;
		
		//Output that bit to the SER Pin
		if(curBit) {
			*hw_SERport |= hw_SERasm;
		}
		
		if(!curBit) {
			*hw_SERport &= ~hw_SERasm;
		}
		
		//Serial clock toggle
		*hw_SCLKport |= hw_SCLKasm;
		*hw_SCLKport &= ~hw_SCLKasm;
	}
	
	//Latch clock toggle
	*hw_RCLKport |= hw_RCLKasm;
	*hw_RCLKport &= ~hw_RCLKasm;
}
