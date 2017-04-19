#ifndef FCY
#define FCY 16000000UL
#endif

#include <xc.h>
#include <p24Fxxxx.h>
#include <stdint.h>
#include <libpic30.h>


typedef struct {
    int pin;
    int strandSize;
    int initialized;
} neopixel;

neopixel activeNeopixels[16];//list of all neopixel objects
/**
 * 
 * @param <b>matrixID: </b>Assigns an ID to the matrix being initialized
 * @param <b>pin: </b>Which pin[0:15] on PORTB is associated with the data line
 * @param <b>strandSize: </b> Number of neopixels in each matrix
 * 
 * <b>@Return: </b>Returns 1 if matrix is initialized properly and 0 if it isn't
 * 
 * <b>Description: </b>Each Neopixel matrix is assigned an ID which is used to
 * store needed information about each matrix
 * 
 */
int initMatrix(int matrixID, int pin, int strandSize){
    if(activeNeopixels[matrixID].initialized == 1 || pin >= 16 || pin < 0){
        return 1;
    }
    activeNeopixels[matrixID].initialized = 1;
    activeNeopixels[matrixID].pin = pin;
    activeNeopixels[matrixID].strandSize = strandSize;
    AD1PCFG |= 0b0000111000111100; //Sets all AN pins on PORTB to function as digital pins
    TRISB &= (65535 - (1 << pin)); //Sets pin to output
    return 0;
}


int writeHighBit( int high, int low){
    
    //Interrupts are disabled so that a full port write can be done without worrying about the value of PORTB changing
    //A value is calculated for setting the bit hi and low and is written to the entire port
    __builtin_disi(0x3FFF);
    
    asm("mov w0,LATB \n repeat #8 \n nop \n mov w1,LATB");
    __builtin_disi(0x0000); //Enable interrupts.
    return 0;
    
}

int writeLowBit(int high, int low){
    
    //Interrupts are disabled so that a full port write can be done without worrying about the value of PORTB changing
    //A value is calculated for setting the bit hi and low and is written to the entire port
    __builtin_disi(0x3FFF);
    asm("mov w0,LATB \n repeat #1 \n nop\n mov w1,LATB ");
    __builtin_disi(0x0000); //Enable interrupts.
    return 0;
    
}