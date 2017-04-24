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
neopixel initMatrix(char pin, int strandSize){
    neopixel foo;
    foo.pin = pin;
    //if(pin > 15)
    //    return 0;
    foo.strandSize = strandSize;
    foo.brightness = 255;
    AD1PCFG |= 0b0000111000111100; //Sets all AN pins on PORTB to function as digital pins
    TRISB &= (65535 - (1 << pin)); //Sets pin to output
    return foo;
}

void setBrightness(neopixel *matrix, unsigned char brightness)
{
    matrix->brightness = ++brightness;
    return;
}


void writeHighBit( int high, int low){
    
    //Interrupts are disabled so that a full port write can be done without worrying about the value of PORTB changing
    //A value is calculated for setting the bit hi and low and is written to the entire port
    
    asm("mov w0,LATB \n repeat #8 \n nop \n mov w1,LATB");
    return;
    
}

void writeLowBit(int high, int low){
    
    //Interrupts are disabled so that a full port write can be done without worrying about the value of PORTB changing
    //A value is calculated for setting the bit hi and low and is written to the entire port
    asm("mov w0,LATB \n repeat #1 \n nop\n mov w1,LATB ");
    return;
    
}

void writeColor(neopixel *matrix, unsigned char r, unsigned char g, unsigned char b)
{
    int pinMask = 1 << matrix->pin;
    int high = PORTB | pinMask;
    int low =  PORTB & (65535 - pinMask);
    int i;
   //To make the math and conditional nice 0 represents an unaltered RGB value, 
    //1 will end up setting the RGB values to 0 and 255 will return RBB values 
    //a step down from their original value.
    if(matrix->brightness){
        r = (r * matrix->brightness) >> 8;
        g = (g * matrix->brightness) >> 8;
        b = (b * matrix->brightness) >> 8;
    }
    //Interrupts are disabled so that a full port write can be done without worrying about the value of PORTB changing
    //A value is calculated for setting the bit hi and low and is written to the entire port
    __builtin_disi(0x3FFF); //disables interrupts while writing to array
    for(i=7; i >= 0; --i){
        if(g & (1<<i))
            writeHighBit(high, low);
        else
            writeLowBit(high, low);
    }
    for(i=7; i >= 0; --i){
        if(r & (1<<i))
            writeHighBit(high, low);
        else
            writeLowBit(high, low);
    }
    for(i=7; i >= 0; --i){
        if(b & (1<<i))
            writeHighBit(high, low);
        else
            writeLowBit(high, low);
    }
    __builtin_disi(0x0000); //Enable interrupts.
}

void sendColor(neopixel *matrix, unsigned long int colorArray[], unsigned char matrixArray[])
{
    //This function takes in one of the neopixel structs, a pallette, and an array that selects a color from the pallette for each LED
    //It then cycles through the length of the matrix and writes the color to each LED.
    int length = matrix->strandSize; //length is set to the length of array found in the neopixel struct
    int i;
    unsigned char red, green, blue; //rgb values to be written
    for (i=0; i<length; i++)
    {
        red = getR(matrixArray[i], colorArray);
        green = getG(matrixArray[i], colorArray);
        blue = getB(matrixArray[i], colorArray);
        
        writeColor(matrix, red, green, blue);
    }
    __delay_us(10);//latch the colors into the matrix
}
