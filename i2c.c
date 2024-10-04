#include "i2c.h"
void i2c_init(void){
		SCL_OUTPUT;
		SDA_OUTPUT;
}
void i2c_startcondition(){
	SDA_OUTPUT;
	SCL_1;
	SDA_1;
	delay_us(10);
	SDA_0;
	delay_us(20);
	SCL_0;
	delay_us(20);
}
void i2c_stopcondition(){
	SDA_OUTPUT;
	SDA_0;
	delay_us(10);
	SCL_1;
	delay_us(10);
	SDA_1;

}
void i2c_writebyte(uint8_t data){
	
	for(int i=0;i<8;i++){
		if((data & 0x80)!=0){
			SDA_1;
		}
		else SDA_0;
		data<<=1;
		delay_us(10);
		SCL_1;
		delay_us(10);
		SCL_0;
	}
}
int8_t i2c_readbyte(){
	// cau hinh SDA input
	SDA_INPUT;
	
	uint8_t data=0;
	for(int i=0;i<8;i++){
		SCL_0;
		delay_us(10);
		SCL_1;
		data=(data<<1) | SDA_VAL;
		delay_us(10);
		SCL_0;
	}
	return data;
}
uint8_t i2c_readACK() {
	// C?u hình PB10 ? ch? d? input v?i pull-up
	SDA_INPUT;
	delay_us(10);
	uint8_t ACK = SDA_VAL;
	// Chuy?n PB10 v? ch? d? output open-drain
	SDA_OUTPUT;
	SCL_1; 
	delay_us(10);
	SCL_0;
	SDA_0;
	delay_us(10);
	return ACK;
}

void i2c_sendACK(){
	SDA_OUTPUT;
	SCL_0;
	SDA_0; // keo SDA xuong thap
	delay_us(10);
	SCL_1;
	delay_us(10);
	SCL_0;
	SDA_INPUT; // Ð?t l?i SDA v? input sau khi g?i ACK
	delay_us(10);
}
void i2c_sendNACK(){
	SDA_OUTPUT;
	SCL_0;
	SDA_1; // keo SDA len 1
	delay_us(10);
	SCL_1;
	delay_us(10);
	SCL_0;
	SDA_INPUT; // Ð?t l?i SDA v? input sau khi g?i ACK
	delay_us(10);
}
// hau het van hanh o che do master write va read, slave thuong la cac cam bien co san phan cung ho tro giao tiep
uint8_t i2c_writedata(uint8_t address, char *data){
	// start ---------------
	i2c_startcondition();
	
	// send address------------
	address=address<<1; // che do ghi du lieu
	SDA_OUTPUT;
	i2c_writebyte(address);
	
	// read ACK
	uint8_t ACK=i2c_readACK();
	if(ACK){
		i2c_stopcondition();
		return 0;
	}
	else {
			while(*data){// ghi chuoi data
			i2c_writebyte(*data);
			ACK=i2c_readACK();
			if(ACK){
					i2c_stopcondition();
					return 0;
			}
			data++;
		}
	}
	// stop ---------------------
	i2c_stopcondition();
	return 1;
}
int8_t i2c_readdata(uint8_t address, char* data, int8_t count){
// return -1 if it failed to read, 1 if it readed successfully
// data from a slave is stored in name called "*data" above
// read data can be a parameter(s) or a measurement(s) from a sensor or another MCU via I2C bus
	//START -------------------
	i2c_startcondition();
	//send the address to read-------------
	address= (address<<1) +1;
	i2c_writebyte(address);
	 // Ki?m tra ACK t? slave
	uint8_t ACK=i2c_readACK();
	if(ACK) {
		i2c_stopcondition();

		return -1;	
	}

	// begin read datas from slave-------------
	for(int i=0;i<count;i++){
		*data=i2c_readbyte();
		i2c_sendACK();
		data++;
	}
	
	// STOP-----------------------------------
	i2c_stopcondition();
	return 1;
}
