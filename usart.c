#include "usart.h"

uint8_t* IntToStr(int num) {
    static char buffer[10];  // S? d?ng bi?n static d? tránh b? h?y sau khi thoát kh?i hàm
    sprintf(buffer, "%d", num);  // Chuy?n s? nguyên num thành chu?i và luu vào buffer
    return (uint8_t*)buffer;     // Tr? v? con tr? d?n chu?i
	
}
void resetdata(uint8_t *str){
	while(*str){
	*str='\0';
	str++;
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
