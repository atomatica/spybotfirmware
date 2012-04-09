/*
Plan:
  Every command will be sent in as two bytes.
  The first byte will be an address, or what value should be changed
  The second byte will be the new value
  
*/

#include <Servo.h>
Servo servo1;


// pins
//--------------------------------------------------------------
const int led1Pin = 5;      
const int led2Pin = 6;
const int servoPin = 9;

// array of data controlled by the anrdoid
//--------------------------------------------------------------
long time;
boolean waiting;
byte target;
byte data[256];

// indexes for the data
//--------------------------------------------------------------
const int led1 = 10;
const int led2 = 11;
const int led3 = 12;

const int servo = 20;
const int rightM = 21;
const int leftM = 22;


//--------------------------------------------------------------
void setup()
{
  // initializing variables
  waiting = true;
  target = 0;
  for (int i = 0; i < 256; i++) {
    data[i] = 0;
  }
  
  // initialize the serial communication:
  Serial.begin(9600);
  
  // initialize the ledPin as an output:
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  servo1.attach(servoPin);
}

//--------------------------------------------------------------
void loop() {

  // check if data has been sent from the computer:
  getData();
  checkConnection();
  
  // normal loop
  analogWrite(led1Pin, data[led1]);
  analogWrite(led2Pin, data[led2]);
  servo1.write(data[servo]);

    
}

// inputs data from the android
//--------------------------------------------------------------
void getData() {
  if (Serial.available()) {
    // checks for the start of a new command
    if (waiting) {
      time = millis();
      target = Serial.read();
      waiting = false;
    }

    // getting value of command
    else {
      data[target] = Serial.read();
      waiting = true;
    }
  }
}

// stops motors of no signals have been received in the last second
//--------------------------------------------------------------
void checkConnection() {
  if (millis() - time > 1000) {
    data[led1] = 0;
    data[led2] = 0;
    
    time = millis();
    waiting = true;
  }
}
   
