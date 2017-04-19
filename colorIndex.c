/*
 * File:   colorIndex.c
 * Author: Justus
 *
 * Created on April 19, 2017, 9:05 AM
 */

#include <p24Fxxxx.h>
#include "xc.h"
#include "colorIndexHead.h"
unsigned char colorArray[16][3]; //colorArray will hold 16 different colors
                                 //Matrix to be output will hold an index to this colorArray.

void setIndex(unsigned char index, char red, char grn, char blu)
{
    colorArray[index][0] = red;
    colorArray[index][1] = grn;
    colorArray[index][2] = blu;
}

void setIndexPacked(unsigned char index, long packedCol)
{
    colorArray[index][0] = (char)(packedCol >> 16);
    colorArray[index][1] = (char)(packedCol >> 8);
    colorArray[index][2] = (char)(packedCol);
}

unsigned char getR(unsigned char index)
{
    return colorArray[index][0];
}

unsigned char getG(unsigned char index)
{
    return colorArray[index][1];
}
unsigned char getB(unsigned char index)
{
    return colorArray[index][2];
}
