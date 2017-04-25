#ifndef FCY
#define FCY 16000000 //Frequency of MCU
#endif

#include <p24Fxxxx.h>
#include <xc.h>
#include <libpic30.h>
#include "inputs.h"

void inputInit(){
    
    TRISBbits.TRISB7 = 1;
    TRISBbits.TRISB8 = 1;
    
    AD1PCFGbits.PCFG0 = 0;
    AD1PCFGbits.PCFG1 = 0;
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    
    AD1CON1 = 0;
    AD1CON1bits.ASAM = 1;//Start sampling at end of conversion
    
    
    AD1CON2 = 0;
    AD1CON2bits.CSCNA = 1;//CH0 scans through AD1CSSL pins
    
    AD1CON3 = 0;
    AD1CON3bits.ADCS = 2;
    
    AD1CSSL = 0;
    AD1CSSLbits.CSSL0 = 1;
    AD1CSSLbits.CSSL1 = 1;
    
    AD1CON1bits.ADON = 1;   
}

controllerData scanInputs(){
    controllerData foo;
    foo.c = PORTBbits.RB7;
    foo.z = PORTBbits.RB8;
    
    __delay_us(10);//Give time for SH to stabilize
    AD1CON1bits.SAMP = 0;
    while(!AD1CON1bits.DONE);//wait for conversion to finish
    foo.joyX = ADC1BUF0;
    
    __delay_us(10);//Give some time to sample new channel
    AD1CON1bits.SAMP = 0;
    while(!AD1CON1bits.DONE);
    foo.joyY = ADC1BUF0;
    return foo;
    
}

