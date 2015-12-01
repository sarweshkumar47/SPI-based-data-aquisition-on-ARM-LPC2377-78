# SPI-based-data-aquisition-on-ARM LPC2378 (MCB2300)

### SPI Master slave module on ARM LPC23XX for data aquisition

Simple application on LPC2377/78 Microcontroller. Program has been developed using Keil tool.



                    (Slave)	          SPI	        (Master)
                    System 1 <--------------------> System 2 ---- LCD
                       |			                   |
               Temperature Sensor 	            Pressure Sensor

### Software Tools
==================
      Keil V4
      
### Hardware

      LPC2378/88 (MCB2300)
      
### Brief description about the project

1. Temperature sensor (may be an analog pot) is connected to an analog input of the System 1 (Slave device)
2. Pressure sensor is connected to an analog input of the System 2 (Main master device) and an LCD display is attached to System 2 
3. At regular intervals (Based on timer interrupt), System 1 samples the temperature sensor and sends the sampled data to System 2 over SPI communication
4. System 2 receives data from System 1 over SPI, samples pressure sensor at regular intervals (Based on timer interrupt) and compares the values with pre-defined range
5. If there is any mismatch, System 2 will trigger an alarm (in this case, just lightup an led)

### Steps

* Create 2 projects on Keil
* Add pressure_system2_lcd.c file to the first project and temperature_system1.c to the second project
* Compile each project and upload it on two MCB2300 boards

