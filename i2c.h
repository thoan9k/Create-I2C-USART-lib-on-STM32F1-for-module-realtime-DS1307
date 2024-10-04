#ifndef __I2C_H
#define __I2C_H
#include "stm32f10x.h"                  // Device header

#define SDA_1 GPIOB->ODR|=1<<10  //PB10 
#define SCL_1 GPIOB->ODR|=1<<11  //PB11
#define SDA_0 GPIOB->ODR&=~(1<<10) //PB10
#define SCL_0 GPIOB->ODR&=~(1<<11)  //PB11
#define SDA_VAL GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10)

#define SDA_INPUT  GPIOB->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10);GPIOB->CRH |= GPIO_CRH_CNF10_1; SDA_1
#define SDA_OUTPUT GPIOB->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10);GPIOB->CRH |= (GPIO_CRH_MODE10_1 | GPIO_CRH_MODE10_0 | GPIO_CRH_CNF10_0)
#define SCL_INPUT  GPIOB->CRH &= ~(GPIO_CRH_MODE11 | GPIO_CRH_CNF11); GPIOB->CRH |= GPIO_CRH_CNF11_1; SCL_1
#define SCL_OUTPUT GPIOB->CRH &= ~(GPIO_CRH_MODE11 | GPIO_CRH_CNF11); GPIOB->CRH |= (GPIO_CRH_MODE11_1 | GPIO_CRH_MODE11_0 | GPIO_CRH_CNF11_0)


extern void delay_us(uint32_t delay);
// i2c --------------------------------------
void i2c_init(void);
//GPIO_InitTypeDef gpio_initstructure;
void i2c_startcondition(void);
void i2c_stopcondition(void);

//    master write---------------------------------
uint8_t i2c_readACK(void);
void i2c_writebyte(uint8_t data);
uint8_t i2c_writedata(uint8_t address, char *data);

//    master read ---------------------------------
int8_t i2c_readbyte(void);
void i2c_sendACK(void);
void i2c_sendNACK(void);
int8_t i2c_readdata(uint8_t address, char *data, int8_t count);

#endif
