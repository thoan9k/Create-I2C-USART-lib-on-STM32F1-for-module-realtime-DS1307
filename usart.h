#ifndef _USART_H
#define _USART_H
#include "stm32f10x.h" 
#include <stdio.h>
#define TX_pin 2
#define RX_pin 3

// C?u hình PA2 làm chân output push-pull thông thu?ng
#define TX        GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2); GPIOA->CRL |= (GPIO_CRL_MODE2_1 | GPIO_CRL_MODE2_0)
// C?u hình PA3 làm chân input floating thông thu?ng
#define RX        GPIOA->CRL &= ~(GPIO_CRL_MODE3 | GPIO_CRL_CNF3); GPIOA->CRL |= GPIO_CRL_CNF3_0

#define writeTX(x)   GPIOA->ODR = (x) ? (GPIOA->ODR | (1<<TX_pin)) : (GPIOA->ODR & ~(1<<TX_pin))
#define readRX()     ((GPIOA->IDR & (1<<RX_pin)) ? 1 : 0)
extern uint32_t cycle;
extern void Delay_ms(uint32_t time);
extern void delay_us(uint32_t time);
uint8_t* IntToStr(int num);
void USART_begin(uint32_t baud);
void USART_writeString(uint8_t *str);
void USART_writebyte(uint8_t onebyte);

uint8_t USART_readdata(void);
uint8_t USART_readString(uint8_t* str);

void resetdata(uint8_t *str);
#endif
