#include "lpc214x.h"
#include "util.h"

#define ENABLE_PIN      (1 << 24)
#define REGISTER_SELECT (1 << 25)

void DisplayInit(void);
void LcdCommand(unsigned char byte);
void LcdData(unsigned char byte);
void SetCursor(unsigned int line, unsigned int column);
void LcdPrint(char* str);

void DisplayInit(void){
    IODIR1 = ENABLE_PIN | REGISTER_SELECT; 
    IOCLR1 = ENABLE_PIN | REGISTER_SELECT;

    // Set pins P1.16 - P1.23 to output and clear
    IODIR1 = 0x00FF0000;
    IOCLR1 = 0x00FF0000;

    Delay();
    LcdCommand(0x01); // clear display
    Delay();
    LcdCommand(0x02); // return home
    Delay();
    LcdCommand(0x01); // clear again
    Delay();

    LcdCommand(0b00111000); // Set 8-bit 2-line mode
}

void LcdCommand(unsigned char byte){
    IOCLR1 = 0x00FF0000;
    IOSET1 = byte << 16;

    IOCLR1 = ENABLE_PIN | REGISTER_SELECT;
    Delay();
    IOSET1 = ENABLE_PIN;
    Delay();
    IOCLR1 = ENABLE_PIN;
    Delay();
}

void LcdData(unsigned char byte){
    IOCLR1 = 0x00FF0000;
    IOSET1 = byte << 16;

    IOCLR1 = ENABLE_PIN;
    Delay();
    IOSET1 = REGISTER_SELECT;
    Delay();
    IOSET1 = ENABLE_PIN;
    Delay();
    IOCLR1 = ENABLE_PIN | REGISTER_SELECT;
    Delay();
}

void SetCursor(unsigned int line, unsigned int column){
    if (line == 0)
        LcdCommand(0x80);
    else 
        LcdCommand(0xc0);
}

void LcdPrint(char* str){
    while(*str){
        LcdData(*str);
        str++;
    }
}