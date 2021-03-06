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

//BELOW 5 PATTERN ARRAYS ARE INITIALIZED
unsigned char diamond[40] = {   0, 2, 0, 0, 0, 0, 0, 0, //pattern for diamond shaped eye
                                2, 1, 2, 0, 0, 0, 0, 0,
                                0, 2, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0};

unsigned char square[40] = {    2, 2, 2, 2, 0, 0, 0, 0, //pattern for square shaped eye
                                2, 1, 1, 2, 0, 0, 0, 0,
                                2, 2, 2, 2, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0};

unsigned char blank[40] = {     0, 0, 0, 0, 0, 0, 0, 0, //"off" pattern
                                0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0};

unsigned char stripesLeft[40] ={0, 0, 0, 0, 0, 0, 0, 0, //stripes pattern for leftEye
                                1, 1, 1, 1, 1, 1, 1, 1,
                                2, 2, 2, 2, 2, 2, 2, 2,
                                3, 3, 3, 3, 3, 3, 3, 3,
                                4, 4, 4, 4, 4, 4, 4, 4};

unsigned char stripesRight[40] = { 5, 5, 5, 5, 5, 5, 5, 5, //stripes pattern for rightEye
                                   6, 6, 6, 6, 6, 6, 6, 6,
                                   7, 7, 7, 7, 7, 7, 7, 7,
                                   8, 8, 8, 8, 8, 8, 8, 8,
                                   9, 9, 9, 9, 9, 9, 9, 9};


void setup(void) //initializer function
{
    CLKDIVbits.RCDIV = 0; // sets clock speed to 16MHz
    leftEye = initMatrix(6, 40); //initializes left eye to pin 5 with strandLength of 40
    rightEye = initMatrix(5, 40);//initializes right eye to pin 6 with strandLength of 40
    inputInit(); //calls function to setup controller
    asm("nop");
    return;
}



int main(void) {
    setup(); //calls our setup function above to set leftEye and rightEye.
    int i,j; //used for loops
    
   //THESE SET OUR COLOR PALETTES
    unsigned long int blueEye[16];
        setIndexPacked(0, 0, 0, 0, blueEye);
        setIndexPacked(1, 255, 255, 255, blueEye); //each eye has index 0 and 1 as black and white since those will be needed for every eye color
        setIndexPacked(2, 0, 0, 255, blueEye);      //the last index is reserved for whatever color the eye is to be
        setIndexPacked(3, 255, 51, 153, blueEye);   //every index after is set almost randomly to demonstrate stripes pattern, as it is the only one that uses more than 3 colors
        setIndexPacked(4, 102, 0, 204, blueEye);
        setIndexPacked(5, 0, 204, 204, blueEye);
        setIndexPacked(6, 255, 255, 0, blueEye);
        setIndexPacked(7, 255, 0, 0, blueEye);
        setIndexPacked(8, 0, 153, 76, blueEye);
        setIndexPacked(9, 255, 102, 102, blueEye);
        setIndexPacked(10, 255, 128, 0, blueEye);
        setIndexPacked(11, 0, 255, 0, blueEye);
        setIndexPacked(12, 76, 0, 153, blueEye);
        setIndexPacked(13, 76, 153, 0, blueEye);
        setIndexPacked(14, 51, 255, 153, blueEye);
        setIndexPacked(15, 102, 51, 0, blueEye);
        
                
    unsigned long int redEye[16];
        setIndexPacked(0, 0, 0, 0, redEye);
        setIndexPacked(1, 255, 255, 255, redEye);
        setIndexPacked(2, 255, 0, 0, redEye); 
        setIndexPacked(3, 0, 245, 255, redEye);
        setIndexPacked(4, 0,139, 43, redEye);
        setIndexPacked(5, 255, 236, 139, redEye );
        setIndexPacked(6, 238, 130, 238, redEye);
        setIndexPacked(7, 193, 255, 193, redEye);
        setIndexPacked(8, 93, 71, 139, redEye);
        setIndexPacked(9, 237, 145, 33, redEye);
        for(i = 10; i < 16; i++)
        {
            setIndexPacked(i,0,0,0,redEye);
        }
    unsigned long int yellowEye[16];
        setIndexPacked(0, 0, 0, 0, yellowEye);
        setIndexPacked(1, 255, 255, 255, yellowEye);
        setIndexPacked(2, 255, 255, 0, yellowEye);
        setIndexPacked(4, 142, 142, 56, yellowEye);
        setIndexPacked(5, 84, 139, 84, yellowEye);
        setIndexPacked(6, 50, 100, 250, yellowEye);
        setIndexPacked(7, 130, 233, 17, yellowEye);
        setIndexPacked(8, 0, 255, 30, yellowEye);
        setIndexPacked(9, 140, 70, 80 ,yellowEye);
        for(i = 10; i < 16; i++)
        {
            setIndexPacked(i,0,0,0,yellowEye);
        }
    unsigned long int palleteFour[16];
        setIndexPacked(0,0,0,0,palleteFour);
        setIndexPacked(1,255,0,0,palleteFour);
        setIndexPacked(2,0,170,0,palleteFour);
        setIndexPacked(3,34,120,230,palleteFour);
        setIndexPacked(4,255,255,255,palleteFour);
        setIndexPacked(5,120,240,80, palleteFour);
        setIndexPacked(6,100,20,255, palleteFour);
        setIndexPacked(7,10,50,150,palleteFour);
        setIndexPacked(8,170, 0, 170,palleteFour);
        setIndexPacked(9,0, 170, 170,palleteFour);
        for(i = 10; i <16; i++)
        {
            setIndexPacked(i,0,0,0,palleteFour);
        }       
        
    unsigned long int palleteFive[16]; //sets every third index to red, green, or blue
    for(i = 0; i < 16; i++)
    {
        if(i < 11)
        {
            if(i % 3 == 0)
            {
                setIndexPacked(i,0,170,0,palleteFive); //sets to green
            }
            else if(i % 3 == 1) 
            {
                setIndexPacked(i,0, 0, 170,palleteFive); //sets to blue
            }
            else if(i % 3 == 2)
            {
                setIndexPacked(i,170, 0, 0,palleteFive); //sets to red
            }
        }
        else
        {
            setIndexPacked(i,0,0,0,palleteFive);
        }
    }
    
    unsigned char displayArray[40];    //holds pattern to be displayed
    setBrightness(&rightEye, 40); //sets the brightness of right and left eye to 40/255
    setBrightness(&leftEye, 40);
    
    controllerData buttons; //creates a struct for the controller
    unsigned long int* colorPallete;
    unsigned char* shapeArrayLeft;
    unsigned char* shapeArrayRight;

    int palleteChoose = 0;
    int shapeChoose = 0;
    int prevPalBut = 0; //checks to see if shape or palette is changed 
    int prevShapeBut = 0;
    
    while (1) //our infinite loop, this code is to run continuously. All setup is complete at this point
    {
        int xShift, yShift; //maps x and y to values that correspond to the display
        buttons = scanInputs(); //calls scanInput for the controller
        
        xShift = ((buttons.joyX) - 304)/211; //maps x to 0 through 2
        yShift = ((buttons.joyY) - 322)/123; // maps y to 0 through 4
        
        if((prevShapeBut != buttons.c) && buttons.c) //if button c for shape is pressed, the shape will be changed
        {
            shapeChoose++; //increments the shape
            if(shapeChoose > 2) //rolls over the shape
            {
                shapeChoose = 0;
            }
        }
        
        if((prevPalBut != buttons.z) && buttons.z) //if button z for color palette is pressed, then the palette will be changed
        {
            palleteChoose++;//increments the palette
            if(palleteChoose >= 5) //rolls over the palette at 5 since we have 6 palettes
            {
                palleteChoose = 0;
            }           
        }
        
        prevPalBut = buttons.z;     //sets previous button values to the current ones
        prevShapeBut = buttons.c;
        
        /////////////////////////////////////////////////////////////////////
        //Choose the appropriate color pallete
        if(palleteChoose == 0)
        {
            colorPallete = blueEye;
        }
        else if (palleteChoose == 1)
        {
            colorPallete = redEye;
        }
        else if (palleteChoose == 2)
        {
            colorPallete = yellowEye;
        }
        else if (palleteChoose == 3)
        {
            colorPallete = palleteFour;
        }
        else if (palleteChoose == 4)
        {
            colorPallete = palleteFive;
        }

        ///////////////////////////////////////////////////////////////////
        //Choose apropriate shape
         if(shapeChoose == 0)
         {
            shapeArrayLeft = diamond;
            shapeArrayRight = diamond;
        }
        else if(shapeChoose == 1)
        {
            shapeArrayLeft = square;
            shapeArrayRight = square;
        }
        else
        {
            shapeArrayLeft = stripesLeft;
            shapeArrayRight = stripesRight;
        }
        //i IS VALUE FOR DISPLAYARRAY, j IS VALUE FROM PATTERN ARRAY
        for(i = 39; i >= 0; --i) //loop to setup the pattern being displayed and its x and y position on the matrix
        {
            j = i - yShift; //look at what is stored in the pattern array at the yshift value
            if(j < 0 || ((j/8) != (i/8))) //can't be a negative index, check if j value is in same row as i
            {
                displayArray[i] = 0;
            } 
            else //if row is pushed off edge of matrix
            {
               j -= xShift * 8;
               if(j < 0 )
               {
                   displayArray[i] = 0;
               } 
               else 
               {
                displayArray[i] = shapeArrayLeft[j]; 
               }
            }

        }
        sendColor(&leftEye, colorPallete, displayArray); //write displayArray to leftEye


        for(i = 39; i >= 0; --i)//same loop as above but for right eye
        {
            j = i - yShift;
            if(j < 0 || ((j/8) != (i/8)))
            {
                displayArray[i] = 0;
            }
            else 
            {
                j -= xShift * 8;
                if(j < 0)
                {
                    displayArray[i] = 0;
                }
                else 
                {
                    displayArray[i] = shapeArrayRight[j];
                }
            }
        }
        sendColor(&rightEye, colorPallete, displayArray);//write displayArray to rightEye
        __delay_ms(200);
    }
}
   