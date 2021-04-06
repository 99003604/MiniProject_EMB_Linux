/* The Mini-Project consits of an external LED, a two-axis joystick and a Beaglebone Black board.
    We are accessing the LED using GPIO pins and by using the joystick the intensity of the LED is changing.
    The project is symbolizing the gear system of an automobile.
    Authors: Alen Reji Kokkad (99003553), Dopplapudi Sri Naga Anjaneya (99003527), N. Naveen Kumar Reddy (99003604)
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#define TRIGGER_FILE_NAME "/sys/class/leds/beaglebone:green:usr0/trigger"
#define LED_CONTROL_BRIGHTNESS "/sys/class/leds/beaglebone:green:usr0/brightness"
#define EXPORT_FILE_LOCATION "/sys/class/gpio/export"
#define JSUP_VALUE_LOCATION "/sys/class/gpio/gpio26/value"
#define JOYSTICK_UP_GPIO_VALUE 26
#define TRIGGER_FILE "none"
#define TRIGGER_MODE "brightness"
#define TRIGGER_DELAY_100ms 100000000
#define TRIGGER_DELAY_900ms 900000000
#define LED_OFF "0"
#define LED_ON "1"
#define BRIGHTNESS_DELAY_7 7
#define BRIGHTNESS_DELAY_3 3
#define BRIGHTNESS_DELAY_1 1
//#define TRUE 1
//#define FALSE 0
/* Function Declerations */
FILE* openFile(const char *name,const char *mode);
void printToFile(FILE* toPrint, const char *value);
void delayCode(long sec, long nsec);
void flashLED(int mode);
void turnLEDOn();
void turnLEDOff();
void changeTriggerMode();
int checkIfPressed();

int main() 
{
	int press_count = 0; //counter to record the number of times the joystick is pressesed.
	int led_flash_mode = 0; //how many times to flash the LED light.
    printf("A simple mini-project using a joystick and LED to show the working of automatic gear system in automobiles \n");
	while(1) 
    {		
	    int joystick_pressed_up = checkIfPressed();	// Receving the output from GPIO pin.
		if (joystick_pressed_up == 1) 
        {
            press_count++;
	        led_flash_mode = BRIGHTNESS_DELAY_1;	
		}
		else 
        {
			led_flash_mode = BRIGHTNESS_DELAY_7;
            led_flash_mode = BRIGHTNESS_DELAY_3;
            press_count++;
        }
	    printf("Flashing %d time(s):  Joystick = %d & counter = %d\n", led_flash_mode, joystick_pressed_up, press_count);
		flashLED(led_flash_mode);
	} // end while loop
	return 0;
}

/*
 Function to access the files inside the Beaglebone black
 */
FILE* openFile(const char *fname,const char *mode) 
{
    FILE *temp = fopen(fname, mode);
	if (temp == NULL) 
    {
		printf("ERROR: Unable to open export file %s - Exiting.\n",fname);
		exit(1);
	}
    return temp;
}

/*
Function that write to the files inside Beaglebone Black
 */
void printToFile(FILE* toPrint, const char *value) 
{
    int charWritten = fprintf(toPrint,"%s", value);
    if (charWritten <= 0) 
    { 
		printf("ERROR: Unable to write to file - Exiting.\n");
	}
	fclose(toPrint);
}

/*
Function to turn on the LED using the files in the Beaglebone Black
 */
void turnLEDOn() 
{
	FILE *pLedBrightnessFile = fopen(LED_CONTROL_BRIGHTNESS, "w");	// open LED_CONTROL_BRIGHTNESS in write mode
    fprintf(pLedBrightnessFile, LED_ON); // write LED_ON (1) to the file
	//printToFile(pLedBrightnessFile, "1");
    fclose(pLedBrightnessFile);
}

/*
Function to turn off the LED using the files in the Beaglebone Black
 */
void turnLEDOff() 
{
    FILE *pLedBrightnessFile = fopen(LED_CONTROL_BRIGHTNESS, "w"); // open LED_CONTROL_BRIGHTNESS in write mode
	fprintf(pLedBrightnessFile, LED_OFF); // write LED_OFF (0) to the file
	fclose(pLedBrightnessFile);
}

/*
Function to change the trigger mode of LED
 */
void changeTriggerMode() 
{
	FILE *pLedTriggerFile = fopen(TRIGGER_FILE_NAME, "w"); // open TRIGGER_FILE_NAME in write mode
	printToFile(pLedTriggerFile, TRIGGER_FILE); // write none to the file so as to access it.
	fclose(pLedTriggerFile);
}

/*
Function to check if the joystick is pressed forward or backward:
 */
int checkIfPressed() 
{
	FILE *export_file = openFile(EXPORT_FILE_LOCATION, "w");	
	fprintf(export_file, "%d", JOYSTICK_UP_GPIO_VALUE);
	fclose(export_file);
	FILE *joysup_value_file = openFile(JSUP_VALUE_LOCATION, "r");
	int value_read = 0;
	fscanf(joysup_value_file, "%d", &value_read);
	if (value_read == 1) 
    {
		fclose(joysup_value_file);
        fclose(export_file);
		return 1;
	}
    else 
    {
        fclose(joysup_value_file);
        fclose(export_file); 
        return 0;
    }
}

/*
 Function to delay lighting up of LED
 */
void delayCode(long seconds, long nanoseconds) 
{
    struct timespec reqDelay = {seconds, nanoseconds};
	nanosleep(&reqDelay, (struct timespec *) NULL);
}

/*
Function to flash/lightup LED 
 */
void flashLED(int mode) 
{
	if (mode == BRIGHTNESS_DELAY_1) //if mode is one than flash the LED once
    { 
	    turnLEDOn();  		                     //ON for 100ms
	    delayCode(0,TRIGGER_DELAY_100ms);	     //delay
	    turnLEDOff(); 		                     //OFF for 900ms
	    delayCode(0,TRIGGER_DELAY_900ms);
	    //return;
	}
    if (mode == BRIGHTNESS_DELAY_7) //if mode is seven than flash the LED seven times
    {
	    turnLEDOn();  	                            //ON for 100ms (1)
	    delayCode(0,TRIGGER_DELAY_100ms);           //dealy
	    turnLEDOff();                               //OFF for 100ms 
	    delayCode(0,TRIGGER_DELAY_100ms);	    //delay
	    turnLEDOn();                                //ON for 100ms (2)
	    delayCode(0,TRIGGER_DELAY_100ms);           //delay
	    turnLEDOff();                               //OFF for 100ms
	    delayCode(0,TRIGGER_DELAY_100ms);	    //delay
	    turnLEDOn();                                //ON for 100ms (3)
	    delayCode(0,TRIGGER_DELAY_100ms);	    //delay
	    turnLEDOff();                               //OFF for 100ms
	    delayCode(0,TRIGGER_DELAY_100ms);
        turnLEDOn();  	                            //ON for 100ms (4)
	    delayCode(0,TRIGGER_DELAY_100ms);           //dealy
	    turnLEDOff();                               //OFF for 100ms 
	    delayCode(0,TRIGGER_DELAY_100ms);	    //delay
	    turnLEDOn();                                //ON for 100ms (5)
	    delayCode(0,TRIGGER_DELAY_100ms);           //delay
	    turnLEDOff();                               //OFF for 100ms
	    delayCode(0,TRIGGER_DELAY_100ms);	    //delay
	    turnLEDOn();                                //ON for 100ms (6)
	    delayCode(0,TRIGGER_DELAY_100ms);	    //delay
	    turnLEDOff();                               //OFF for 100ms
	    delayCode(0,TRIGGER_DELAY_100ms);
        turnLEDOn();                                //ON for 100ms (7)
	    delayCode(0,TRIGGER_DELAY_100ms);	    //delay
	    turnLEDOff();                               //OFF for 900ms
	    delayCode(0,TRIGGER_DELAY_900ms);
	}	
	return;
}
