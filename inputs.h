/* 
 * File:   inputs.h
 * Author: pepin024
 *
 * Created on April 25, 2017, 2:26 PM
 */

#ifndef INPUTS_H
#define	INPUTS_H

#ifdef	__cplusplus
extern "C" {
#endif

    struct controller;
    
    typedef struct controller{
        int c;
        int z;
        int joyX;
        int joyY;
    } controllerData;
    
    void inputInit();
    controllerData scanInputs();


#ifdef	__cplusplus
}
#endif

#endif	/* INPUTS_H */