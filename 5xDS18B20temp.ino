/*******************************************************

This sketch will read the 5 1Wire (DS18B20) temperature 
sensors and display the values on the 2x16 LCD panel.

Pontus Henningsson, December 2015

********************************************************/

#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Assign the addresses of your 1-Wire temp sensors.
DeviceAddress t1 = { 0x28, 0x82, 0xEF, 0x29, 0x07, 0x00, 0x00, 0xB6 };
DeviceAddress t2 = { 0x28, 0xDD, 0xBD, 0x1C, 0x07, 0x00, 0x00, 0x63 };
DeviceAddress t3 = { 0x28, 0x89, 0x2F, 0x29, 0x07, 0x00, 0x00, 0x6C };
DeviceAddress t4 = { 0x28, 0x41, 0xE4, 0x29, 0x07, 0x00, 0x00, 0x00 };
DeviceAddress t5 = { 0x28, 0xF5, 0xBD, 0x2A, 0x07, 0x00, 0x00, 0x35 };

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  // (rs, enable, d4, d5, d6, d7)

// Data wire is plugged into port 3 on the Arduino
#define ONE_WIRE_BUS 3
#define TEMPERATURE_PRECISION 9

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// define values used by the buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// variable to hold the display mode
#define NormalMode 	0
#define DiffMode 	1

int DisplayMode = NormalMode;

// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      

 if (adc_key_in > 1000) return btnNONE; 
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 150)  return btnUP; 		
 if (adc_key_in < 300)  return btnDOWN; 	
 if (adc_key_in < 500)  return btnLEFT; 	
 if (adc_key_in < 700)  return btnSELECT;  

 return btnNONE;  // when all others fail, return this...
}

void setup()
{
	Serial.begin(9600); // start serial port
	
	// Initialize LCD interface and specify width and height. 
	lcd.begin(16, 2);             
	
	// Clears the LCD screen and positions the cursor in the upper-left corner.
	lcd.clear();
	
	// Display status
	lcd.print("Initializing"); 
  	Serial.println("Dallas Temperature IC Control Library Start Up");
	delay(1000);

  	// Start up the library
  	sensors.begin();

  	// locate devices on the bus
  	lcd.clear();
  	lcd.print("Locating devices...");
  	delay(1000);
  	int numDevices = sensors.getDeviceCount();
  	lcd.setCursor(0,2);
  	lcd.print("Found ");
  	lcd.print(numDevices);
  	lcd.print(" devices");

	delay(1000);

	// set the resolution to 9 bit
  	sensors.setResolution(t1, TEMPERATURE_PRECISION);
  	sensors.setResolution(t2, TEMPERATURE_PRECISION);
  	sensors.setResolution(t3, TEMPERATURE_PRECISION);
  	sensors.setResolution(t4, TEMPERATURE_PRECISION);
  	sensors.setResolution(t5, TEMPERATURE_PRECISION);
  	
  	lcd.clear();
}

 
void loop()
{
	// call to issue a global temperature 
	// request to all devices on the bus
  	sensors.requestTemperatures();
	delay(1000);
	
	float tempC = 0;
	
	switch(DisplayMode)
	{
		case NormalMode:
		{
			lcd.clear();
			
			Serial.print("   t1   ");
			tempC = sensors.getTempC(t1);
			Serial.println(tempC,1);
			lcd.setCursor(0,0);
			lcd.print(tempC,1);
			
			Serial.print("   t2   ");
			tempC = sensors.getTempC(t2);
			Serial.println(tempC,1);
			lcd.setCursor(5,0);
			lcd.print(tempC,1);	
			
			Serial.print("   t3   ");
			tempC = sensors.getTempC(t3);
			Serial.println(tempC,1);
			lcd.setCursor(10,0);
			lcd.print(tempC,1);
			
			Serial.print("   t4   ");
			tempC = sensors.getTempC(t4);
			Serial.println(tempC,1);
			lcd.setCursor(0,1);
			lcd.print(tempC,1);
			
			Serial.print("   t5   ");
			tempC = sensors.getTempC(t5);
			Serial.println(tempC,1);
			lcd.setCursor(5,1);
			lcd.print(tempC,1);
			
			break;
		}
		
		// Based on the difference between t4 and the others
		case DiffMode:
		{
			lcd.clear();
			
			int t4_tempC = sensors.getTempC(t4);
			
			Serial.print("   t1   ");
			tempC = sensors.getTempC(t1)-t4_tempC;
			Serial.println(tempC,1);
			lcd.setCursor(0,0);
			lcd.print(tempC,1);
			
			Serial.print("   t2   ");
			tempC = sensors.getTempC(t2)-t4_tempC;
			Serial.println(tempC,1);
			lcd.setCursor(5,0);
			lcd.print(tempC,1);	
			
			Serial.print("   t3   ");
			tempC = sensors.getTempC(t3)-t4_tempC;
			Serial.println(tempC,1);
			lcd.setCursor(10,0);
			lcd.print(tempC,1);
			
			Serial.print("   t4   -> ");
			
			Serial.println(t4_tempC,1);
			lcd.setCursor(0,1);
			lcd.print(">");
			lcd.print(t4_tempC,1);
			lcd.print("<");
			
			Serial.print("   t5   ");
			tempC = sensors.getTempC(t5)-t4_tempC;
			Serial.println(tempC,1);
			lcd.setCursor(7,1);
			lcd.print(tempC,1);
			
			break;
		}
	}	
	Serial.println();
		

	

	lcd_key = read_LCD_buttons();  // read the buttons
	
	switch (lcd_key)               
 	{
   		case btnRIGHT: {Serial.println("RIGHT button pressed!");break;}
   		case btnLEFT: {Serial.println("LEFT button pressed!");break;}
   		case btnUP: {Serial.println("UP button pressed!");break;}
   		case btnDOWN: {Serial.println("DOWN button pressed!");break;}
   		case btnSELECT: 
   		{
   			Serial.println("SELECT button pressed!");
   			DisplayMode=!DisplayMode;
   			break;
   		}
     	case btnNONE: {Serial.println("NONE  ");break;}
 	}

}
