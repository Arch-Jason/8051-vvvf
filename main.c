#include <STC89C5xRC.H>
#include <stdio.h>
#include <math.h>
#include "spwmData.h"
#include "ADC0804.h"

#define STAT1 P00
#define STAT2 P01

float spwmDataDivider = 1, frequency;
unsigned int triwave = 20, spwmIndex, polarFlag, triwaveFlag, spwmIndexStep;
unsigned char adc;

/*
    triwave -- triangle wave
    spwmIndex -- index of spwm data array
    polarFlag -- output polarity flag
    triwaveFlag -- triangle wave increment/decrement flag
    spwmIndexStep -- spwm array index increment step for frequency
    spwmDataDivider -- voltage
*/
void Timer0_Init(void)		//50微秒@11.0592MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0x00;				//设置定时初始值
	TH0 = 0xEE;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
    
    EA = 1;
    ET0 = 1;
}


void spwm_routine() interrupt 1 {
    TL0 = 0x00;				//设置定时初始值
	TH0 = 0xEE;				//设置定时初始值
    
    if(spwmIndex > 99) { // change the output polarity flag
        spwmIndex = 0;
        polarFlag = !polarFlag;
    }
    
    if(triwave == 100) // change the triangle wave increment/decrement flag
        triwaveFlag = 0;
    if(triwave == 0)
        triwaveFlag = 1;
    
    if(!polarFlag) { // output 1
        if(spwm[spwmIndex] / spwmDataDivider > triwave) {
            STAT2 = 0; //off
            // delay(1); // dead time
            STAT1 = 1; // on
        } else {
            STAT1 = 0;
            // delay(1);
            STAT2 = 0;
        }
    }
    if(polarFlag) { // output 2
        if(spwm[spwmIndex] / spwmDataDivider > triwave) {
            STAT1 = 0;
            // delay(1);
            STAT2 = 1;
        } else {
            STAT1 = 0;
            // delay(1);
            STAT2 = 0;
        }
    }
    
    if(triwaveFlag == 1) // increment of triangle wave
        triwave += 20;
    if(triwaveFlag == 0)
        triwave -= 20; // decrement of triangle wave
    
    spwmIndex += spwmIndexStep; // increase spwm data index by the length of step variable
    
    adc = ADCRead();
    frequency = (float)adc/225;
    if(adc >= 0 && adc <= 255) {
        spwmIndexStep = frequency*40 + 10;  //10 - 50
    }
    return;
}

void main() {
    Timer0_Init();
    spwmDataDivider = 1;
    spwmIndexStep = 10;
    while(1);
}