#ifndef FCY
#define FCY 16000000
#endif

#include <xc.h>
#include <p24Fxxxx.h>
#include <libpic30.h>
#include <p24FJ64GA002.h>
#include "NUNCHUCK.h"
#include "drvI2C.h"

#define SLAVEADDRESS 0x52
#define WRITEADDRESS 0xA4
#define READADDRESS 0xA5

//#define MYCODE

void nunchuckInit(){
    
#ifdef MYCODE
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
#else
    drvI2CInit();
    drvI2CWriteByte(0x40, 0x00, SLAVEADDRESS);
#endif
    
    
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
    TRISBbits.TRISB2 = 0;
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

unsigned char readByte(){
    TRISBbits.TRISB2 = 1;
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.RCEN = 1;
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    return I2C2RCV;
}

controllerData getControllerData(){
    controllerData foo;
    int i = 0;
    unsigned char temp[6];
#ifdef MYCODE
    beginTransmission(WRITEADDRESS);
    send(0x00);
    endTransmission(); 
        
    beginTransmission(READADDRESS);
    temp[i++] = readByte();
    I2C2CONbits.ACKDT = 0;
    I2C2CONbits.ACKEN = 1;
    while(I2C2CONbits.ACKEN);
    
    temp[i++] = readByte();
    I2C2CONbits.ACKDT = 0;
    I2C2CONbits.ACKEN = 1;
    while(I2C2CONbits.ACKEN);
    
    temp[i++] = readByte();
    I2C2CONbits.ACKDT = 0;
    I2C2CONbits.ACKEN = 1;
    while(I2C2CONbits.ACKEN);
    
    temp[i++] = readByte();
    I2C2CONbits.ACKDT = 0;
    I2C2CONbits.ACKEN = 1;
    while(I2C2CONbits.ACKEN);
    
    temp[i++] = readByte();
    I2C2CONbits.ACKDT = 0;
    I2C2CONbits.ACKEN = 1;
    while(I2C2CONbits.ACKEN);
    
    temp[i] = readByte();
    I2C2CONbits.ACKDT = 1;
    I2C2CONbits.ACKEN = 1;
    while(I2C2CONbits.ACKEN);
    
    endTransmission();
#else
    drvI2CReadRegisters(0x00, temp, 6, SLAVEADDRESS);
#endif
    
    
    for(i = 0; i < 6; ++i)
    temp[i] = (temp[i] ^ 0x17) + 0x17;
    
    foo.joyX = temp[0];
    foo.joyY = temp[1];
    foo.accelX = (temp[2] << 2) | ((temp[5] >> 2) & 0b11); //LSB of accelerometer data is masked in last byte.
    foo.accelY = (temp[3] << 2) | ((temp[5] >> 4) & 0b11);
    foo.accelZ = (temp[4] << 2) | ((temp[5] >> 6) & 0b11);
    foo.c = !(temp[5] & 0b10);
    foo.z = !(temp[5 & 0b1]);
    
    return foo;
}

