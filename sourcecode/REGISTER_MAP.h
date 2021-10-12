//read write registers
#define device_adr 0x0D		//default device address
#define control_hi 0x80		//control
#define control_lo 0x81
#define start_hi 0x82		//start frequency
#define start_mi 0x83
#define start_lo 0x84
#define freq_hi 0x85		//frequency increment
#define freq_mi 0x86
#define freq_lo 0x87
#define numincr_hi 0x88		//numer of increments
#define numincr_lo 0x89
#define cycle_hi 0x8A		//number of settling time cycles
#define cycle_lo 0x8B

//read only registers
#define status_reg 0x8F		//status
#define temp_hi 0x92		//temperature data
#define temp_lo 0x93
#define real_hi 0x94		//real data
#define real_lo 0x95
#define imag_hi 0x96		//imaginary data
#define imag_lo 0x97

#define R_ref 10000  //Calibrate resistence 1K
/******************************************************************************
 *  Control Register Table (control_hi)
 *  0x00	
 *  0x10	initialize with start frequency
 *  0x20	start frequency sweep
 *  0x30	increment frequency
 *  0x40	repeat frequency
 *  
 *  0x80	
 *  0x90	measure temperature
 *  0xA0	power-down mode
 *  0xB0	stanby mode
 *  0xC0	
 *  0xD0
 
 *  Status Resister Map
 *  0x01        Valid temperature measurement
 *  0x02        Valid real/imaginary data
 *  0x04        Frequency sweep complete
 ******************************************************************************/

