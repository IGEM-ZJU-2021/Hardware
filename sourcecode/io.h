/******************************************************************************
 *  I2C arduino
 *  Ye
 *  7/15/2013
 *
 *  io.h
 *
 *  Arduino A5 - I2C SCL
 *  Arduino A4 - I2C SDA
 *
 ******************************************************************************/

#ifndef io_h
#define io_h

#include "Arduino.h"
#include "Wire.h"
#include <SoftwareSerial.h>

class io{
  public:
    io();
    void i2cWrite(int Reg_adr, int data);
    int i2cRead(int Reg_adr);
    void freq_sweep(void);
    void freq_calib(void);
    long param_init(void);
    int count;
    
    long freq_start;
    long freq_end;
    long freq_step;
    long num_inc;
    
    static const long FRE[25];
    
  private:
    void main_sweep(int);
    void ioSetup(int);
    
    double r;                      //real data
    double i;                      //imag data
    double mag;                 //magnitude of real square and imag square
    double impedance;           //impedance value at each frequency point
    double phase;
    double degree_phase;
    double ref_phase[25];
    double ref_mag[25];
    double R;
    
    int temp;                   //temperature data (optional)
    int stat;
    int init;
    int DFT_stat;
    
};
#endif
