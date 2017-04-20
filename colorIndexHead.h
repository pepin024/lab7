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
    void setIndex(unsigned char index, char red, char grn, char blu);
    void setIndexPacked(unsigned char index, long packedCol);
    unsigned char getR(unsigned char index);
    unsigned char getG(unsigned char index);
    unsigned char getB(unsigned char index);
    void initColorArray(void);
    long getColor(unsigned char index);



#ifdef	__cplusplus
}
#endif

#endif	/* COLORINDEXHEAD_H */

