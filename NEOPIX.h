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

    neopixel initMatrix(int pin, int strandSize);
    void writeHighBit(int high, int low);
    void writeLowBit(int high, int low);


#ifdef	__cplusplus
}
#endif

#endif	/* NEOPIX_H */

