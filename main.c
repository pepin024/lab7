#ifndef FCY
#define FCY 16000000 //Frequency of MCU
#endif

#include <p24Fxxxx.h>
#include <xc.h>
#include <libpic30.h>
#include "colorIndexHead.h"
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

neopixel leftEye, rightEye; //these represent each LED matrix, they are of the neopixel Struct type defined in the NEOPIX.h

void setup(void) { //initializer function
    CLKDIVbits.RCDIV = 0; // sets clock speed to 16MHz
    
    
    leftEye = initMatrix(6, 5); //initializes left eye to pin 5 with strandLength of 5, STRAND LENGTH IS SET TO 5 FOR TESTING PURPOSES, THIS SHOULD BE AT 40
    rightEye = initMatrix(5, 5);//initializes right eye to pin 6 with strandLength of 5, STRAND LENGTH IS SET TO 5 FOR TESTING PURPOSES, THIS SHOULD BE AT 40
    asm("nop");
    
    return;
}

void loop(void) 
{
    return;
}

int main(void) {
    setup(); //calls our setup function above to set leftEye and rightEye.
    unsigned char array[5];
    array[0]= 4;
    array[1]=2;
    array[2]=1;
    array[3]=15;
    array[4]=5;
    unsigned long int colorArray[32];
    initColorArray(colorArray);
    setBrightness(&rightEye, 60);
    setBrightness(&leftEye, 60);
    
    while (1) 
    {
        sendColor(&rightEye, colorArray, array);
        __delay_ms(5);
        sendColor(&leftEye, colorArray, array); //sendColor takes 952 cycles between each write color
        __delay_ms(5);
    }

}