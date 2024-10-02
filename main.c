#include "stm32f10x.h"                  // Device header
uint32_t timecount;

#include "ds1307.h"
uint8_t hour;
uint8_t minute;
uint8_t second;
uint8_t dayofweek;
uint8_t date;
uint8_t month;
uint8_t year;
void delay_us(uint32_t delay){
	timecount = delay;
	while(timecount);
}
int main(void){
	// tao xung cho ham delay
	SysTick_Config(SystemCoreClock/1000000);
	while(1){
		
	}
	
}
