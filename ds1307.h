#ifndef DS1307_H
#define DS1307_H
            
#include "i2c.h"

#define DS1307_address 				0x68   // 0x68

#define SECOND_register 			0x00
#define MINUTE_register 			0x01
#define HOUR_register 				0x02
#define DAYOFWEEK_register 		0x03
#define DATE_register 				0x04
#define MONTH_register 				0x05
#define YEAR_register 				0x06
#define CONTROL_register			0x07
#define RAM										0x08

extern uint8_t hour;
extern uint8_t minute;
extern uint8_t second;
extern uint8_t dayofweek;
extern uint8_t date;
extern uint8_t month;
extern uint8_t year;
extern uint8_t IS_PM;
extern uint8_t MODE;
//Functions
uint8_t bcdToDec(uint8_t val);
uint8_t DecTobcd(uint8_t dec);
//void DisplayTime();
void Set_mode(int mode, int IS_MODE_12_PM);// set up form 12h or 24h
	
void RTC_write(uint8_t* data,int count);
void RTC_read(void);
void Set_time(uint8_t hour,uint8_t minute,uint8_t second,uint8_t dayofweek,uint8_t date,uint8_t month,uint8_t year);
#endif
