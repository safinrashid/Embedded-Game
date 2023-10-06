#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data



// **************DAC_Init*********************
// Initialize 6-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
	
		GPIO_PORTB_DIR_R |= 0x3F;		//output on PB0-5 (set)
		GPIO_PORTB_DEN_R |= 0x3F;		//enable digital on PB0-5
		//GPIO_PORTB_DR8R_R |= 0x3F;

}

// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 63 
// Input=n is converted to n*3.3V/63
// Output: none
void DAC_Out(uint32_t data){

		GPIO_PORTB_DATA_R = data;		//output data to DAC
	
}
