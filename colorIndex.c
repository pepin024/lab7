/*
 * File:   colorIndex.c
 * Author: Justus
 *
 * Created on April 19, 2017, 9:05 AM
 */

#include <p24Fxxxx.h>
#include "xc.h"
#include "colorIndexHead.h"

void initColorArray(unsigned long int ac[]) //initializes array to 16 basic colors, these can be changed
{                                   //many of these colors appear the same on the matrix such as gray, light gray, dark gray
    setIndexPacked(0, 0, 0, 0, ac); //BLACK
    setIndexPacked(1, 0, 0, 170, ac); //BLUE
    setIndexPacked(2, 0, 170, 0, ac); //GREEN
    setIndexPacked(3, 0, 170, 170, ac); //CYAN
    setIndexPacked(4, 170, 0, 0, ac); //RED
    setIndexPacked(5, 170, 0, 170, ac); //MAGENTA
    setIndexPacked(6, 170, 85, 0, ac); //BROWN
    setIndexPacked(7, 170, 170, 170, ac); //LIGHT GRAY
    setIndexPacked(8, 85, 85, 85, ac); //DARK GRAY
    setIndexPacked(9, 85, 85, 170, ac); //BRIGHT BLUE
    setIndexPacked(10, 85, 255, 85, ac); //BRIGHT GREEN
    setIndexPacked(11, 85, 255, 255, ac); //BRIGHT CYAN
    setIndexPacked(12, 255, 85, 85, ac); //BRIGHT RED
    setIndexPacked(13, 255, 85, 255, ac); //BRIGHT MAGENTA
    setIndexPacked(14, 255, 255, 85, ac); //BRIGHT YELLOW
    setIndexPacked(15, 255, 255, 255, ac); //WHITE
}

/*void setIndex(unsigned char index, char red, char grn, char blu)
{
    colorArray[index][0] = red;
    colorArray[index][1] = grn;
    colorArray[index][2] = blu;
}*/

void setIndexPacked(unsigned char index, unsigned char red, unsigned char grn, unsigned char blu, unsigned long int ac[])
{
    ac[index] = ((long) red << 16) | ((long)grn << 8) | ((long) blu);
}

unsigned char getR(unsigned char index, unsigned long int ac[])
{
    return (unsigned char)(ac[index] >> 16);
}

unsigned char getG(unsigned char index, unsigned long int ac[])
{
    return (unsigned char)(ac[index] >> 8);
}
unsigned char getB(unsigned char index , unsigned long int ac[])
{
    return (unsigned char)(ac[index]);
}

