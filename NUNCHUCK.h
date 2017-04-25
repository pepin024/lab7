/* 
 * File:   NUNCHUCK.h
 * Author: pepin024
 *
 * Created on April 24, 2017, 12:22 PM
 */

#ifndef NUNCHUCK_H
#define	NUNCHUCK_H

#ifdef	__cplusplus
extern "C" {
#endif
    struct controller;
    
    typedef struct controller{
        unsigned char joyX;
        unsigned char joyY;
        unsigned char c;
        unsigned char z;
        int accelX;
        int accelY;
        int accelZ;
    } controllerData;
    
    void nunchuckInit();
    void beginTransmission(unsigned char deviceAddress);
    void send(unsigned char command);
    void endTransmission();
    unsigned char readByte();
    controllerData getControllerData();



#ifdef	__cplusplus
}
#endif

#endif	/* NUNCHUCK_H */

