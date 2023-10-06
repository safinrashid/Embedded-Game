#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "Print.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"
#include "Images.h"
#include "Sound.h"
#include "Timer0.h"
#include "Timer1.h"
#include "Timer2.h"
#include "DAC.h"


/*--------------------------*/
#define test 5			//1 for sim, 5 for board
#define initspeed0 	8000000		//speed of button polling (not used)
#define fallingspeed	15 //cant be 1
/*--------------------------*/


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds

int goblin_position, goblin_up = 159, lang, power, death = 0;
int initspeed1 = 5000000;
int initspeed = 10000000;		//speed of falling	//change for levels

void IO_Init(void) {
	
	SYSCTL_RCGC2_R |= 0x3F; 		//clock on all
	SYSCTL_RCGCGPIO_R |= 0x3F;
	__asm("nop");__asm("nop");  //wait 4 clock cycles		
	__asm("nop");__asm("nop");		
	
	GPIO_PORTF_LOCK_R = 0x4C4F434B;//unlock GPIO Port F
	GPIO_PORTF_CR_R = 0x1F; 		// allow changes to PF4-0				
	GPIO_PORTF_AMSEL_R = 0x00; 	//disable analog on PF
	GPIO_PORTF_PCTL_R = 0x00000000;//PCTL GPIO on PF4-0
	GPIO_PORTF_DIR_R = 0x0E;    //input PF4/0, output PF1-3				
	GPIO_PORTF_AFSEL_R = 0x00; 	//disable alt funct on PF
	GPIO_PORTF_PUR_R = 0x11; 		//enable pull-up on PF4/0
	GPIO_PORTF_DEN_R |= 0x1F;		//enable digital on PF0-4
	
	GPIO_PORTE_AMSEL_R = 0x00; 	//disable analog on PE
	GPIO_PORTE_DIR_R = 0x00;    //input PE0
	GPIO_PORTE_AFSEL_R = 0x00; 	//disable alt funct on PE
	GPIO_PORTE_DEN_R |= 0x03;		//enable digital on PE0
	
}

int main1(void){
  DisableInterrupts();
  TExaS_Init(NONE);       // Bus clock is 80 MHz 
  Random_Init(1);

  Output_Init();
  
  
	

  while(1){
		
  }

}


// You can't use this timer, it is here for starter code only 
// you must use interrupts to perform delays
void Delay1s(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 7272400;  // 1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}

void Delay10ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 72724;  // 0.01sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}
void Delay1msmain(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 7272;  // 0.001sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}



const unsigned short *objects[] = {food1, food2, food3, poison1, poison1, nullbox, nullbox, nullbox, nullbox};
#define numofobj 9

//null objects, generate on every column, 8 pieces of same code with different objects including a null object(black box)
	 
static int objectpos, score = 0;

void SysTick_Init(uint32_t period){

	NVIC_ST_CTRL_R = 0;																						
	NVIC_ST_RELOAD_R = period-1;																	
	NVIC_ST_CURRENT_R = 0;																				
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000;		
	NVIC_ST_CTRL_R = 0x0007; 
	//From lab6

}

uint32_t data, position, mailbox, status = 1;
void SysTick_Handler(void){
	
	//capture output from ADC (12-bit)
	data = ADC_In(); mailbox = ADC_In();
	status = 1; //flag
	
}

uint8_t CheckCollision(int x){ //check = CheckCollision(goblin_position);
	//col for 0->8   i*14
	int col;
	for(int col = 0; col <= 8; col++){
		if((col*14) - x > 8 || x - (col*14) < 8){//goblin_position = character x
			return col;
		}
	}
	return col; //never reaches
}


void Timer1A_Handler(void){ // can be used to perform tasks in background
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout

	if((GPIO_PORTE_DATA_R & 0x02) == 2){
		power = 3;
		Sound_powerup();
		if(death == 0){
	int maxbottom = goblin_up + 10; //where to stop falling
		
	static int i = 9;
	static int o[] = {0,0,0,0,0,0,0,0,0};//,o0=0,o1=0,o2=0,o3=0,o4=0,o5=0,o6=0,o7=0,o8=0;
	
	if(i < maxbottom && i != 9){
		ST7735_DrawBitmap((0*14), i, objects[o[0]], 15, 15);
			ST7735_FillRect((0*14), (i-30), 15, 15, 0x0000);
		ST7735_DrawBitmap((1*14), i, objects[o[1]], 15, 15);
			ST7735_FillRect((1*14), (i-30), 15, 15, 0x0000);
		ST7735_DrawBitmap((2*14), i, objects[o[2]], 15, 15);
			ST7735_FillRect((2*14), (i-30), 15, 15, 0x0000);
		ST7735_DrawBitmap((3*14), i, objects[o[3]], 15, 15);
			ST7735_FillRect((3*14), (i-30), 15, 15, 0x0000);
		ST7735_DrawBitmap((4*14), i, objects[o[4]], 15, 15);
			ST7735_FillRect((4*14), (i-30), 15, 15, 0x0000);
		ST7735_DrawBitmap((5*14), i, objects[o[5]], 15, 15);
			ST7735_FillRect((5*14), (i-30), 15, 15, 0x0000);
		ST7735_DrawBitmap((6*14), i, objects[o[6]], 15, 15);
			ST7735_FillRect((6*14), (i-30), 15, 15, 0x0000);
		ST7735_DrawBitmap((7*14), i, objects[o[7]], 15, 15);
			ST7735_FillRect((7*14), (i-30), 15, 15, 0x0000);
		ST7735_DrawBitmap((8*14), i, objects[o[8]], 15, 15);
			ST7735_FillRect((8*14), (i-30), 15, 15, 0x0000);
		i += fallingspeed;
	}
	else{
		if(i != 9){
			if(o[CheckCollision(goblin_position)] <= 2){
				score = score + power;
				Sound_eat();
				if (score >=10){
					initspeed = 7000000;
					DisableInterrupts();
					Timer1_Init(initspeed*1.2, 2);
					EnableInterrupts();
				} else if (score >=20){
					initspeed = 5000000;
					DisableInterrupts();
					Timer1_Init(initspeed*1.2, 2);
					EnableInterrupts();
				}
			}
			else if(o[CheckCollision(goblin_position)] <= 4){
				death = 1;
			}
		}
		ST7735_FillRect((0*14), (i-30), 15, 15, 0x0000);
		ST7735_FillRect((1*14), (i-30), 15, 15, 0x0000);
		ST7735_FillRect((2*14), (i-30), 15, 15, 0x0000);
		ST7735_FillRect((3*14), (i-30), 15, 15, 0x0000);
		ST7735_FillRect((4*14), (i-30), 15, 15, 0x0000);
		ST7735_FillRect((5*14), (i-30), 15, 15, 0x0000);
		ST7735_FillRect((6*14), (i-30), 15, 15, 0x0000);
		ST7735_FillRect((7*14), (i-30), 15, 15, 0x0000);
		ST7735_FillRect((8*14), (i-30), 15, 15, 0x0000);
		i = 8;
		objectpos = Random()%9; // returns a random number from 0 to 8
		 o[0] = Random()%numofobj; //return 0 to 8 (pointer to a struct)
		 o[1] = Random()%numofobj;
		 o[2] = Random()%numofobj;
		 o[3] = Random()%numofobj;
		 o[4] = Random()%numofobj;
		 o[5] = Random()%numofobj;
		 o[6] = Random()%numofobj;
		 o[7] = Random()%numofobj;
		 o[8] = Random()%numofobj;
	}
	Delay10ms(1);
}
	}
}

void Timer2A_Handler(void){ // can be used to perform tasks in background
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
	//GPIO_PORTF_DATA_R ^= 0x04; //Heartbeat
	if(death == 0){
	int maxbottom = goblin_up + 10; //where to stop falling
		
	static int i = 9;
	static int o[] = {0,0,0,0,0,0,0,0,0};//,o0=0,o1=0,o2=0,o3=0,o4=0,o5=0,o6=0,o7=0,o8=0;
	
	if(i < maxbottom && i != 9){
		ST7735_DrawBitmap((0*14), i, objects[o[0]], 15, 15);
			ST7735_FillRect((0*14), (i-30), 15, 15, 0x0000);
		ST7735_DrawBitmap((1*14), i, objects[o[1]], 15, 15);
			ST7735_FillRect((1*14), (i-30), 15, 15, 0x0000);
		ST7735_DrawBitmap((2*14), i, objects[o[2]], 15, 15);
			ST7735_FillRect((2*14), (i-30), 15, 15, 0x0000);
		ST7735_DrawBitmap((3*14), i, objects[o[3]], 15, 15);
			ST7735_FillRect((3*14), (i-30), 15, 15, 0x0000);
		ST7735_DrawBitmap((4*14), i, objects[o[4]], 15, 15);
			ST7735_FillRect((4*14), (i-30), 15, 15, 0x0000);
		ST7735_DrawBitmap((5*14), i, objects[o[5]], 15, 15);
			ST7735_FillRect((5*14), (i-30), 15, 15, 0x0000);
		ST7735_DrawBitmap((6*14), i, objects[o[6]], 15, 15);
			ST7735_FillRect((6*14), (i-30), 15, 15, 0x0000);
		ST7735_DrawBitmap((7*14), i, objects[o[7]], 15, 15);
			ST7735_FillRect((7*14), (i-30), 15, 15, 0x0000);
		ST7735_DrawBitmap((8*14), i, objects[o[8]], 15, 15);
			ST7735_FillRect((8*14), (i-30), 15, 15, 0x0000);
		
		i += fallingspeed;
	}
	else{
		if(i != 9){
		//if(CheckCollision(goblin_position) != 100){ //CheckCollision returns 0-8, 100
			if(o[CheckCollision(goblin_position)] <= 2){
			//its a food
				score = score + power;
				Sound_eat();
				if (score >=10){
					initspeed = 7000000;
					DisableInterrupts();
					Timer2_Init(initspeed, 2);
					EnableInterrupts();
				} else if (score >=20){
					initspeed = 5000000;
					DisableInterrupts();
					Timer2_Init(initspeed, 2);
					EnableInterrupts();
				}
			}
			else if(o[CheckCollision(goblin_position)] <= 4){
				death = 1;
				Sound_killed();
			}
		}
		ST7735_FillRect((0*14), (i-30), 15, 15, 0x0000);
		ST7735_FillRect((1*14), (i-30), 15, 15, 0x0000);
		ST7735_FillRect((2*14), (i-30), 15, 15, 0x0000);
		ST7735_FillRect((3*14), (i-30), 15, 15, 0x0000);
		ST7735_FillRect((4*14), (i-30), 15, 15, 0x0000);
		ST7735_FillRect((5*14), (i-30), 15, 15, 0x0000);
		ST7735_FillRect((6*14), (i-30), 15, 15, 0x0000);
		ST7735_FillRect((7*14), (i-30), 15, 15, 0x0000);
		ST7735_FillRect((8*14), (i-30), 15, 15, 0x0000);
		i = 8;
		objectpos = Random()%9; // returns a random number from 0 to 8
		 o[0] = Random()%numofobj; //return 0 to 8 (pointer to a struct)
		 o[1] = Random()%numofobj;
		 o[2] = Random()%numofobj;
		 o[3] = Random()%numofobj;
		 o[4] = Random()%numofobj;
		 o[5] = Random()%numofobj;
		 o[6] = Random()%numofobj;
		 o[7] = Random()%numofobj;
		 o[8] = Random()%numofobj;
	}
	Delay10ms(1);
}
}

int main(void){ 
  DisableInterrupts();
  TExaS_Init(NONE);       // Bus clock is 80 MHz 
  IO_Init();
	Output_Init();
	Sound_Init();
	ADC_Init();
	SysTick_Init(1000000);
	Random_Init(1);
	EnableInterrupts();
	
	goblin_position = 0; goblin_up = 0;
	lang = 0; 
	power = 1;
	
	while(lang == 0){//menu and language select
	ST7735_DrawBitmap(16,95,title,96,81);  
	ST7735_DrawString(5, 11, "English: SW1", 0xFFFF);
	ST7735_DrawString(5, 13, "Spanish: SW2", 0xFFFF);
		if((((GPIO_PORTF_DATA_R & 0x10) >> 4) != 1) || ((GPIO_PORTF_DATA_R & 0x01) != 1)){
			while(lang == 0){
				if(((GPIO_PORTF_DATA_R & 0x10) >> 4) != 1){
					lang = 1;
					int* ptr = (int*) malloc(8);
					ST7735_FillScreen(0x0000);
					ST7735_DrawString(1, 2, "Slide the goblin", 0xFFFF);
					ST7735_DrawString(1, 4, "Eat food", 0xFFFF);
							ST7735_DrawBitmap(8,65,food1,15,15);  
							ST7735_DrawBitmap(25,65,food2,15,15);  
							ST7735_DrawBitmap(42,65,food3,15,15);  
					ST7735_DrawString(1, 7, "Avoid poison", 0xFFFF);
							ST7735_DrawBitmap(8,95,poison1,15,15);  
							ST7735_DrawBitmap(25,95,poison1,15,15);  	
					ST7735_DrawString(1, 10,	"Move up, earn more", 0xFFFF);
					Delay100ms(8*test);
				}
				else if((GPIO_PORTF_DATA_R & 0x01) != 1){
					lang = 2;
					ST7735_FillScreen(0x0000);
					ST7735_DrawString(1, 2, "Mueve el monstruo", 0xFFFF);				
					ST7735_DrawString(1, 4, "Come comida", 0xFFFF);
							ST7735_DrawBitmap(8,65,food1,15,15);  
							ST7735_DrawBitmap(25,65,food2,15,15);  
							ST7735_DrawBitmap(42,65,food3,15,15);  
					ST7735_DrawString(1, 7, "Evitar el veneno", 0xFFFF);
							ST7735_DrawBitmap(8,95,poison1,15,15);  
							ST7735_DrawBitmap(25,95,poison1,15,15);  
					ST7735_DrawString(1, 10, "Sube y gana mas", 0xFFFF);
					Delay100ms(8*test);
				}
			}
		}
	}
	
	{//countdown
	ST7735_FillScreen(0x0000);
	ST7735_DrawCharS(50, 50, '3', 0xFFFF, 0x0000, 5);
	Delay100ms(2*test);
	ST7735_DrawCharS(50, 50, '2', 0xFFFF, 0x0000, 5);
	Delay100ms(2*test);
	ST7735_DrawCharS(50, 50, '1', 0xFFFF, 0x0000, 5);
	Delay100ms(1*test);
		
	ST7735_FillScreen(0x0000);
		if(lang == 1){
	ST7735_DrawCharS(35, 50, 'G', 0xFFFF, 0x0000, 3);
	ST7735_DrawCharS(55, 50, 'O', 0xFFFF, 0x0000, 3);}
		if(lang == 2){
	ST7735_DrawCharS(35, 50, 'V', 0xFFFF, 0x0000, 3);
	ST7735_DrawCharS(55, 50, 'E', 0xFFFF, 0x0000, 3);
		}
	ST7735_DrawCharS(70, 50, '!', 0xFFFF, 0x0000, 3);
	Delay100ms(1*test);
	ST7735_FillScreen(0x0000);
	}

	DisableInterrupts();
	Timer1_Init((initspeed*1.2), 1);
	Timer2_Init(initspeed, 2);
	EnableInterrupts();
	
	while(1){ //main while
	if(death == 1){
		ST7735_DrawBitmap(goblin_position, 160, deadgoblin, 17, 15);		
		if(power == 2){			
			ST7735_FillRect(goblin_position, (goblin_up-15), 40, 15, 0x0000);
				ST7735_FillRect(30, 135, 17, 15, 0x0000);
				ST7735_FillRect(95, 135, 17, 15, 0x0000);
		}	
		ST7735_FillRect(0,0,160,15, 0x0000);
		if(lang == 1){
			ST7735_FillRect(0, 0, 160, 30, 0x0000);
			ST7735_DrawString(1, 3, "~~~ U died ~~~       ", 0xFFFF);
			ST7735_DrawString(1, 4, "Score:", 0xFFFF);
				ST7735_SetCursor(8, 4);
				ST7735_SetTextColor(0xFFFF);
				ST7735_OutUDec(score);
				ST7735_DrawString(11, 4, "            ", 0xFFFF);
			ST7735_FillRect(0, 47, 160, 25, 0x0000);
			ST7735_DrawString(1, 7, "Press SW1/SW2 to     ", 0xFFFF);
			ST7735_DrawString(1, 8,"play again            ", 0xFFFF);
			ST7735_FillRect(0, 90, 160, 55, 0x0000);
			ST7735_FillRect(105, 0, 40, 100, 0x0000);
		}
		if(lang == 2){
			ST7735_DrawString(1, 3, "~~~ Moriste ~~~      ", 0xFFFF);
			ST7735_DrawString(1, 4, "Puntos:", 0xFFFF);
				ST7735_SetCursor(9, 4);
				ST7735_SetTextColor(0xFFFF);
				ST7735_OutUDec(score);
				ST7735_DrawString(12, 4, "            ", 0xFFFF);
			ST7735_FillRect(0, 47, 160, 24, 0x0000);
			ST7735_DrawString(1, 7, "Presione SW1/SW2     ", 0xFFFF);
			ST7735_DrawString(1, 8,"para jugar otra vez   ", 0xFFFF);
			ST7735_FillRect(0, 88, 160, 57, 0x0000);
//			ST7735_FillRect(105, 0, 40, 100, 0x0000);
		}
		if((((GPIO_PORTF_DATA_R & 0x10) >> 4) != 1) || ((GPIO_PORTF_DATA_R & 0x01) != 1)){
				death = 0; score = 0;
				ST7735_FillScreen(0x0000);
	}
}
	while(death == 0){//alive		loop
		{//move character
		if (status == 1) {
//			GPIO_PORTF_DATA_R ^= 0x04;// Heartbeat
			status = 0;	
			//up button check
			if((GPIO_PORTE_DATA_R & 0x01) == 1){
			power = 2;
				Sound_powerup();
			goblin_up = 120;
			ST7735_FillRect(0, 145, 160, 15, 0x0000);
				ST7735_DrawBitmap(30, 150, poweruparrow, 17, 15);
				ST7735_DrawBitmap(95, 150, poweruparrow, 17, 15);
			}
			else{
			goblin_up = 159;
				if(power == 2){ST7735_FillRect(0, 105, 160, 15, 0x0000);
				ST7735_FillRect(30, 135, 17, 15, 0x0000);
				ST7735_FillRect(95, 135, 17, 15, 0x0000);
				}
			power = 1;}	
			//12-bit to x-coord
			ST7735_FillRect(goblin_position, (goblin_up-15), 40, 15, 0x0000);
			goblin_position = ((128)*mailbox)/4096;
			if(goblin_position > 115){goblin_position = 115;}
			ST7735_DrawBitmap(goblin_position, goblin_up, goblin, 17, 15);  
		}  		
//		GPIO_PORTF_DATA_R ^= 0x04;// Heartbeat	
		ST7735_DrawString(0, 0, "SCORE: ", 0xFFFF);
			ST7735_SetCursor(8, 0);
			ST7735_SetTextColor(0xFFFF);
			ST7735_OutUDec(score);
		}
	}
}
}
