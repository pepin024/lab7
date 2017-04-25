/* 
 * File:   NEOPIX.h
 * Author: pepin024
 *
 * Created on April 19, 2017, 1:06 PM
 */

#ifndef NEOPIX_H
#define	NEOPIX_H

#ifdef	__cplusplus
extern "C" {
#endif

    struct neopixelTag ;

    typedef struct neopixelTag{
        char pin;
        int strandSize;
        char brightness;
    } neopixel;
    
    neopixel initMatrix( char pin, int strandSize);
    void writeHighBit(int high, int low);
    void writeLowBit(int high, int low);
    void writeColor(neopixel *matrix, unsigned char displayArray[]);
    void sendColor(neopixel *matrix, unsigned long int colorArray[], unsigned char matrixArray[]);
    void setBrightness(neopixel *matrix, unsigned char brightness);


#ifdef	__cplusplus
}
#endif

#endif	/* NEOPIX_H */