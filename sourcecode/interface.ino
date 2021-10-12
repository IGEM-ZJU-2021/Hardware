/******************************************************************************
 *  I2C arduino
 *  Ye
 *  7/15/2014
 *
 *  Arduino I2C interface with Analog Devices AD5933
 *
 *  Arduino A5 - I2C SCL
 *  Arduino A4 - I2C SDA
 *
 ******************************************************************************/

#include <Wire.h>
#include "io.h"
#include "REGISTER_MAP.h"
io measure;              //initialize a io class parameter

int pin1 = 5;//CLK
int pin2 = 6;//DATA
int pin3 = 7;//LE
int pin4 = 8;//program-control
int data_I[24] = {0,0,0,1,1,1,0,1,1,0,0,0,0,1,0,0,1,1,0,0,1,0,1,1};//power-down
int data_R[24] = {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0};//2Diveide
int data_N[24] = {0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1};//200Divide
int data_F[24] = {0,0,0,1,1,1,0,1,1,0,0,0,0,1,0,0,1,1,0,0,0,0,1,0};//Normal
int count_R = 0;
int count_N = 0;
int count_F = 0;
int count_I = 0;
void setup() {
  //initialize libraries
  Serial.begin(9600);
  pinMode(pin1,OUTPUT);
  pinMode(pin2,OUTPUT);
  pinMode(pin3,OUTPUT);
  pinMode(pin4,OUTPUT);
/******************************************************************/
/*
main

*/
  /*********************NNNNNNNNN*******************************/ 

  while(count_N<24)
  {
 digitalWrite(pin3,0);
 
 digitalWrite(pin1,0);
 digitalWrite(pin2,data_N[count_N]);
  //status_pin(count_R);
 delayMicroseconds(50);
 
 digitalWrite(pin1,1);
 digitalWrite(pin2,data_N[count_N]);
  //status_pin(count_R);
 delayMicroseconds(50);
 

 count_N++;
 if(count_N==24)
 {
   digitalWrite(pin3,1);
   delayMicroseconds(50);
   digitalWrite(pin3,0);
 }//if
  }//while
  
/*******************RRRR********************/

  while(count_R<24)
  {
 digitalWrite(pin3,0);
 
 digitalWrite(pin1,0);
 digitalWrite(pin2,data_R[count_R]);
  //status_pin(count_R);
 delayMicroseconds(50);
 
 digitalWrite(pin1,1);
 digitalWrite(pin2,data_R[count_R]);
  //status_pin(count_R);
 delayMicroseconds(50);
 

 count_R++;
 if(count_R==24)
 {
   digitalWrite(pin3,1);
   delayMicroseconds(50);
   digitalWrite(pin3,0);
 }//if
  }//while

 /****************FFFF********************/

  while(count_F<24)
  {
 digitalWrite(pin3,0);
 
 digitalWrite(pin1,0);
 digitalWrite(pin2,data_F[count_F]);
  //status_pin(count_R);
 delayMicroseconds(50);
 
 digitalWrite(pin1,1);
 digitalWrite(pin2,data_F[count_F]);
  //status_pin(count_R);
 delayMicroseconds(50);
 


 count_F++;
 if(count_F==24)
 {
   digitalWrite(pin3,1);
   delayMicroseconds(50);
   digitalWrite(pin3,0);
 }//if
  }//while

}

char m;
int flag = 0;

void loop() {
  if (flag == 0){
    while (1)
    {
      m = Serial.read();
      if (m == '1') break;
    };
    Serial.print("B");
    digitalWrite(pin4,HIGH);
    for (measure.count = 0; measure.count < 25; measure.count++){
      
      measure.freq_start = measure.FRE[measure.count];
      
      measure.freq_calib();
    }
    Serial.print("A");
    flag = 1;
  }
  while (1)
  {
    m = Serial.read();
    if (m == '2') break;
  };
  Serial.print("B");
  digitalWrite(pin4,LOW);
  for (measure.count = 0; measure.count < 25; measure.count++){
    measure.freq_start = measure.FRE[measure.count];
    measure.freq_sweep();
  }
  Serial.print("C");
}

void wait_for_press(void) {
  delay(1000);
  while (1) {
    if (Serial.available() > 0) {
      Serial.read();
      return;
    }
  }
}
