/*
Arduino Wind Speed Meter Anemometer mph � Adafruit anemometer (product ID 1733).
Modified code created March 2016 from original code created by Joe Burg 11th November 2014 at http://www.hackerscapes.com/ with help from Adafruit forum users shirad
*/

//Initialise LCD display

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27  // Define I2C Address where the PCF8574A is
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

int n = 1;

LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

int serial_in;

//Setup Variables windmeter

double x = 0;
double y = 0;
const int sensorPin = A1; //Defines the pin that the anemometer output is connected to
int sensorValue = 0; //Variable stores the value direct from the analog pin
float sensorVoltage = 0; //Variable that stores the voltage (in Volts) from the anemometer being sent to the analog pin
float windSpeed = 0; // Wind speed in meters per second (m/s)

float voltageConversionConstant = .004882814; //This constant maps the value provided from the analog read function, which ranges from 0 to 1023, to actual voltage, which ranges from 0V to 5V
int sensorDelay = 2000; //Delay between sensor readings, measured in milliseconds (ms)

//Anemometer Technical Variables
//The following variables correspond to the anemometer sold by Adafruit, but could be modified to fit other anemometers.

float voltageMin = .4; // Mininum output voltage from anemometer in mV.
float windSpeedMin = 0; // Wind speed in meters/sec corresponding to minimum voltage

float voltageMax = 2.0; // Maximum output voltage from anemometer in mV.
float windSpeedMax = 32; // Wind speed in meters/sec corresponding to maximum voltage

void setup()
{

  //Setup LCD display with welcome screen
  lcd.begin(16,2);

  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home();                   // go home
    
  lcd.print("Opstarten");
  lcd.setCursor(0, 1);
  lcd.print("MY windenergy");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  Serial.begin(9600); //Start the serial connection

}

//Anemometer calculations

void loop()
{
  sensorValue = analogRead(sensorPin); //Get a value between 0 and 1023 from the analog pin connected to the anemometer

  sensorVoltage = sensorValue * voltageConversionConstant; //Convert sensor value to actual voltage

  //Convert voltage value to wind speed using range of max and min voltages and wind speed for the anemometer


  if (sensorVoltage <= voltageMin) {
    //Check if voltage is below minimum value. If so, set wind speed to zero.
    windSpeed = 0;  
  } else {
    //For voltages above minimum value, use the linear relationship to calculate wind speed.
    windSpeed = ((sensorVoltage - voltageMin) * windSpeedMax / (voltageMax - voltageMin) * 2.23694);
  }

  //Max wind speed calculation
  x = windSpeed; if (x >= y) {
  y = x;
  } else {
  y = y;
  }

  //Print voltage and windspeed to serial

  Serial.print("Voltage: ");
  Serial.print(sensorVoltage);
  Serial.print("\t");
  Serial.print("Wind speed in m/s: ");
  Serial.println(windSpeed);

  //Display Wind Speed results to LCD with Max wind speed

  lcd.setCursor(0, 0);
  lcd.print("Windsnelheid m/s: ");
  lcd.setCursor(0, 1);
  lcd.print(windSpeed);
  lcd.setCursor(7, 1);
  lcd.print("Max=");
  lcd.setCursor(11, 1);
  lcd.print(y);

  delay(sensorDelay);
}
