/*
  2 IR Proximity Sensors, a servo and LCD and  interface code
  Opens a barrier gate using a Servo when a car is detected, else off.
     Automatic Parking Lot 
     When Parking space has been completely filled, the barrier gate opens 
     only for exit and 'no entry'
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Space Left for : " along
 with remaining cars to the LCD
  
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

There will be no problem if the power is lost for a short time as
EEPROM is used to store data.
  */
  
// include the library code:
#include <LiquidCrystal.h>
#include <Servo.h>
#include <EEPROM.h>
# define default_value 0xFF //Default value in EEPROM is 255 i.e. 0xFF
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
 int contrast=40;
 int addr = 0; // to store value in EEPROM
 const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int size=8;
int rem; // space remaining for cars
int pos;  // for position of servo
Servo myservo;  // create servo object to control a servo

const int SensorIn=7;  // YELLOW-WIRED Sensor for entrance (going in)
const int SensorOut=8;  // ORANGE-WIRED Sensor for exit

void setup() {                
  // runs after each powerup or reset
 int pos=0;  // for position of servo
      // set up the LCD's number of columns and rows:
myservo.attach(9);  // attaches the servo on pin 9 to the servo object
myservo.write(pos);
analogWrite(6,contrast);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Welcome ......");
if(EEPROM.read(addr)!=default_value){
  rem=EEPROM.read(addr);
}
else {
  rem=size;
}
  //  Serial.begin(9600);
  //Pin 2 is connected to the output of proximity sensor
  pinMode(SensorOut,INPUT);
  pinMode(SensorIn,INPUT);
 
  }

void loop() {
   
   delay(150);  // wait for 150 ms
   lcd.clear();
   lcd.setCursor(1, 0);
   lcd.println("Space left for ");
   lcd.setCursor(8,1);
   if(rem<0) {rem=0;
   EEPROM.write(addr,rem);
   }
   lcd.print(rem);
   lcd.print(" Cars");
  if(rem==0&&digitalRead(SensorIn)==HIGH)
  {
 //Serial.print(Sorry !! No space left);
  //Serial.println("cars \n");
     lcd.clear();
   lcd.setCursor(1, 0);
   lcd.print("Sorry !! ");
   lcd.setCursor(8,1);
   lcd.print("No space left");
   delay(2500);
  }
  else if(digitalRead(SensorIn)==HIGH&&rem>0)      //Check the sensor output
  {
  /*digitalWrite(13, HIGH);   // set the LED on
  delay(700); 
  Serial.print("Space available for:  \t");
  */
    rem--;
   EEPROM.write(addr,rem);
  //Serial.print(rem);
  //Serial.println("cars \n");
  lcd.clear();
   lcd.setCursor(1, 0);
   lcd.println("Space left for ");
   lcd.setCursor(8,1);
   lcd.print(rem);
   lcd.print(" Cars");

    for(; pos<=70; pos++)
  {
  myservo.write(pos);                  // sets the servo position according to the scaled value
  delay(5);                           // waits for the servo to get there
   }
  
  delay(2500);   // waits for car's entrance
  
  for(; pos>=0; pos--)
  {
  myservo.write(pos);                  // sets the servo position according to the scaled value
  delay(5);                           // waits for the servo to get there
   }
    delay(1500);
  
     
  }
  else if(digitalRead(SensorOut)==HIGH)
{
  rem++;
   EEPROM.write(addr,rem);
   lcd.clear();
   lcd.setCursor(1, 0);
   lcd.println("Space left for ");
   lcd.setCursor(8,1);
   if(rem>size){
    rem=size;
     EEPROM.write(addr,rem);
   }
   lcd.print(rem);
   lcd.print(" Cars");
    //Serial.print(rem);
  //Serial.println("cars \n");
  for(; pos<=70; pos++)
  {
  myservo.write(pos);                  // sets the servo position according to the scaled value
  delay(5);                           // waits for the servo to get there
   }
  delay(2500);   // waits for car's exit
  for(; pos>=0; pos--)
  {
  myservo.write(pos);                  // sets the servo position according to the scaled value
  delay(5);                           // waits for the servo to get there
  }
  delay(1500);
  } 
  }
