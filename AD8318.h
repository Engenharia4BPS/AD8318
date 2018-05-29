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
//  http://elsenaju.info/Rechner/Ausgleichsgerade.htm
//  http://www.xuru.org/rt/PR.asp#Manually
//
//  License see https://creativecommons.org/licenses/by/4.0/

#ifndef _AD8318_H
#define _AD8318_H

class AD8318 {
  public:
	/**
	 *  Create a new instance 
	 *  
	 */
    AD8318( void );

		
	/**
	 *  convert value to real value
	 *
	 *	pX	Value to convert
	 *	pdBm	true	convert to dBm
	 *			false	convert to W
	 */
	float convert(uint16_t pX, uint8_t pdBm, uint16_t cp15, uint16_t cp30);

	/**
	 *  get the unit as string depending on the conversion mode and the measured value
	 */
	const char * getUnit(void);
	
	/**
	 *  EEPROM interface
	 *  
	 *  read a set of k-factors from EEPROM at given offset
	 */
	void readEEPROM( uint16_t pOffset );
	
	/**
	 *  PROGMEM interface
	 *  
	 *  read a set of k-factors from progmem at given offset
	 */
	void readPROGMEM( uint16_t pOffset );
	
	/**
	 *  set the poweroffset in dBm
	 */
	inline void setOffset(float pOffset) { mOffset = pOffset; };
	
	/**
	 *  get the size of a parameter set in float objects
	 */
	inline uint16_t getParmSize(void) { return 12; };
	
	float val;
    
	float mK0;
    float mK1;
    float mK2;
    float mK3;
	float mK4;
    float mK5;
    float mK6;
    float mK7;
	float mK8;
    float mK9;
    float mK10;
    float mK11;
	
	float mOffset;
	int8_t mUnit;
	
	
};


#endif
