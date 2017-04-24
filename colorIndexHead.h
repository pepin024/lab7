/* 
 * File:   colorIndexHead.h
 * Author: Justus
 *
 * Created on April 19, 2017, 9:12 AM
 */

#ifndef COLORINDEXHEAD_H
#define	COLORINDEXHEAD_H

#ifdef	__cplusplus
extern "C" {
#endif
    void setIndexPacked(unsigned char index, unsigned char red, unsigned char blu, unsigned char grn, unsigned long int ac[]);
    unsigned char getR(unsigned char index, unsigned long int ac[]);
    unsigned char getG(unsigned char index, unsigned long int ac[]);
    unsigned char getB(unsigned char index, unsigned long int ac[]);
    void initColorArray(unsigned long int ac[]);



#ifdef	__cplusplus
}
#endif

#endif	/* COLORINDEXHEAD_H */

