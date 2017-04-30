#ifndef FCY
#define FCY 16000000 //Frequency of MCU
#endif

#include <p24Fxxxx.h>
#include <xc.h>
#include <libpic30.h>
#include <p24FJ64GA002.h>
#include "inputs.h"

#define NUMOFSAMPLES 5

void inputInit() //initializes AD and TRIS registers for the controller
{
    TRISBbits.TRISB7 = 1; //these pins are used for the digital buttons c and z, set to input
    TRISBbits.TRISB8 = 1;
    
    AD1PCFGbits.PCFG0 = 0;//sets pins RA1 and RA0 to analog
    AD1PCFGbits.PCFG1 = 0;
    TRISAbits.TRISA0 = 1; //these pins are used for the analog joystick, they are set to input
    TRISAbits.TRISA1 = 1;
    
        //pins to control the shape of the eye and cycle through the color:
    TRISBbits.TRISB8 = 1; //shape of eye?
    TRISBbits.TRISB10 = 1; //color of eye?
    
    T3CON = 0;
    PR3 = 100; //sets up timer 3 which is used with sampling for ADC
    TMR3 = 0;
    
    AD1CON1 = 0;
    AD1CON1bits.ASAM = 1;//Start sampling at end of conversion
    AD1CON1bits.SSRC = 0b10;
    
    
    AD1CON2 = 0;
    AD1CON2bits.CSCNA = 1;//CH0 scans through AD1CSSL pins
    AD1CON2bits.SMPI = 1; //interrupts at every 2nd sample, so BUF1 the BUF2
    
    AD1CON3 = 0;
    AD1CON3bits.ADCS = 1; //AD conversion synced with clock cycle
    AD1CON3bits.SAMC = 3; //3 tads sample time (3 clock cycles)
    
    AD1CSSL = 0;
    AD1CSSLbits.CSSL0 = 1;
    AD1CSSLbits.CSSL1 = 1; //scans an0 and an1 pins
    
    AD1CON1bits.ADON = 1;   //turn on AD conversion
}

controllerData scanInputs() //user function to read joystick values and c and z buttons
{
    controllerData foo; //creates a controllerData struct
    int sample = 0;
    int xJoystickSum = 0;
    int yJoystickSum = 0;
    int n = 0;
    
    foo.c = !PORTBbits.RB7; //the buttons are pulled up, so the c and z are set to the inverse of them
    foo.z = !PORTBbits.RB8;
    while(n < 4) //this loop checks to see if a button was pressed for a significant amount of time, 4 loops
    { //exits loop when button has been held for 4 loops
        if((!PORTBbits.RB7 == foo.c) && (!PORTBbits.RB8 == foo.z)) //if the button's state hasn't changed, increment n
            n++;
        else    //if the button's state has changed reset n
            n = 0;
        
        foo.c = !PORTBbits.RB7; //will set c to most recent button state
        foo.z = !PORTBbits.RB8; //will set z to most recent button state
        __delay_us(10);
    }
    
   //below is to return the average of 5 samples of foo.joyX and foo.joyY:
    //NUMOFSAMPLES is #def'd up top
     for(sample = 0; sample < NUMOFSAMPLES; sample++) //buf0 (x) then buf1 (y))
     {
        _AD1IF = 0;
        T3CONbits.TON = 1;
        while(_AD1IF == 0);
        T3CONbits.TON = 0;
        TMR3 = 0;
        _AD1IF = 0;
        xJoystickSum = xJoystickSum + ADC1BUF0; //sum up 5 values of the joystick
        yJoystickSum = yJoystickSum + ADC1BUF1;
    }
    //now divide the sum by 5 and return it:
    foo.joyX = xJoystickSum / NUMOFSAMPLES; //average the sums
    foo.joyY = yJoystickSum / NUMOFSAMPLES; 
    //below is the original code
    //foo.joyX = ADC1BUF0;
    //foo.joyY = ADC1BUF1;
 return foo;
}