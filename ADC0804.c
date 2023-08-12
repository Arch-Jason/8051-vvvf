#include <STC89C5xRC.H>
#include <intrins.h>
#include "delay.h"

#define CS P32
#define RD P33
#define WR P34
#define INTR P35

#define buffer P2

unsigned char ADCRead() {
    CS = 0;                 //chipselect is on now --It is active low pin
    WR = 0;                 // write is enabled --It is also active low
    // delay(1);
    WR = 1;                       //start conversion analog to digital
    RD = 1;                        
    while(INTR == 1);            //The looop runs until intr =  = 1 and 
                              //when intr =  = 0 it jumps to next iteration
    RD = 0;                     //read the digital output from adc
    return buffer;
}