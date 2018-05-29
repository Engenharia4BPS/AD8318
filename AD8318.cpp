//=================================================================
//
//  Simple library to access the AD8318 ADC with an Arduino. 
//
//	See https://github.com/ik2biy/AD8318
//
//	Version 2.0
//
//  2018 IK2BIY
//  Based on DL2SBA project
//
//  Credit : RF powermwtwr Dietmar Krause, DL2SBA https://github.com/dl2sba/AD8318
//
//  License see https://creativecommons.org/licenses/by/4.0/
//
#include <arduino.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>
#include "AD8318.h"


AD8318::AD8318 ( void ) {
	mK0 = 2.804742194E1;
	mK1 = -3.432439417E-2;
	mK2 = 5.570579072E-6;
	mK3 = -1.015882955E-9;
	mK4 = 2.804742194E1;
	mK5 = -3.432439417E-2;
	mK6 = 5.570579072E-6;
	mK7 = -1.015882955E-9;
	mK8 = 2.804742194E1;
	mK9 = -3.432439417E-2;
	mK10 = 5.570579072E-6;
	mK11 = -1.015882955E-9;
	mOffset = 0;
}

/**
 *  
 */
void AD8318::readPROGMEM( uint16_t pK0 ) {
	mK0 = pgm_read_float_near(pK0 + 0 * sizeof mK0);
	mK1 = pgm_read_float_near(pK0 + 1 * sizeof mK0);
	mK2 = pgm_read_float_near(pK0 + 2 * sizeof mK0);
	mK3 = pgm_read_float_near(pK0 + 3 * sizeof mK0);
	//
	mK4 = pgm_read_float_near(pK0 + 4 * sizeof mK0);
	mK5 = pgm_read_float_near(pK0 + 5 * sizeof mK0);
	mK6 = pgm_read_float_near(pK0 + 6 * sizeof mK0);
	mK7 = pgm_read_float_near(pK0 + 7 * sizeof mK0);
	//
	mK8 = pgm_read_float_near(pK0 + 8 * sizeof mK0);
	mK9 = pgm_read_float_near(pK0 + 9 * sizeof mK0);
	mK10 = pgm_read_float_near(pK0 + 10 * sizeof mK0);
	mK11 = pgm_read_float_near(pK0 + 11 * sizeof mK0);
}

/**
 *  
 */
void AD8318::readEEPROM( uint16_t pEEPROMOffset ) {
	EEPROM.get(pEEPROMOffset + 0 * sizeof mK0, mK0);
	EEPROM.get(pEEPROMOffset + 1 * sizeof mK0, mK1);
	EEPROM.get(pEEPROMOffset + 2 * sizeof mK0, mK2);
	EEPROM.get(pEEPROMOffset + 3 * sizeof mK0, mK3);
	//
	EEPROM.get(pEEPROMOffset + 4 * sizeof mK0, mK4);
	EEPROM.get(pEEPROMOffset + 5 * sizeof mK0, mK5);
	EEPROM.get(pEEPROMOffset + 6 * sizeof mK0, mK6);
	EEPROM.get(pEEPROMOffset + 7 * sizeof mK0, mK7);
	//
	EEPROM.get(pEEPROMOffset + 8 * sizeof mK0, mK8);
	EEPROM.get(pEEPROMOffset + 9 * sizeof mK0, mK9);
	EEPROM.get(pEEPROMOffset + 10 * sizeof mK0, mK10);
	EEPROM.get(pEEPROMOffset + 11 * sizeof mK0, mK11);
}

/**
 *  convert value to real value
 */
float AD8318::convert(uint16_t pX, uint8_t pdBm, uint16_t cp15, uint16_t cp30) {
	
	//float val = (mK0 + mK1 * pX + mK2 * pX * pX + mK3 * pX * pX * pX) - mOffset;
	
	if (pX < cp15) {
		
	    val = (mK0 + mK1 * pX + mK2 * pX * pX + mK3 * pX * pX * pX) - mOffset;
    
	}else if (pX < cp30) {
		
        val = (mK4 + mK5 * pX + mK6 * pX * pX + mK7 * pX * pX * pX) - mOffset;
    
	}else {
		
        val = (mK8 + mK9 * pX + mK10 * pX * pX + mK11 * pX * pX * pX) - mOffset;
    }
		
		//	 overload check
		if (val > (1 - mOffset)){
		val = 8888; // Overload
		}
	
		// we are in dBm mode ?
		 
		if (val == 8888) {
			
			//	overload
			mUnit = 8;
		} else if ( pdBm ) {
			
			//yes
			mUnit = -1;
		} else {
			val =  pow(10.0, val / 10.0);
		
			//	watt mode
			if (val < 0.000001 ) {
				//	nW
				mUnit = 4;
				val *= 1e9;
			} else if (val < 0.001 ) {
				//	nW
				mUnit = 3;
				val *= 1e6;
			} else if (val < 1.0) {
				//	uW
				mUnit = 2;
				val *= 1e3;
			} else if (val < 1000.0 ) {
				//	mW
				mUnit = 1;
			} else {
				//	W
				mUnit = 0;
				val /= 1e3;
			}
			
		} 
		
	return val;
}		

/**
 *  get the unit as string depending on the conversion mode and the measured value
 */
const char * AD8318::getUnit(void) {
	static char unit[4];
	if ( mUnit == -1 ) {
		unit[0] = 'd';
		unit[1] = 'B';
		unit[2] = 'm';
	} else if( mUnit == 8 ) {
		unit[0] = ' ';
		unit[1] = ' ';
		unit[2] = ' ';
	} else {
		unit[1] = 'W';
		unit[2] = 0;
		if ( mUnit == 0 ) {
			unit[0] = ' ';
		} else  if ( mUnit == 1 ) {
			unit[0] = 'm';
		} else  if ( mUnit == 2 ) {
			unit[0] = 'µ';
		} else  if ( mUnit == 3 ) {
			unit[0] = 'n';
		} else  if ( mUnit == 4 ) {
			unit[0] = 'p';
		} else {
			unit[0] = '?';
		}
	}
    unit[3] = 0;

	return unit;
}
