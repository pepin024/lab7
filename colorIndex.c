/*
 * File:   colorIndex.c
 * Author: Justus
 *
 * Created on April 19, 2017, 9:05 AM
 */

#include <p24Fxxxx.h>
#include "xc.h"
#include "colorIndexHead.h"
unsigned char colorArray[32][3]; //colorArray will hold 16 default colors and 16 extra spaces for user colors.
                                 //Matrix to be output will hold an index to this colorArray.

unsigned char* initColorArray(void)
{
    setIndex(0, 0, 0, 0); //BLACK
    setIndex(1, 0, 0, 170); //BLUE
    setIndex(2, 0, 170, 0); //GREEN
    setIndex(3, 0, 170, 170); //CYAN
    setIndex(4, 170, 0, 0); //RED
    setIndex(5, 170, 0, 170); //MAGENTA
    setIndex(6, 170, 85, 0); //BROWN
    setIndex(7, 170, 170, 170); //LIGHT GRAY
    setIndex(8, 85, 85, 85); //DARK GRAY
    setIndex(9, 85, 85, 170); //BRIGHT BLUE
    setIndex(10, 85, 255, 85); //BRIGHT GREEN
    setIndex(11, 85, 255, 255); //BRIGHT CYAN
    setIndex(12, 255, 85, 85); //BRIGHT RED
    setIndex(13, 255, 85, 255); //BRIGHT MAGENTA
    setIndex(14, 255, 255, 85); //BRIGHT YELLOW
    setIndex(15, 255, 255, 255); //WHITE
    return colorArray;
}

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

long getColor(unsigned char index)
{
    return ((long)colorArray[index][0] << 16) | ((long)colorArray[index][1] << 8) | ((long)colorArray[index][2]);
}
