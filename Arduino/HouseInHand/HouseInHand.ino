#include <Servo.h>    // because I will use the servo to control a door

/* DIO pins */
const int DIO_bedroom1  = 8 ;
const int DIO_bedroom2  = 9 ;
const int DIO_recp      = 10;
const int DIO_kitchen   = 11;
const int DIO_bathroom  = 12;
const int DIO_fan       = 7 ;
const int DIO_door      = 6 ;
Servo door;

/*
  Function to initialize the digital inputs and outputs:
  bedrooms, reception, kitchen, bathroom
*/
void DIO_init() {
  pinMode(DIO_bedroom1, OUTPUT);
  pinMode(DIO_bedroom2, OUTPUT);
  pinMode(DIO_recp,     OUTPUT);
  pinMode(DIO_kitchen , OUTPUT);
  pinMode(DIO_bathroom, OUTPUT);
  pinMode(DIO_fan     , OUTPUT);
  /*Door*/
  door.attach(DIO_door);
  door.write(0);
}

void DIO_doorOpen(){ 
  door.write(90);
}

void DIO_doorClose(){ 
  door.write(0);
}



// Start up code, executed only once
void setup() {
  DIO_init();       // initialize the digital inputs/outputs
  Serial.begin(9600);     // Initialize Serial PORT <=> Bluetooth
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
      default: break;
    }
  }
}
