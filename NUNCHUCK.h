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

    struct controllerData{
        unsigned char joyX;
        unsigned char joyY;
        unsigned char c;
        unsigned char z;
    };
    
    void nunchuckInit();
    void beginTransmission(unsigned char deviceAddress);
    void send(unsigned char command);
    void endTransmission();



#ifdef	__cplusplus
}
#endif

#endif	/* NUNCHUCK_H */

