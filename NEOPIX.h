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

    int initMatrix(int matrixID, int pin, int strandSize);
    int writeHighBit(int high, int low);
    int writeLowBit(int high, int low);


#ifdef	__cplusplus
}
#endif

#endif	/* NEOPIX_H */

