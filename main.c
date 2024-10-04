#include "stm32f10x.h"                  // Device header
uint32_t timecount;
#include <stdio.h>
#include "ds1307.h"
#define TX_pin 2
#define RX_pin 3

// C?u hình PA2 làm chân output push-pull thông thu?ng
#define TX        GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2); GPIOA->CRL |= (GPIO_CRL_MODE2_1 | GPIO_CRL_MODE2_0)
// C?u hình PA3 làm chân input floating thông thu?ng
#define RX        GPIOA->CRL &= ~(GPIO_CRL_MODE3 | GPIO_CRL_CNF3); GPIOA->CRL |= GPIO_CRL_CNF3_0

#define writeTX(x)   GPIOA->ODR = (x) ? (GPIOA->ODR | (1<<TX_pin)) : (GPIOA->ODR & ~(1<<TX_pin))
#define readRX()     ((GPIOA->IDR & (1<<RX_pin)) ? 1 : 0)
uint8_t hour;
uint8_t minute;
uint8_t second;
uint8_t dayofweek;
uint8_t date;
uint8_t month;
uint8_t year;
void GPIO_Configuration(void);
void Delay_ms(uint16_t time){
	
	while(time--){
		delay_us(1000);
	}
}

uint32_t cycle=0;
void USART_begin(uint32_t baud);
void USART_writeString(uint8_t *str);
void USART_writebyte(uint8_t onebyte);

uint8_t USART_readdata(void);
uint8_t USART_readString(uint8_t* str);
void resetdata(uint8_t *str){
	while(*str){
	*str='\0';
	str++;
	}
}
void delay_us(uint32_t delay){
	timecount = delay;
	while(timecount);
}
uint8_t* IntToStr(int num) {
    static char buffer[10];  // S? d?ng bi?n static d? tránh b? h?y sau khi thoát kh?i hàm
    sprintf(buffer, "%d", num);  // Chuy?n s? nguyên num thành chu?i và luu vào buffer
    return (uint8_t*)buffer;     // Tr? v? con tr? d?n chu?i
	
}
char *dayofweek_[] = {
		"Noday",
    "Sunday",
    "Monday",
    "Tuesday",
    "Weneday",
    "Thurday",
		"Friday",
		"Saturday",
};
int main(void){
	// tao xung cho ham delay
	
	SysTick_Config(SystemCoreClock/1000000);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	i2c_init();
	//GPIO_Configuration();
	// Set up the clock
		uint8_t k[1];
		k[0] = 0x00;
		RTC_write(k,1);
		Set_mode24h();
		Set_time(23,59,55,7,31,12,24);
	
	
	USART_begin(9600);

	
//	RTC_read();
//	USART_writeString( IntToStr(second));
//	Delay_ms(1000);
//	RTC_read();
//	USART_writeString( IntToStr(second));
	while(1){
//				uint8_t data[2];
//				data[0]=1<<7;
//				data[1] = 10;
//				RTC_write(data);// b?t RTC// b?t RTC
		
		
			 RTC_read();
			 hour = ((hour >> 4) &0x03) * 10 + (hour & 0x0f);
			 USART_writeString(IntToStr(hour)) ; USART_writebyte(':');
			 USART_writeString(IntToStr(bcdToDec( minute))) ; USART_writebyte(':');
       USART_writeString(IntToStr(bcdToDec( second))) ;  USART_writebyte(' ');// In giá tr? giây
			 USART_writeString((uint8_t*)dayofweek_[(dayofweek)]);  USART_writebyte('-');
			 USART_writeString(IntToStr(bcdToDec( date))) ; USART_writebyte('/');
			 USART_writeString(IntToStr(bcdToDec( month))) ; USART_writebyte('/');
			 USART_writeString((uint8_t*)"20");USART_writeString(IntToStr(bcdToDec( year))) ;
       USART_writebyte('\n');
       Delay_ms(1000);
		//GPIOB->ODR^= GPIO_Pin_11;
	}
	
}
void USART_begin(uint32_t baud){
	cycle=1.0/baud*1000*1000; //us
	TX;
	RX;
}
void USART_writebyte(uint8_t onebyte) {
    // Send start bit
    writeTX(0);
    delay_us(cycle);

    // Send 1 byte bit by bit
    for (int i = 0; i < 8; i++) {
        int s = onebyte & 1; // Send the least significant bit first
        onebyte >>= 1;
        writeTX(s);
        delay_us(cycle);
    }

    // Send stop bit
    writeTX(1);
    delay_us(cycle);
}

void USART_writeString(uint8_t *str) {
    while (*str) {
        USART_writebyte(*str);
        str++;
    }
}

uint8_t USART_readbyte() {
    uint8_t data = 0;
    // Read START bit
    while (readRX() == 1);
    delay_us(cycle);

    // Read 1 byte bit by bit
    for (int i = 0; i < 8; i++) {
        delay_us(cycle / 2);
        data |= (readRX() << i);
        delay_us(cycle / 2);
    }

    // Read STOP bit
    if (readRX() == 1) { // Check if data is received correctly
        return data;
    } else {
        return 0;
    }
}
