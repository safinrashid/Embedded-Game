// ADC.c
// Runs on TM4C123
// Provide functions that initialize ADC0
// Last Modified: 1/12/2021  
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly
// Labs 8 and 9 specify PD2
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// ADC initialization function using PD2 Ain5
// Input: none
// Output: none
void ADC_Init(void){ 
	
	SYSCTL_RCGCADC_R |= 0x0021;		//activate ADC5
	SYSCTL_RCGCGPIO_R |= 0x08;		//activate clock for Port D
	for(int nopLoop = 0; nopLoop < 100 ; nopLoop++) {}
	GPIO_PORTD_DIR_R &= ~0x04;		//make PD2 input
	GPIO_PORTD_AFSEL_R |= 0x04;		//enable alternate function on PD2
	GPIO_PORTD_DEN_R &= ~0x04;		//disable digital I/O on PD2
	GPIO_PORTD_AMSEL_R |= 0x04;		//enable analog functionality on PD2
		
	ADC0_PC_R &= ~0xF;						//clear max sample rate field
	ADC0_PC_R |= 0x1;							//configure for 125K samples/sec
	ADC0_SSPRI_R = 0x0123;				//sequencer 3 is highest priority
	ADC0_ACTSS_R &= ~0x0008;			//disable sample sequencer 3
	ADC0_EMUX_R &= ~0xF000;				//seq3 is software trigger
	ADC0_SSMUX3_R &= ~0x000F;			//clear ss3 field
	ADC0_SSMUX3_R	+= 5;						//set channel Ain5
	ADC0_SSCTL3_R = 0x0006;				//no TS0 D0, yes IE0 END0
	ADC0_IM_R &= ~0x0008;					//disable ss3 interuprs
	ADC0_ACTSS_R |= 0x0008;				//enable sample
		
}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD2, analog channel 5
uint32_t ADC_In(void){  
	
	uint32_t result;
	
	ADC0_PSSI_R = 0x0008;							//initiate SS3
	while((ADC0_RIS_R&0x08) == 0){};	//wait for conversion done
	result = ADC0_SSFIFO3_R & 0xFFF;	//read result
	ADC0_ISC_R = 0x0008;							//acknowledge completion
		
  return result; 
}


