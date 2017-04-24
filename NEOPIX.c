#ifndef FCY
#define FCY 16000000UL
#endif

#include <xc.h>
#include <p24Fxxxx.h>
#include <stdint.h>
#include <libpic30.h>
#include "NEOPIX.h"


/**
 * 
 * @param <b>pin: </b>Which pin[0:15] on PORTB is associated with the data line
 * @param <b>strandSize: </b> Number of neopixels in each matrix
 * @param <b>brightness: </b> [0:255] 0 is unaltered, 255 is unaltered
 * 
 * <b>@Return: </b>Returns 1 if matrix is initialized properly and 0 if it isn't
 * 
 * <b>Description: </b>All 
 * 
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


void writeHighBit( int high, int low){
    
    //Interrupts are disabled so that a full port write can be done without worrying about the value of PORTB changing
    //A value is calculated for setting the bit hi and low and is written to the entire port
    __builtin_disi(0x3FFF);
    
    asm("mov w0,LATB \n repeat #8 \n nop \n mov w1,LATB");
    __builtin_disi(0x0000); //Enable interrupts.
    return;
    
}

void writeLowBit(int high, int low){
    
    //Interrupts are disabled so that a full port write can be done without worrying about the value of PORTB changing
    //A value is calculated for setting the bit hi and low and is written to the entire port
    __builtin_disi(0x3FFF);
    asm("mov w0,LATB \n repeat #1 \n nop\n mov w1,LATB ");
    __builtin_disi(0x0000); //Enable interrupts.
    return;
    
}

void writeColor(neopixel *matrix, unsigned char r, unsigned char g, unsigned char b){
    int pinMask = 1 << matrix->pin;
    int high = PORTB | pinMask;
    int low =  PORTB & (65535 - pinMask);
    int i;
    
    
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
}

void sendColor(neopixel matrix, unsigned char colorArray[][3], unsigned char matrixArray[])
{
    int length = matrix.strandSize;
    int i;
    unsigned char red, green, blue;
    for (i=0; i<length; i++)
    {
        red = colorArray[matrixArray[i]][0];
        blue = colorArray[matrixArray[i]][1];
        green = colorArray[matrixArray[i]][2];
        writeColor(matrix, red, green, blue);
    }
}
