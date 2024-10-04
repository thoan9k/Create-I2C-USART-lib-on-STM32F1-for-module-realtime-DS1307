#include "stm32f10x.h"                  // Device header
#include "ds1307.h"
#include "usart.h"

uint8_t hour;
uint8_t minute;
uint8_t second;
uint8_t dayofweek;
uint8_t date;
uint8_t month;
uint8_t year;
uint8_t IS_PM;
uint8_t MODE;
uint32_t cycle;
uint32_t timecount;

void Delay_ms(uint32_t time){
	
	while(time--){
		delay_us(1000);
	}
}

void delay_us(uint32_t delay){
	timecount = delay;
	while(timecount);
}

char *dayofweek_[] = {
		"Noday",
    "Sunday",
    "Monday",
    "Tuesday",
    "Weneday",
    "Thurday",
		"Friday",
		"Saturday"
};
void USART_displayTime(void);
int main(void){
	SysTick_Config(SystemCoreClock/1000000);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	i2c_init();
	// set up RTC for first time
//	Set_mode(24,1);// select form of time display
//	Set_time(3,9,20,7,5,10,24);
		
	
	USART_begin(9600);
	while(1){
		 RTC_read();
		 USART_displayTime();
		 Delay_ms(1000);
	}
}

void USART_displayTime(void){
	 USART_writeString(IntToStr(hour)) ; USART_writebyte(':');
	 USART_writeString(IntToStr( minute)) ; USART_writebyte(':');
	 USART_writeString(IntToStr( second)) ;  
	 if(MODE == 12){
		if(IS_PM) USART_writeString((uint8_t*)" PM");
		else USART_writeString((uint8_t*)" AM");
	 }
	 USART_writebyte(' ');// In giá tr? giây
	 USART_writeString(IntToStr(date)) ; USART_writebyte('/');
	 USART_writeString(IntToStr( month)) ; USART_writebyte('/');
	 USART_writeString((uint8_t*)"20");USART_writeString(IntToStr(year)) ;
	 USART_writebyte(' ');
	 USART_writeString((uint8_t*)dayofweek_[(dayofweek)]); 
	 USART_writebyte('\n');
}
