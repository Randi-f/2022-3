//#include "stm32f10x.h"
#include <platform.h>
#include <gpio.h>
#include <stdlib.h>
#include "delay.h"
#include "timer.h"

int Interrupt=0;

void task_activate(void);
void SysTick_Init(unsigned ticks);
static inline void SysTick_Wait_ms(unsigned ms);

// The function for the GPIO Interrupt Handler
void EXTI0_IRQHandler(){
	EXTI->PR |=1;
	//Adjust the variable GPIO interrupt here
	if(Interrupt==0){
		Interrupt=1;
	}
	else{
		Interrupt=0;
	}
	
}

void task_activate(void){
	
	
	gpio_toggle(PC_13);
	gpio_toggle(PC_14);
	gpio_toggle(PC_15);
}


// ####### Problem 4 
void SysTick_Init(unsigned ticks)//Similar to CMSIS-Core function SysTick_Config(ticks)
{	//Configure the SysTick->CTRL as SysTick_CTRL_ENABLE_Msk only.
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;//Common procedure should be :1.set CTRL to 0 to reset the bit 
																					//2.write LOAD with the new reload value
																					//3.write VAL to clear the current value to 0
																					//4.write CTRL to start the timer
	//while((SysTick->CTRL & SysTick_CTRL_ENABLE_Msk)==0);
	//while((SysTick->CTRL & 0x00010000)==0);
	//SysTick->CTRL = 0;
	//while((SysTick->CTRL & SysTick_CTRL_ENABLE_Msk)==0);
	//SysTick->CTRL= SysTick_CTRL_ENABLE_Msk;
}

// ####### Problem 4 
static inline void SysTick_Wait_ms(unsigned ms)
{
	//Invoke SysTick_Init that convert the ms to ticks (machine cycles) 
	//This function illustrates the relationship between machine cycles, systemcoreclock and frequency. ticks=SystemCoreClock/frequency(in Hz)=SystemCoreClock*s	
	SysTick_Init(ms*SystemCoreClock*1000);//call the function to initialize with variable ticks, which represent the machine cycles
}


int main(void) {
	
		// Initialise GPIO		
		//Set GPIO trigger for rising edge
		gpio_set_trigger(PA_0,Rising);
		//gpio_set_trigger(PA_15,Rising);
		//set input connected to push button PB
		gpio_set_callback(PA_0, *EXTI0_IRQHandler);
	  //1 pin configured as GPIO input connected to the push button.
		gpio_set_mode(PA_0,Input);
		//gpio_set_mode(PA_15,Input);
	  //1 pin configured as the GPIO output for the LED status signal.
		gpio_set_mode(PC_0,Output);
		//3 pins configured as GPIO outputs for the three scheduled tasks.
		gpio_set_mode(PC_13,Output);
		gpio_set_mode(PC_14,Output);
		gpio_set_mode(PC_15,Output);
	
		
	
		// Global Interrupt in NVIC, nested vectored interrupt controller, which manages and prioritizes external interrupts
		NVIC_EnableIRQ(EXTI0_IRQn);	//allows interrupt to be recognized in CMSIS interface
		__enable_irq();//manipulate the core exception mask register in CMSIS-CORE API, clears PM flag
	
	
		// %% ############ Problem 1 ############ %%
		
		while (1)
		{		
			if(gpio_get(PA_0)==0){// if the button is not pushed, or the status is 0, all the tasks and LED should terminate
						gpio_set_range(PC_13,3,0);//set PC_13,14,15 to 0 to represent task A,B,C are terminated
						gpio_set(PC_0,0);//set PC_0 to 0 to represent the LED is turned off
			}
			else{
				if(Interrupt==1){	//if the button is pushed and an interrupt is generated
					gpio_set(PC_0,1); //lit the LED
					delay_ms(50); //use the settled delay_ms as software timer					
					task_activate();
					delay_ms(50);//Set another timer delay 50ms for the changing state of the GPIO toggling.
					
				}
				else{
					gpio_set_range(PC_13,3,0);
					gpio_set(PC_0,0);
				}
			}
				
		}
		
		
		
		// %% ############ Problem 2 ############ %%	
		/*
		timer_init(50*1000);//the "unit" of timestamp is us, we want ms, so we should mutiply with 1000
		timer_set_callback(task_activate);//provide a pointer to the callback function, set interrupt handler
		timer_enable();//	Enable SysTick(start)
		while (1)
		{			
			//Using SysTick hardware timer
			if(gpio_get(PA_0)==0){
				gpio_set(PC_0,0);	
			}
			else{
				if(Interrupt==1){		
				gpio_set(PC_0,1);									
			}
			else{		
				
				gpio_set(PC_0,0);
			}	
			}
			
			
			__WFI();//wait for interrupt instead of wait for events, without it, the logic analyzer will have some immediate changes
		
		}
		*?
		
		// %% ############ Problem 4 ############ %%
		/*
		while (1)
		{		
			if(gpio_get(PA_0)==1){
				if(Interrupt==1){		//when an interrupt is triggered	
					gpio_set(PC_0,1);		//lit the LED
					SysTick_Wait_ms(50);//wait for 50 ms
					
					task_activate();	//concurrently run tasks ABC
					SysTick_Wait_ms(50);//Set another SysTick_Wait_ms(unsigned ms) with 50ms for the changing state of the GPIO toggling.
				}
				else{
					if(gpio_get(PA_0)==0){
							gpio_set(PC_0,0);
					}
				}	
			}
			else{
				gpio_set_range(PC_13,3,0);//set PC_13,14,15 to 0 to represent task A,B,C is terminated
				gpio_set(PC_0,0);//set PC_0 to 0 to represent the LED is turned off
			}
			
			

		}*/
	
	
	
	
}


