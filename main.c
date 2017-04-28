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
unsigned char blank[] = {0, 0, 0, 0, 0};
unsigned char test1[] = {0, 1, 2, 3, 4};


void setup(void) { //initializer function
    CLKDIVbits.RCDIV = 0; // sets clock speed to 16MHz
    
    
    leftEye = initMatrix(6, 40); //initializes left eye to pin 5 with strandLength of 5, STRAND LENGTH IS SET TO 5 FOR TESTING PURPOSES, THIS SHOULD BE AT 40
    rightEye = initMatrix(5, 40);//initializes right eye to pin 6 with strandLength of 5, STRAND LENGTH IS SET TO 5 FOR TESTING PURPOSES, THIS SHOULD BE AT 40
    inputInit();
    initColorArray(colorArray);
    asm("nop");
    
    return;
}


void loop(void) 
{
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
    
    
    unsigned char array[40];    //holds pattern to be displayed
    setBrightness(&rightEye, 60); //sets the brightness of right and left eye to 60/255
    setBrightness(&leftEye, 60);
    
    controllerData buttons;
    unsigned char flag = 1; //flag checks if eyes should be rewritten
    int xPos = 1;
    int yPos = 2;
    int oldX, oldY,botLeftPix; //used to see if the eye has moved.
    unsigned char eyeCol = 0, oldEyeCol = 0;
    while (1) 
    {
        buttons = scanInputs();
        
        xPos = ((buttons.joyX)-304)/211; //Maps joyX to 0-2
        if(xPos > 2)
            xPos = 2; //ensures that xPos doesn't exceed 2
        xPos = 2 - xPos; // The joy stick reads left as max, so the "2-" maps left to min and right to max
        yPos = ((buttons.joyY)-322)/123; //Maps joyY to 0-4
        if(yPos > 4)
            yPos = 4; //ensures that yPos doesn't exceed 4
        yPos = 4 - yPos; // The joy stick reads down as max, so the "4-" maps down to min and up to max
        
        //THIS SECTION WILL ASSEMBLE THE ARRAY - POSITION OF THE EYE
        if(xPos!=oldX || yPos!=oldY) //checks if the eye has moved from its old position
        {
            botLeftPix = xPos*8+yPos; //botLeftPix represents the bottom left position of the iris
            int i;
            for(i=0; i<40; i++)
            {
                if((i>=botLeftPix && i<botLeftPix+4) || (i>=botLeftPix+8 && i<botLeftPix+12) || (i>=botLeftPix+16 && i<botLeftPix+20))
                { //if statement is used to catch an index that is part of the iris or pupil which should be a color other than black
                    if(i==botLeftPix+9 || i==botLeftPix+10) //this catches if the index is part of the pupil and sets it white
                    {
                        array[i]=1;
                    }
                    else    //otherwise the index is part of the iris and is set to the desired color.
                    {
                        array[i]=2;
                    }
                }
                else //if an index is not part of the iris or pupil then it is sets to black
                {
                    array[i]=0;
                }
            }
            flag = 1; //set the flag to rewrite to the matrices
            oldX = xPos; //update old positions
            oldY = yPos;
        }
        
        
        //THIS SECTION SELECTS THE COLOR OF THE EYE
        if(buttons.c) //if button c is pushed it will set eyeCol to 1
        {
            eyeCol = 1;
        }
        else if(buttons.z) //if button z is pushed it will set eyeCol to 2
        {
            eyeCol = 2;
        }
        else // otherwise eyeCol is 0
        {
            eyeCol = 0;
        }
        
        if(eyeCol != oldEyeCol)
        {
            flag = 1; //set the flag to rewrite to the matrices
            oldEyeCol = eyeCol;
        }
        
        if(flag) //if we should rewrite the matrices
        {
            flag = 0; //reset flag
            //use conditions for the correct eye color
            if(eyeCol == 0)
            {
                sendColor(&rightEye, blueEye, array);
                __delay_ms(5);
                sendColor(&leftEye, blueEye, array);
                __delay_ms(5);
            }
            else if(eyeCol == 1)
            {
                sendColor(&rightEye, redEye, array);
                __delay_ms(5);
                sendColor(&leftEye, redEye, array);
                __delay_ms(5);
            }
            else
            {
                sendColor(&rightEye, yellowEye, array);
                __delay_ms(5);
                sendColor(&leftEye, yellowEye, array);
                __delay_ms(5);
            }
        }
    }       
}



/* EACH EYE WILL LOOK LIKE THE IMAGE BELOW
 * "." REPRESENTS A BLACK (OFF) LED
 * "=" REPRESENTS THE IRIS WHICH IS A COLOR THAT IS SELECTED
 * "+" REPRESENTS THE PUPIL WHICH WILL BE WHITE IN THIS CASE
 
  -----------
 | . . . . . |
 | . . . . . |
 | . = = = . |
 | . = + = . |
 | . = + = . |
 | . = = = . | <-THE LOWER LEFT OF THE IRIS IS WHAT botLeftPix MAPS TO, THIS IS USED AS A REFERENCE POINT TO BUILD THE EYE
 | . . . . . |
 | . . . . . |
  -----------
 
 * THE MATRIX WRITES FROM THE BOTTEM LEFT UP. ONCE IT REACHES THE TOP IT LOOPS BACK TO THE NEXT LED ON THE BOTTOM
 * 
  ----------------
 | 07 15 23 31 39 |
 | 06 14 22 30 38 |
 | 05 13 21 29 37 |
 | 04 12 20 28 36 |
 | 03 11 19 27 35 |
 | 02 10 18 26 34 |
 | 01 09 17 25 33 |
 | 00 08 16 24 32 |
  ----------------
 */

        /*setBrightness(&rightEye, buttons.joyX >> 2);
        setBrightness(&leftEye, buttons.joyY >> 2);
        if(buttons.c)
            sendColor(&rightEye, colorArray, array);
        else
            sendColor(&rightEye, colorArray, test1);
        __delay_ms(5);
        if(buttons.z)
            sendColor(&leftEye, colorArray, array); //sendColor takes 952 cycles between each write color
        else
            sendColor(&leftEye, colorArray, blank);
        __delay_ms(5);*/