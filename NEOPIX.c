#ifndef FCY
#define FCY 16000000UL
#endif

#include <xc.h>
#include <p24Fxxxx.h>
#include <stdint.h>
#include <libpic30.h>
#include "colorIndexHead.h"
#include "NEOPIX.h"


/**
 * 
 * @param <b>pin: </b>Which pin[0:15] on PORTB is associated with the data line
 * @param <b>strandSize: </b> Number of neopixels in each matrix
 */
neopixel initMatrix(char pin, int strandSize) //initializes a struct for the neopix matrix
{
    neopixel foo;
    foo.pin = pin; //selects the pin to use, on PORTB
    foo.strandSize = strandSize; //sets the length of the matrix
    foo.brightness = 255; //initializes brightness to MAX at 255
    AD1PCFG |= 0b0000111000111100; //Sets all AN pins on PORTB to function as digital pins
    TRISB &= (65535 - (1 << pin)); //Sets pin to output
    return foo;
}

void setBrightness(neopixel *matrix, unsigned char brightness) //function to change brightness of a neopixel struct
{
    matrix->brightness = ++brightness; //brightness is incremented by one, otherwise 0 was MAX, now 0 is off as far as user is concerned
    return;
}


void writeHighBit( int high, int low) //writes a high bit to the matrix using assembly commands
{
    //A value is calculated for setting the bit hi and low and is written to the entire port
    asm("mov w0,LATB \n repeat #8 \n nop \n mov w1,LATB");
    return;
}

void writeLowBit(int high, int low)
{
    //A value is calculated for setting the bit hi and low and is written to the entire port
    asm("mov w0,LATB \n repeat #1 \n nop\n mov w1,LATB ");
    return;
}

void writeColor(neopixel *matrix, unsigned char displayArray[]) //writes a string of colors using writeLow and writeHigh to fill entire matrix, takes in a displayArray from sendColor
{
    int pinMask = 1 << matrix->pin; //shifts a mask to only allow writing to the pin the matrix is connected to
    int high = PORTB | pinMask; //sets pin to high
    int low =  PORTB & (65535 - pinMask); //sets pin to low
    int i,j;
    int length = matrix->strandSize;//maps the length of the display
    unsigned char g, r, b; //holds rgb values to be written
   
    //Interrupts are disabled so that a full port write can be done without worrying about the value of PORTB changing
    //A value is calculated for setting the bit hi and low and is written to the entire port
    
    for(j=0; j<length; j++) //for loop to write the length of the neopix matrix
    {
        __builtin_disi(0x3FFF); //disables interrupts while writing to array
        g = displayArray[j*3]; //every third element contains the value for green, starting at 0
        for(i=7; i >= 0; --i) //uses writeHighBit and writeLowBit to push out value for green
        {
            if(g & (1<<i))
                writeHighBit(high, low);
            else
                writeLowBit(high, low);
        }
        r = displayArray[(j*3)+1]; //every third element contains the value for red, starting at 1
        for(i=7; i >= 0; --i)//uses writeHighBit and writeLowBit to push out value for red
        {
            if(r & (1<<i))
                writeHighBit(high, low);
            else
                writeLowBit(high, low);
        }
        b = displayArray[(j*3)+2]; //every third element contains the value for blue, starting at 2
        for(i=7; i >= 0; --i) //uses writeHighBit and writeLowBit to push out value for blue
        {
            if(b & (1<<i))
                writeHighBit(high, low);
            else
                writeLowBit(high, low);
        }
        __builtin_disi(0x0000); //Enable interrupts.
    }
}

void sendColor(neopixel *matrix, unsigned long int colorArray[], unsigned char matrixArray[]) 
{
    //This function takes in one of the neopixel structs, a palette, and a pattern array
    //It then cycles through the length of the matrix and writes the rgb colors to one array that is passed to write color
    int length = matrix->strandSize; //length is set to the length of array found in the neopixel struct
    char bright = matrix->brightness;//bright value is set to the brightness of the neopix matrix
    int i;
    unsigned char displayArray[length*3]; //displayArray holds individual rgb values to be written, it is multiplied by 3 to include r, g, b for each color
    for(i=0; i<length; i++)
    {
        unsigned char g = getG(matrixArray[i], colorArray); //sets each rgb value to be stored in the array to its proper color
        unsigned char r = getR(matrixArray[i], colorArray); //as indicated by the matrix pattern and the color pallette
        unsigned char b = getB(matrixArray[i], colorArray);
        
        if(bright) //scales rgb values to brightness
        {
        r = (r * bright) >> 8;       //To make the math and conditional nice 0 represents an unaltered RGB value, 
        g = (g * bright) >> 8;       //1 will end up setting the RGB values to 0 and 255 will return RBB values 
        b = (b * bright) >> 8;       //a step down from their original value.
        }
        
        displayArray[i*3] =  g;     //stores the rgb values in the displayArray
        displayArray[(i*3)+1] = r;
        displayArray[(i*3)+2] = b;
        
    }
    writeColor(matrix, displayArray); //calls write color for the displayArray
    __delay_us(10);//latch colors
}
