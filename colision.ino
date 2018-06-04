// defines pins numbers
const int trigPin = 8;
const int echoPin = 9;
// defines variables
long duration;
int distancee;

#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#define reed A0//pin connected to read switch
//storage variables
float radius = 1.1811;// tire radius (in inches)- CHANGE THIS FOR YOUR OWN BIKE
int buttonInt=0;
int reedVal;
long timer = 0;
float kmh = 0.00;
float distance=0.00;
float circumference;
boolean backlight;
int maxReedCounter = 100;
int reedCounter;


void setup(){
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  //Serial.begin(9600); // Starts the serial communication
  reedCounter = maxReedCounter;
  circumference = 2*3.14*radius;
  pinMode(1,OUTPUT);
  pinMode(2,INPUT);
  pinMode(reed, INPUT);
  lcd.begin(16,2);
  Serial.write(12);
  cli();

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  
  OCR1A = 1999;// = (1/1000) / ((1/(16*10^6))*8) - 1
  
  TCCR1B |= (1 << WGM12);
  
  TCCR1B |= (1 << CS11);   
  
  TIMSK1 |= (1 << OCIE1A);
  
  sei();//allow interrupts
  //END TIMER SETUP

  Serial.begin(9600);
} 


ISR(TIMER1_COMPA_vect) {//Interrupt at freq of 1kHz to measure reed switch
  reedVal = digitalRead(reed);//get val of A0
  if (reedVal){//if reed switch is closed
    if (reedCounter == 0){//min time between pulses has passed
      kmh = (56.8*float(circumference))/float(timer)*1.60934;//calculate kmh per hour
      timer = 0;//reset timer
      reedCounter = maxReedCounter;//reset reedCounter
    }
    else{
      if (reedCounter > 0){//don't let reedCounter go negative
        reedCounter -= 1;//decrement reedCounter
      }
    }
  }
  else{//if reed switch is open
    if (reedCounter > 0){//don't let reedCounter go negative
      reedCounter -= 1;//decrement reedCounter
    }
  }
  if (timer > 2000){
    kmh = 0;//if no new pulses from reed switch- tire is still, set mph to 0
  }
  else{
    timer += 1;//increment timer
  } 
 
 distance +=kmh;
 }


void loop(){

  if(distancee > 50){
    lcd.setCursor(0,0);
    lcd.print("               ");
    
    lcd.setCursor(0,1);
    lcd.print("               ");
  lcd.setCursor(0,0);
  lcd.print("Speed=");
  lcd.setCursor(6,0);
  lcd.print(kmh);
  lcd.setCursor(0,1);
  lcd.print("Distance=");
  lcd.setCursor(10,1);
  lcd.print(distance/3600000);}


  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
   duration = pulseIn(echoPin, HIGH);
// Calculating the distance
  distancee= duration*0.034/2;
// Prints the distance on the Serial Monitor

  if(distancee <= 50){

    lcd.setCursor(0,0);
    lcd.print("               ");
    
    lcd.setCursor(0,1);
    lcd.print("               ");

    lcd.setCursor(4,0);
    lcd.print("Warning!");
    
    lcd.setCursor(3,1);
    lcd.print("Collision!");
    
    
    }
 
 
}



