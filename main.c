#ifndef FCY
#define FCY 16000000
#endif

#include <p24Fxxxx.h>
#include <xc.h>
#include <libpic30.h>
#include "NEOPIX.h"

// PIC24FJ64GA002 Configuration Bit Settings
// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary oscillator disabled. 
// Primary Oscillator refers to an external osc connected to the OSC1 and OSC2 pins)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // OSC2/CLKO/RC15 functions as port I/O (RC15)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
// Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

 neopixel leftEye, rightEye;

void setup(void) {
    CLKDIVbits.RCDIV = 0;
    
    
    leftEye = initMatrix(5, 5);
    rightEye = initMatrix(6, 5);
    asm("nop");
    
    return;
}

void loop(void) {
    unsigned char i;
    for(i = 255; i >= 0; --i){
        setBrigthness( &leftEye, i);
        writeColor(&leftEye, 255, 0, 0);
        writeColor(&leftEye, 0, 255, 0);
        writeColor(&leftEye, 0, 0, 255);
        writeColor(&leftEye, 255, 255, 0);
    
        writeColor(&rightEye, 0, 0, 0);
        writeColor(&rightEye, 0, 0, 0);
        writeColor(&rightEye, 0, 0, 0);
        writeColor(&rightEye, 0, 0, 0);
        __delay_ms(2);
        writeColor(&leftEye, 0, 0, 0);
        writeColor(&leftEye, 0, 0, 0);
        writeColor(&leftEye, 0, 0, 0);
        writeColor(&leftEye, 0, 0, 0);
        
        writeColor(&rightEye, 255, 255, 0);
        writeColor(&rightEye, 0, 255, 255);
        writeColor(&rightEye, 255, 0, 255);
        writeColor(&rightEye, 255, 255, 255);
    
       
        __delay_ms(2);
    }
    return;
}

int main(void) {
    setup();
    while (1) {
        loop();
    }

}
