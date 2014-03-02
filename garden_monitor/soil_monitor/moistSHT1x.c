// Compile with: gcc -o moistSHT1x ./support_files/bcm2835-1.36/src/bcm2835.c ./RPi_SHT1x.c moistSHT1x.c

/*
Raspberry Pi SHT1x communication library.
By:      John Burns (www.john.geek.nz)
Date:    14 August 2012
License: CC BY-SA v3.0 - http://creativecommons.org/licenses/by-sa/3.0/

This is a derivative work based on
	Name: Nice Guy SHT11 library
	By: Daesung Kim
	Date: 04/04/2011
	Source: http://www.theniceguy.net/2722

Dependencies:
	BCM2835 Raspberry Pi GPIO Library - http://www.open.com.au/mikem/bcm2835/

Sensor:
	Sensirion SHT10 Temperature and Humidity Sensor interfaced to Raspberry Pi GPIO port

SHT pins:
	1. GND  - Connected to GPIO Port P1-06 (Ground)
	2. DATA - Connected via a 10k pullup resistor to GPIO Port P1-01 (3V3 Power)
	2. DATA - Connected to GPIO Port P1-18 (GPIO 24)
	3. SCK  - Connected to GPIO Port P1-16 (GPIO 23)
	4. VDD  - Connected to GPIO Port P1-01 (3V3 Power)

Note:
	GPIO Pins can be changed in the Defines of RPi_SHT1x.h
*/

#include <bcm2835.h>
#include <stdio.h>
#include "RPi_SHT1x.h"
#include <time.h>

void printTempAndHumidity(void)
{
	unsigned char noError = 1;  
	value humi_val,temp_val;
	
	
	// Wait at least 11ms after power-up (chapter 3.1)
	delay(20); 
	
	// Set up the SHT1x Data and Clock Pins
	SHT1x_InitPins();
	
	// Reset the SHT1x
	SHT1x_Reset();
	
	// Request Temperature measurement
	noError = SHT1x_Measure_Start( SHT1xMeaT );
	if (!noError) {
		return;
		}
		
	// Read Temperature measurement
	noError = SHT1x_Get_Measure_Value( (unsigned short int*) &temp_val.i );
	if (!noError) {
		return;
		}

	// Request Humidity Measurement
	noError = SHT1x_Measure_Start( SHT1xMeaRh );
	if (!noError) {
		return;
		}
		
	// Read Humidity measurement
	noError = SHT1x_Get_Measure_Value( (unsigned short int*) &humi_val.i );
	if (!noError) {
		return;
		}

	// Convert intergers to float and calculate true values
	temp_val.f = (float)temp_val.i;
	humi_val.f = (float)humi_val.i;
	
	// Calculate Temperature and Humidity
	SHT1x_Calc(&humi_val.f, &temp_val.f);

	//Print the Humidity to the console
	//printf("%0.2f\n",humi_val.f);


FILE *file;
file = fopen("lastread_soil_moist.out","w");
fprintf(file,"%0.2f\n",humi_val.f);
fclose(file);

}





int main ()
{

	//Initialise the Raspberry Pi GPIO
	if(!bcm2835_init())
		return 1;
	
	printTempAndHumidity();

	return 1;
}
