/* System 1 (SPI Slave)
   Read the temperature sensor which is connected to ADC and send the data 
   to System 1 over SPI
*/

#include <LPC23xx.H>
#include <stdio.h>

int flag_int=0;


void timer_init()									   
{
	PCONP |=(1<<2);
	PINSEL3 |=0x003000;			//selecting function "11" for MAT1.0 port P1.22
	T1TCR=0x00;				//timer1 not started
	T1PR=0x00;                          
	T1MR0         = 60000000;   					
	T1MCR         = 3;     							
	T1CTCR         = 0;			/* Timer1 Enable */

} 

void adc_init()
{
	PCONP |=(1<<12);
	PINSEL1 |= 0x00004000 ;            	//selecting PO.23 for ADC
	AD0CR=0x00200301;                	//ADCclk is 3MHz  and interrupt for MAT1.0 
}

void spi_init()
{
	PCONP |=(1<<8);	   			// powering the SPI controller
	PINSEL0 |=0xC0000000;
	PINSEL1 |=0x3F;	   			// configuring for SSEL, MISO, MOSI
	S0SPCR=0x0A04;				//selects slave mode SCK in high mode transmitting 10 bits
	S0SPCCR=0x00;
} 
void led_init()
{
	PINSEL4 &= 0x00;   			// configuring as GPIO
	PINMODE4 &= 0x00;  			// configuring as pullup resistor
	FIO2DIR |= 0xFF;   			// configuring as output pin
	FIO2PIN =0x0;				// initializing LED to 0
}																				

__irq void Timer1_ISR (void){
	AD0CR |= 0x01000000;			/* Start A/D Conversion */
	flag_int= 1;				/* Set flag */
	FIO2PIN=0xf0;
	T1IR = 1;				/* Clear Timer0 interrupt flag */
	VICVectAddr = 0;			/* VIC Acknowledge Interrupt */

}
int main()
{
	unsigned long int ADC_sdata=0;
	
	VICVectAddr5 = (unsigned long)Timer1_ISR;	/* Register Timer1 ISR with VIC */
	VICVectPriority5  = 15;                 	/* Timer1 Interrupt Priority */	
	VICIntEnable  = (1  << 5);			/* Enable Timer1 Interrupt  */

	timer_init();
	adc_init();
	spi_init();
	led_init();

	T1TCR |=0x01;                       		// start timer1

	while(1)
	{	
		while(flag_int)
		{
					
			while(AD0DR0 &(1<<31));
			ADC_sdata=(AD0DR0>>6)& 0x3FF;
			S0SPDR=ADC_sdata;
		//	printf("hello");
			while((S0SPSR & 0x80)==0);	   // wait here till the data is transfered
			//printf("bye");
			FIO2PIN=0x44;
		}	
					
		
		flag_int=0;
	}
}
