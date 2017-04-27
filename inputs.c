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
    
    T3CON = 0;
    PR3 = 100;
    TMR3 = 0;
    
    AD1CON1 = 0;
    AD1CON1bits.ASAM = 1;//Start sampling at end of conversion
    AD1CON1bits.SSRC = 0b10;
    
    
    AD1CON2 = 0;
    AD1CON2bits.CSCNA = 1;//CH0 scans through AD1CSSL pins
    AD1CON2bits.SMPI = 1;
    
    AD1CON3 = 0;
    AD1CON3bits.ADCS = 1;
    AD1CON3bits.SAMC = 3;
    
    AD1CSSL = 0;
    AD1CSSLbits.CSSL0 = 1;
    AD1CSSLbits.CSSL1 = 1;
    
    AD1CON1bits.ADON = 1;   
}

controllerData scanInputs(){
    controllerData foo;
    foo.c = PORTBbits.RB7;
    foo.z = PORTBbits.RB8;
    
    _AD1IF = 0;
    T3CONbits.TON = 1;
    while(_AD1IF == 0);
    T3CONbits.TON = 0;
    TMR3 = 0;
    _AD1IF = 0;
    
    foo.joyX = ADC1BUF0;
    foo.joyY = ADC1BUF1;
 return foo;
}