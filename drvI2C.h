
#ifndef DRVI2C_H
#define DRVI2C_H

// HardwareProfile includes the register definitions via <p24fxxxx.h> for PIC24..
#include "HardwareProfile.h"
#include "GenericTypeDefs.h"

// Change below defines to I2C2xxx if using I2C2
#define I2CCON        I2C2CON
#define I2CCONbits    I2C2CONbits
#define I2CSTAT       I2C2STAT
#define I2CSTATbits   I2C2STATbits
#define I2CMSK        I2C2MSK
#define I2CRCV        I2C2RCV
#define I2CTRN        I2C2TRN
#define I2CADD        I2C2ADD
#define I2CBRG        I2C2BRG

#define FCL        100000 // Check the max speed supported by your peripheral!!

#define I2C_DEBUG 0 // Change to 1 for debug messages


void drvI2CInit(void);
BOOL drvI2CReadRegisters(UINT8 reg, UINT8* rxPtr, UINT8 len, UINT8 slave_adr);
BOOL drvI2CWriteRegisters(UINT8 reg, UINT8* data, UINT8 len, UINT8 slave_adr);
BOOL drvI2CWriteByte(UINT8 reg, UINT8 byte, UINT8 slave_adr );

#endif
