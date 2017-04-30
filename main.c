#ifndef FCY
#define FCY 16000000 //Frequency of MCU
#endif

#include <p24Fxxxx.h>
#include <xc.h>
#include <libpic30.h>
#include "colorIndexHead.h"
#include "NEOPIX.h"
#include "inputs.h"

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
unsigned long int colorArray[16];
unsigned char array[] = {4, 2, 1, 15, 15};
unsigned char test1[] = {0, 1, 2, 3, 4};

unsigned char diamond[40] = {   0, 2, 0, 0, 0, 0, 0, 0,
                                2, 1, 2, 0, 0, 0, 0, 0,
                                0, 2, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0};

unsigned char square[40] = {    2, 2, 2, 2, 0, 0, 0, 0,
                                2, 1, 1, 2, 0, 0, 0, 0,
                                2, 2, 2, 2, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0};

unsigned char blank[40] = {     0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0};


void setup(void) { //initializer function
    CLKDIVbits.RCDIV = 0; // sets clock speed to 16MHz
    
    
    leftEye = initMatrix(6, 40); //initializes left eye to pin 5 with strandLength of 5, STRAND LENGTH IS SET TO 5 FOR TESTING PURPOSES, THIS SHOULD BE AT 40
    rightEye = initMatrix(5, 40);//initializes right eye to pin 6 with strandLength of 5, STRAND LENGTH IS SET TO 5 FOR TESTING PURPOSES, THIS SHOULD BE AT 40
    inputInit();
    initColorArray(colorArray);
    asm("nop");
    return;
}



int main(void) {
    setup(); //calls our setup function above to set leftEye and rightEye.
    
    //SET EYE-COLORS --- There are three colors to cycle between, blue, red, and yellow
    unsigned long int blueEye[3];
        setIndexPacked(0, 0, 0, 0, blueEye);
        setIndexPacked(1, 255, 255, 255, blueEye); //each eye has index 0 and 1 as black and white since those will be needed for every eye color
        setIndexPacked(2, 0, 0, 255, blueEye);      //the last index is reserved for whatever color the eye is to be
    unsigned long int redEye[3];
        setIndexPacked(0, 0, 0, 0, redEye);
        setIndexPacked(1, 255, 255, 255, redEye);
        setIndexPacked(2, 255, 0, 0, redEye); 
    unsigned long int yellowEye[3];
        setIndexPacked(0, 0, 0, 0, yellowEye);
        setIndexPacked(1, 255, 255, 255, yellowEye);
        setIndexPacked(2, 255, 255, 0, yellowEye);
    
    
    unsigned char displayArray[40];    //holds pattern to be displayed
    setBrightness(&rightEye, 60); //sets the brightness of right and left eye to 60/255
    setBrightness(&leftEye, 60);
    
    controllerData buttons;
    unsigned long int* colorPallete;
    

   
    
    while (1) 
    {
        int xShift, yShift;
        buttons = scanInputs();
        
        xShift = ((buttons.joyX) - 304)/211;
        yShift = ((buttons.joyY) - 322)/123;
        if(buttons.c)
            colorPallete = redEye;
        else
            colorPallete = blueEye;

        int i, j;

        for(i = 39; i >= 0; --i){
            j = i - yShift;
            if(j < 0 || ((j/8) != (i/8))){
                displayArray[i] = 0;
            } else {
               j -= xShift * 8;
               if(j < 0 ){
                   displayArray[i] = 0;
               } else {
                displayArray[i] = square[j]; 
               }
            }

        }
        sendColor(&leftEye, colorPallete, displayArray);


        for(i = 39; i >= 0; --i){
            j = i - yShift;
            if(j < 0 || ((j/8) != (i/8))){
                displayArray[i] = 0;
            } else {
                j -= xShift * 8;
                if(j < 0){
                    displayArray[i] = 0;
                } else {
                    displayArray[i] = diamond[j];
                }
            }
        }
        sendColor(&rightEye, colorPallete, displayArray);


        __delay_ms(200);
       
    }
}
   