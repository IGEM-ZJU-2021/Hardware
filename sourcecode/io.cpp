/******************************************************************************
 *  I2C arduino
 *  Ye
 *  7/15/2014
 *
 *  io.cpp
 *
 *  Arduino A5 - I2C SCL
 *  Arduino A4 - I2C SDA
 *
 ******************************************************************************/

#include "Arduino.h"
#include "Wire.h"
#include "io.h"
#include "REGISTER_MAP.h"
#include "stdlib.h"

/*const long io::FRE[21] = {
0x008312,0x00A503,0x00CFBC,0x010586,0x01493D,
0x019E7C,0x0209CF,0x0290EB,0x033B02,0x041124,
0x051EB8,0x067219,0x081D5A,0x0A373B,0x0CDC61,
0x1030DC,0x146212,0x19A929,0x204E16,0x28AB6B,
0x333333};*/

const long io::FRE[25] = {
53687091, 44291850, 36721970, 30172145, 24910810, 
20454782, 17018808, 13904956, 11542724, 9448928,
7892002, 6549825, 5245228, 4429185, 3656091,
3017214, 2491081, 2056216, 1696512, 1401233,
1154272, 955630, 789200, 649614, 536871};
/*const long io::FRE[21] = {
  167772,167772,167772,167772,167772,
  167772,167772,167772,167772,167772,
  167772,167772,167772,167772,167772,
  167772,167772,167772,167772,167772,
  167772};*/


io::io()
{
  Wire.begin();
}
  
int io::i2cRead(int Reg_adr){
  //initialize data
  int data=0;
  //begin interface, send "read" register address
  Wire.beginTransmission(device_adr);
  Wire.write(Reg_adr);
  Wire.endTransmission();
  
  //make request from device to get data
  Wire.beginTransmission(device_adr);
  Wire.requestFrom(device_adr, 1);
  if (Wire.available()){
    data = Wire.read();
  }
  
  //return data
  Wire.endTransmission();
  return data;
}

void io::i2cWrite(int Reg_adr, int data){
  //send output register address
  Wire.beginTransmission(device_adr);
  Wire.write(Reg_adr);
  
  //connect to device and write to register
  Wire.write(data);
  Wire.endTransmission();
}

void io::ioSetup(int select){  
  if(select == 2){                         //sweep
    i2cWrite(start_hi, ((0x00FF0000 & freq_start)/pow(16,4)));        //start frequency
    i2cWrite(start_mi, ((0x0000FF00 & freq_start)/pow(16,2)));
    i2cWrite(start_lo, ((0x000000FF & freq_start)/pow(16,0)));
    i2cWrite(freq_hi, ((0x00FF0000 & freq_step)/pow(16,4)));          //frequency increment
    i2cWrite(freq_mi, ((0x0000FF00 & freq_step)/pow(16,2)));
    i2cWrite(freq_lo, ((0x000000FF & freq_step)/pow(16,0)));
    i2cWrite(cycle_hi, 0x07);              //settling cycles
    i2cWrite(cycle_lo, 0xFF);
    i2cWrite(control_hi, 0xB1);            //standby mode starts
    i2cWrite(control_lo, 0x18);            //reset
    i2cWrite(control_hi, 0x11);            //initialize start frequency
    delay(1500);
  }
}

void io::freq_calib(void){
 
  ioSetup(2);                                 //new calibration
   
  main_sweep(1);
  return;
}

void io::freq_sweep(void){
  ioSetup(2);
  main_sweep(2);
  return;
}

void io::main_sweep(int select){
  i2cWrite(control_hi, 0x21);    //single freq        //perform frequency sweep
  stat = i2cRead(status_reg);
  
  //need to consider where to put the increment frequency command
  //while(((stat&6)!=6)){    
    stat = i2cRead(status_reg);
    while((stat&2)!=2){
      stat = i2cRead(status_reg);      
    }
    
    stat = i2cRead(status_reg);          //retrieve status
    r = i2cRead(real_hi)<<8 | i2cRead(real_lo);
    i = i2cRead(imag_hi)<<8 | i2cRead(imag_lo);
    mag = sqrt(sq(r) + sq(i));
    if(r<0) //add 180dgr
        phase = atan(i/r)+3.14;
    else if(i<0) 
        phase = atan(i/r)+3.14*2;
    else 
       phase=atan(i/r);
    
    if(select == 1){
      ref_mag[count] = mag;//cali restitance
      ref_phase[count] = phase;
      Serial.print("ref_mag[");
      Serial.print(count);
      Serial.print("]: ");
      Serial.print(ref_mag[count]);
      Serial.print("ref_phase[");
      Serial.print(count);
      Serial.print("]:");
      Serial.print(ref_phase[count]);
      Serial.print("\n");
      i2cWrite(control_lo, 0x18);              //reset
      i2cWrite(control_hi, 0xA1);            //power-down
      i2cWrite(control_lo, 0x08);
  return;
    }
    
    if(select == 2){
      Serial.print("10^");
      Serial.print(3+count/10.0);
      Serial.print("Hz:");
      Serial.print("\n");
      impedance = (ref_mag[count]/mag)*R_ref;
      Serial.print(impedance);
      Serial.print("\n");
      Serial.print("phase:");
      degree_phase = (phase-ref_phase[count])*180/3.14;
      Serial.print(degree_phase);
      Serial.print("\n");
      i2cWrite(control_lo, 0x18);              //reset
      i2cWrite(control_hi, 0xA1);            //power-down
      i2cWrite(control_lo, 0x08);
  return;
    }
    //i2cWrite(control_hi, 0x41);          //incrememnt frequency step
  //}
}
