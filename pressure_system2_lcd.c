/* System 2 (SPI Master)
   Collect the temperature from system 1 (SPI Slave) over SPI, pressure sensor value and
   display temperature and pressure on LCD
   Any mismatch in temperature and pressure values alert the system (Glow an LED)
*/

#include <LPC23xx.H>


int flag_int=0;

void timer_init()									   
{
	PCONP |=(1<<2);				//selecting function "11" for MAT1.0 port P1.22
	PINSEL3 |=0x003000;
	T1TCR=0x00;
	T1PR=0x00;                           
	T1MR0         = 24000000;   		/* 1sec = 12000000-1 at 12.0 MHz */
	T1MCR         = 3;     			/* Interrupt and Reset on MR0  */
	T1CTCR         = 0;			/* Timer0 Enable */

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
	PINSEL1 &=0xFFFFFCFF;  			// selecting SSEL as GPIO
	PINSEL0 |= 0xC0000000;	 		// selecting for SCLK
//	PINSEL1 |=0x3F;
	PINSEL1 |=0x3C; 		        // selecting for MISO, MOSI
//	FIO0DIR= 0x00200000;			//output direction for SSEL as output pin
//	FIO0SET= 0x00200000;			// deselecting the slave
} 

__irq void Timer1_ISR (void){			/* Timer interrupt block */

	// FIO2PIN = 0X33;
	AD0CR |= 0x01000000;			/* Start A/D Conversion */
	flag_int= 1;				/* Set flag */
	T1IR = 1;				/* Clear Timer0 interrupt flag */
	VICVectAddr = 0;			/* VIC Acknowledge Interrupt */
}

int main()
{


	unsigned long int pressure_data=0;
	unsigned long int temp_data=0;
	int pressure_x, pressure_y, pressure_new;
	float pressure_analog, temp_analog;
	int temp_x, temp_y, temp_new;
	
	PINSEL10 |=0x0;
	PINSEL3 |=0x00000000;			//GPIO selection for LCD
	PINSEL4 |=0x00000000;                   //GPIO selection for LED and SSEL for P2.8
	PINMODE4 |=0x00000000;                  //internal pull up for SSEL 
	FIO2DIR &=0x00000100;	        	//output direction for SSEL	using GPIO	

	FIO2SET=0x100;                 		//high signal for SSEL
	FIO2DIR |=0x000000ff;	 		//output direction for leds
	FIO2CLR |=0x000000ff;       		//clearing leds
	FIO2PIN=0x00;

	VICVectAddr5 = (unsigned long)Timer1_ISR;	/* Register Timer1 ISR with VIC */
	VICVectPriority5  = 15;                 	/* Timer1 Interrupt Priority */	
	VICIntEnable  = (1  << 5);			/* Enable Timer1 Interrupt     */


	timer_init();
	adc_init();
	spi_init();
	LcdInit();

	

	T1TCR |=0x01;                       		// start timer0

	while(1){
	
		if(flag_int){
	        S0SPCCR=0X08;	               		// configuring the clock
			S0SPCR=0x0a24;	                // configuring the SPI as master mode, 8bit
			FIO2CLR= 0x100;	                // selecting the Slave
			//FIO2PIN =0x00;
			S0SPDR=0xAA;   	                // data transfer begins
	 		while((S0SPSR & 0x80)==0);	// wait here till the data is transfered
			FIO2SET=0x100;		        //sending high signal to SSEL to stop the slave select
		//	FIO2PIN=S0SPDR;
			
			while(AD0DR0 & (1<<31));
			pressure_data=(AD0DR0>>6)& 0x3FF;

			temp_mdata=S0SPDR;
		
			LcdClear();

			if(((pressure_data>=310)&&(pressure_data<=620))&&((temp_data>=775)&&(temp_data<=930)))
			{
				temp_analog=(float)((temp_data *3.3)/1023);
				temp_new=SPI_analog *10;
				temp_x=SPI_new/10;
				temp_y=SPI_new%10;
				LcdPrint("Temp is ");
				LcdWriteData(temp_x+48);		/* Clear LCD Display */
				LcdPrint (".");				/* 1st Line Display */
				LcdWriteData(temp_y+48);
				LcdPrint("V");

				pressure_analog=(float)((pressure_data *3.3)/1023);
				pressure_new=pressure_analog*10;
				pressure_x=pressure_new/10;
				pressure_y=pressure_new%10;
				LcdSetCursor (0, 1);			/* Set Cursor on 2nd Line*/
				LcdPrint("Pressure is ");
				LcdWriteData(pressure_x+48);		/* Clear LCD Display */
		    		LcdPrint (".");				/* 1st Line Display */
				LcdWriteData(pressure_y+48);
				LcdPrint("V");
				
			} 
			else
			{
				temp_analog=(float)((temp_data *3.3)/1023);
				temp_new=temp_analog *10;
				temp_x=temp_new/10;
				temp_y=temp_new%10;
				
				LcdPrint("Temp is ");
				LcdWriteData(temp_x+48);		/* Clear LCD Display */
				LcdPrint (".");				/* 1st Line Display */
				LcdWriteData(temp_y+48);
				LcdPrint("V");

				pressure_analog=(float)((pressure_data *3.3)/1023);
				pressure_new=pressure_analog*10;
				pressure_x=pressure_new/10;
				pressure_y=pressure_new%10;
				
				LcdSetCursor (0, 1);			/* Set Cursor on 2nd Line*/
				LcdPrint("Pressure is ");
				LcdWriteData(pressure_x+48);		/* Clear LCD Display */
				LcdPrint (".");				/* 1st Line Display */
				LcdWriteData(pressure_y+48);
				LcdPrint("V");
				FIO2PIN=0xaa;  
				}
			 
			flag_int=0;
		
		}
	}
}


			  
				 
					 
