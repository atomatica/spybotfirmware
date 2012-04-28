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

// motors

const int rightMPin = 3;
const int leftMPin = 11;

const int rightDPin = 12; 
const int leftDPin = 13;


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

// motor inputs from android
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
  
  // set default states
  data[leftM] = 128;
  data[rightM] = 128;
  data[servo] = 140;
  
  // initialize the serial communication:
  Serial.begin(9600);
  
  // initialize the ledPin as an output:
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(rightMPin, OUTPUT);
  pinMode(leftMPin, OUTPUT);
  pinMode(rightDPin, OUTPUT);
  pinMode(leftDPin, OUTPUT);
  
  servo1.attach(servoPin);
}

//--------------------------------------------------------------
void loop() {

  // check if data has been sent from the computer:
  getData();
  checkConnection();
  
  // normal loop
  drive();
  analogWrite(led1Pin, data[led1]);
  analogWrite(led2Pin, data[led2]);
  servo1.write(data[servo]);

    
}

// drives the motors
//--------------------------------------------------------------
void drive() {
  boolean rightd = true;
  boolean leftd = true;
  unsigned int rightm = data[rightM];
  unsigned int leftm = data[leftM];
  if (rightm == 128) {
    rightm = 0;
  }
  else if (rightm < 128) {
    rightd = false;
    rightm = 255 - rightm;
  }
  
  if (leftm == 128) {
    leftm = 0;
  }
  else if (leftm < 128) {
    leftd = false;
    leftm = 255 - leftm;
  }
  
  // shifting to -128, 128
  //rightm = rightm - 128;
  //leftm = leftm - 128;
  
  // scaling to -256 to 256
  //rightm = 2*rightm;
  //leftm = 2*leftm;
  
  // setting booleans
  //boolean rightd = false;
  //boolean leftd = false;
  //if (rightm > 0) rightd = true;
  //if (leftm > 0) leftd = true;
  
  //rightm = abs(rightm);
  //leftm = abs(leftm);
  
  // actually driving motors 
  if (rightd) digitalWrite(rightDPin, HIGH);
  else digitalWrite(rightDPin, LOW);
  
  if (leftd) digitalWrite(leftDPin, HIGH);
  else digitalWrite(leftDPin, LOW);
  
  analogWrite(rightMPin, rightm);
  analogWrite(leftMPin, leftm);
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
    data[rightM] = 128;
    data[leftM] = 128;
    
    time = millis();
    waiting = true;
  }
}
   
