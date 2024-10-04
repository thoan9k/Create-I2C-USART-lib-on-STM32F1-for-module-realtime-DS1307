#include "ds1307.h"

uint8_t bcdToDec(uint8_t bcd) {
    return (bcd >> 4) * 10 + (bcd & 0x0F);
}
uint8_t DecTobcd(uint8_t dec){
	return ( (dec/10*16) + (dec%10));
}
void Set_mode(int mode, int IS_MODE_12_PM){
	MODE = mode;
	i2c_startcondition();
	
	// write address
	uint8_t address = DS1307_address << 1;
	i2c_writebyte(address);
	
	// read ACK
	uint8_t ACK=i2c_readACK();
	if(ACK){
		i2c_stopcondition();
		return;
	}
	// write word address, first pointer address
	i2c_writebyte(HOUR_register);
	ACK=i2c_readACK();
	if(ACK == 1){
		i2c_stopcondition();
		return;
	}
	// begin transfering datas
	if(mode == 24) i2c_writebyte(0x00);// 0x3f = 0b0011 1111
	else if(mode==12){
		IS_PM = IS_MODE_12_PM;
		if(IS_MODE_12_PM) i2c_writebyte(0x60); // 0x40-0b0100 0000 AM , 0x60-0b0110 0000 PM 
			   
		else i2c_writebyte(0x40); 
	}
	ACK=i2c_readACK();
	if(ACK){
			i2c_stopcondition();
			return;
		}		

}
void RTC_write(uint8_t* data, int count){
	i2c_startcondition();
	
	// write address
	uint8_t address = DS1307_address << 1;
	i2c_writebyte(address);
	
	// read ACK
	uint8_t ACK=i2c_readACK();
	if(ACK){
		i2c_stopcondition();
		return;
	}
	// write word address, first pointer address
	i2c_writebyte(SECOND_register);
	ACK=i2c_readACK();
	if(ACK == 1){
		i2c_stopcondition();
		return;
	}
	// begin transfering datas
	else {
			for (int i=0;i<count;i++){
				// ghi chuoi data
				i2c_writebyte(data[i]);
				ACK=i2c_readACK();
				if(ACK){
					i2c_stopcondition();
					return;
				}
			}
	}
}
void RTC_read() {
    uint8_t data[7]; // T?o m?t buffer d? ch?a d? li?u t? DS1307

    // 1. B?t d?u giao ti?p I2C
    i2c_startcondition();

    // 2. G?i d?a ch? DS1307 v?i ch? d? ghi (write mode)
    uint8_t address = DS1307_address << 1; // Ð?a ch? DS1307 (shift 1 bit sang trái d? g?i d?a ch? ghi)
    i2c_writebyte(address);

    // 3. Ch? nh?n ACK t? DS1307
    if (i2c_readACK() != 1) {
        // 4. G?i d?a ch? thanh ghi b?t d?u (0x00) d? d?c t? thanh ghi giây
        do{
					i2c_writebyte(0x00);  // 0x00 là thanh ghi giây (second register)
				}
				while (i2c_readACK());        // Ch? ACK d? d?m b?o DS1307 nh?n d?a ch? thanh ghi
				i2c_stopcondition();
			
			
        // 5. B?t d?u l?i giao ti?p I2C d? chuy?n sang ch? d? d?c
        i2c_startcondition(); // B?t d?u l?i giao ti?p d? chuy?n sang ch? d? d?c

        // 6. G?i d?a ch? DS1307 v?i ch? d? d?c (read mode)
        address = (DS1307_address << 1) | 1; // Ð?a ch? DS1307 v?i bit d?c (bit 0 = 1 cho ch? d? d?c)
        i2c_writebyte(address);
				if(i2c_readACK()){
					return;
				}
        // 7. Ð?c d? li?u t? DS1307 (giây, phút, gi?, v.v.)
        for (int i = 0; i < 6; i++) {
            data[i] = i2c_readbyte();  // Ð?c t?ng byte
            i2c_sendACK();             // G?i ACK d? ti?p t?c d?c
        }
        data[6] = i2c_readbyte();       // Ð?c byte cu?i cùng (nam)
        i2c_sendNACK();                // G?i NACK sau khi d?c byte cu?i cùng
				i2c_stopcondition();
        // 8. Luu d? li?u vào các bi?n toàn c?c
        second = bcdToDec( data[0]);
        minute = bcdToDec( data[1]);
				if(MODE==12) {
					IS_PM = (data[2]>>5)&0x01; // 0b0010 0000;
					hour = ((data[2] >> 4) &0x01) * 10 + (data[2] & 0x0f);
				}
				else hour = ((data[2] >> 4) &0x03) * 10 + (data[2] & 0x0f);
        dayofweek = data[3];
        date = bcdToDec( data[4]);
        month = bcdToDec( data[5]);
        year = bcdToDec(data[6]);

        // 9. Ket thuc I2C
        
    } else {
        // N?u không nh?n du?c ACK, d?ng giao ti?p
        i2c_stopcondition();
    }
}

void Set_time(uint8_t hour, uint8_t minute, uint8_t second, uint8_t dayofweek, uint8_t date, uint8_t month, uint8_t year) {
    uint8_t data[7];  // Khai báo m?ng ch?a 7 giá tr? tuong ?ng v?i th?i gian

    data[0] = DecTobcd(second);    // Giây
    data[1] = DecTobcd(minute);    // Phút
    data[2] = ((hour / 10) << 4) | (hour % 10);      // Gi?
		if(MODE==12){
			data[2] |= IS_PM << 5|1<<6;
		}
    data[3] = DecTobcd(dayofweek); // Ngày trong tu?n
    data[4] = DecTobcd(date);      // Ngày
    data[5] = DecTobcd(month);     // Tháng
    data[6] = DecTobcd(year);      // Nam

    RTC_write(data,7);  // Truy?n m?ng vào hàm RTC_write d? ghi th?i gian
}
