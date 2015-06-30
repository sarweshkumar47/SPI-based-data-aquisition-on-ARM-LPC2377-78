# SPI-based-data-aquisition-on-MCB2300

SPI Master slave module on ARM LPC23XX for data aquisition

Simple application based on LPC2377/78 Microcontroller. Program has been developed using Keil tool.



		(Slave)		SPI	    (Master)
		System 1 <----------------> System 2 -- LCD
		   |			       |
   	  Temperature Sensor 	        Pressure Sensor

--> Temperature sensor (may be an analog pot) is connected to an analog input of the System 1 (Slave device)

--> Pressure sensor is connected to an analog input of the System 2 (Main master device) and an LCD display is attached to System 2 

--> At regular intervals (Based on timer interrupt), System 1 samples the temperature sensor and sends the sampled data to System 2 over SPI communication

--> System 2 receives data from System 1 over SPI, samples pressure sensor at regular intervals (Based on timer interrupt) and compares the values with pre-defined range

--> If there is any mismatch, System 2 will trigger an alarm (in this case, just lightup an led)

1) Create 2 projects on Keil
2) Add pressure_system2_lcd.c file to the first project and temperature_system1.c to the second project
3) Compile the code and upload it to MCB2300


