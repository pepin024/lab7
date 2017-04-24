#ifndef FCY
#define FCY 16000000
#endif

#include <xc.h>
#include <p24Fxxxx.h>
#include <libpic30.h>
#include "NUNCHUCK.h"

#define WRITEADDRESS 0xA4
#define READADDRESS 0xA5

void nunchuckInit(){
    AD1PCFGbits.PCFG4 = 1; //Sets SDA2/SCL2 as digital
    AD1PCFGbits.PCFG5 = 1;
    //RB3 = SCL2, RB2 = SDA2
    
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    
    I2C2CONbits.I2CEN = 0; //disable I2C bus
    I2C2BRG = 0x9d;      //SCL at 100kHz
    I2C2CONbits.I2CEN = 1; //enable I2C bus
    IFS3bits.MI2C2IF = 0; //clear interrupt flag
    
    beginTransmission(WRITEADDRESS);
    send(0x40);
    send(0x00);
    endTransmission();
    
}

void beginTransmission(unsigned char deviceAddress){
    I2C2CONbits.SEN = 1;//Start bit
    while(I2C2CONbits.SEN == 1);//Wait for SEN to finish
    IFS3bits.MI2C2IF = 0;//Reses interrupt flag
    
    I2C2TRN = deviceAddress;
    while(IFS3bits.MI2C2IF == 0);
    return;
}

void send(unsigned char command){
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = command;
    while(IFS3bits.MI2C2IF == 0);
    return;
}

void endTransmission(){
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN == 1);
    return;
}