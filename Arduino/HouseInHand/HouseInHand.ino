#include <Arduino.h>
#include <Servo.h>    // because I will use the servo to control a door

#define LDR_LOW_THRESHOLD 450
#define LDR_HIGH_THRESHOLD 500




/* DIO pins */
const int DIO_bedroom1  = 8 ;
const int DIO_bedroom2  = 9 ;
const int DIO_recp      = 10;
const int DIO_kitchen   = 11;
const int DIO_bathroom  = 12;
const int DIO_fan       = 7 ;
const int DIO_door      = 6 ;

const int DIO_PIR		= 5 ;
const int DIO_BUZZER 	= 3	;

const int ANG_LDR		= A0;
const int ANG_TMP		= A1;

Servo door;



/* Global Variable */
// Default Values
String Garage_stat 	= "FREE";
String Alarm_stat 	= "OFF";
float Temperaure 	= 25.0;
bool Alarm_mode = false ;

/* Function Prototypes */
void DIO_doorOpen();
void DIO_doorClose();
void sendData();
void checkGarage();
void checkAlarm();
void readTemp();
float getInstantTemp();

/*
  Function to initialize the digital inputs and outputs:
  bedrooms, reception, kitchen, bathroom
*/
void DIO_init() {
  /*DIO*/
  pinMode(DIO_bedroom1, OUTPUT);
  pinMode(DIO_bedroom2, OUTPUT);
  pinMode(DIO_recp,     OUTPUT);
  pinMode(DIO_kitchen , OUTPUT);
  pinMode(DIO_bathroom, OUTPUT);
  pinMode(DIO_fan     , OUTPUT);	// Fan
  pinMode(DIO_PIR	  , INPUT );	// Motion Detection Sensor
  pinMode(DIO_BUZZER  , OUTPUT);	// Alarm Buzzer
  /*Analog */
  pinMode(ANG_LDR		,INPUT);	// Garage Sensor
  /*Door*/
  door.attach(DIO_door);
  door.write(0);
}

// Start up code, executed only once
void setup() {
  DIO_init();       // initialize the digital inputs/outputs
  Serial.begin(9600);     // Initialize Serial PORT <=> Bluetooth
  door.write(15);
}

// Program
void loop() {
  if(Serial.available()) {
    char x = Serial.read();
    switch(x) {
      // Bedroom1
      case 'c': digitalWrite(DIO_bedroom1, HIGH ); break;
      case 'd': digitalWrite(DIO_bedroom1, LOW  ); break;
      // Bedroom2
      case 'e': digitalWrite(DIO_bedroom2, HIGH ); break;
      case 'f': digitalWrite(DIO_bedroom2, LOW  ); break;
      // Reception
      case 'a': digitalWrite(DIO_recp,     HIGH ); break;
      case 'b': digitalWrite(DIO_recp,     LOW  ); break;
      // Kitchen
      case 'g': digitalWrite(DIO_kitchen,  HIGH ); break;
      case 'h': digitalWrite(DIO_kitchen,  LOW ); break;
      // Bathroom
      case 'i': digitalWrite(DIO_bathroom, HIGH ); break;
      case 'j': digitalWrite(DIO_bathroom, LOW  ); break;
      // Fan
      case 'm': digitalWrite(DIO_fan     , HIGH ); break;
      case 'n': digitalWrite(DIO_fan     , LOW  ); break;
      // Door
      case 'k': DIO_doorOpen  ();                  break;
      case 'l': DIO_doorClose ();                  break;
      // Alarm System
      case 'x' : Alarm_mode = true;  			   break;
      case 'y' : Alarm_mode = false;  			   break;
      
      default: break;
    }
  }
  
  
  checkGarage();
  checkAlarm();
  readTemp();
  sendData();
  delay(100);
  
  
}



void DIO_doorOpen(){ 
  door.write(90);
}

void DIO_doorClose(){ 
  door.write(15);
}


void sendData()
{
  Serial.print("#");
  Serial.print(Garage_stat);
  Serial.print("+");
  Serial.print(Alarm_stat);
  Serial.print("+");
  Serial.print(Temperaure);
  Serial.println("~");
  
}


void checkGarage()
{
  // We've used hysteresis control 
  int lux = analogRead(ANG_LDR);
  if(lux > LDR_HIGH_THRESHOLD) {
    // this means that the sensor is reading a dark lux (car in the garage)
    Garage_stat = "BUSY";    
  } else if ( lux < LDR_LOW_THRESHOLD) {
    // this means that (car place is free)
    Garage_stat = "FREE";
  } else {
    // Do nothing
  }
}

void checkAlarm()
{
  if (Alarm_mode == true )
  {
  // Alarm mode is on 
    if (digitalRead(DIO_PIR) == HIGH )
    {
     // there is somone
      Alarm_stat = "ALARM" ;
    }else 
    {
     // there is no one 
      Alarm_stat = "NORMAL" ;
    }
  }else 
  {
   Alarm_stat = "OFF";
    
  }
  
  
  // buzzer condition 
  if (Alarm_stat == "ALARM" )
  {
    // there is someone
    tone(DIO_BUZZER , 1000 );
    
  }else
  {
    noTone(DIO_BUZZER);
  }
  
}
void readTemp()
{

  // Using the average filter to stabilize the temperatures readings
  float sumOfTemp = 0;
  for(int i=0; i<64; i++)
  {
    // summing all the temperatures
    sumOfTemp += getInstantTemp();
  }
  // getting the average temperature
  Temperaure = (int)sumOfTemp >> 6;


  if (Temperaure > 50.0 ) 
  {
   // start of fire 
    
    //tone(DIO_BUZZER , 1000);
  }else
  {
  noTone(DIO_BUZZER);
  }
  
}

float getInstantTemp() {
    float rawData = analogRead(ANG_TMP);
    float volt = 5.0 * rawData / 1024.0 ; 
    float instantTemp = (volt) / 0.01 ;
    return instantTemp;
}
