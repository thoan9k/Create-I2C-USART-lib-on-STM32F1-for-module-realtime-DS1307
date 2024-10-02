#include "ds1307.h"

uint8_t bcdToDec(uint8_t val){
	return ( (val/16*10) + (val%16) );
}
uint8_t DecTobcd(uint8_t dec){
	return ( (dec/10*16) + (dec%10) );
}
void RTC_write(uint8_t* data){
	i2c_startcondition();
	
	// write address
	SDA_OUTPUT;
	uint8_t address = DS1307_address << 1;
	i2c_writebyte(address);
	
	// read ACK
	uint8_t ACK=i2c_readACK();
	if(ACK){
		i2c_stopcondition();
	}
	// write word address, first pointer address
	i2c_writebyte(SECOND_register);
	ACK=i2c_readACK();
	if(ACK){
		i2c_stopcondition();
	}
	// begin transfering datas
	else {
			while(*data){// ghi chuoi data
			i2c_writebyte(*data);
			ACK=i2c_readACK();
			if(ACK){
					i2c_stopcondition();
			}
			data++;
		}
	}
	// stop ---------------------
	i2c_stopcondition();
}
void RTC_read(){
	i2c_startcondition();
	
	// write address
	SDA_OUTPUT;
	uint8_t address = (DS1307_address << 1) + 1;
	i2c_writebyte(address);
	uint8_t ACK=i2c_readACK();
	if(ACK) {
		i2c_stopcondition();
	}
	second = bcdToDec(i2c_readbyte());i2c_sendACK();
	minute = bcdToDec(i2c_readbyte());i2c_sendACK();
	hour = bcdToDec(i2c_readbyte());i2c_sendACK();
	dayofweek = bcdToDec(i2c_readbyte());i2c_sendACK();
	date = bcdToDec(i2c_readbyte());i2c_sendACK();
	month = bcdToDec(i2c_readbyte());i2c_sendACK();
	year = bcdToDec(i2c_readbyte());
	i2c_stopcondition();
}
void Set_time(uint8_t hour, uint8_t minute, uint8_t second, uint8_t dayofweek, uint8_t date, uint8_t month, uint8_t year) {
    uint8_t data[7];  // Khai báo m?ng ch?a 7 giá tr? tuong ?ng v?i th?i gian

    data[0] = DecTobcd(second);    // Giây
    data[1] = DecTobcd(minute);    // Phút
    data[2] = DecTobcd(hour);      // Gi?
    data[3] = DecTobcd(dayofweek); // Ngày trong tu?n
    data[4] = DecTobcd(date);      // Ngày
    data[5] = DecTobcd(month);     // Tháng
    data[6] = DecTobcd(year);      // Nam

    RTC_write(data);  // Truy?n m?ng vào hàm RTC_write d? ghi th?i gian
}
