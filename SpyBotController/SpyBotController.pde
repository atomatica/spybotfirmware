
import processing.serial.*;
Serial port;

// indexes for the data
byte led1 = 10;
byte led2 = 11;
byte led3 = 12;
byte maintain = 0;

byte servo = 20;
byte rightM = 21;
byte leftM = 22;

long time;

void setup() {
  size(256, 150);

  println("Available serial ports:");
  println(Serial.list());

  port = new Serial(this, Serial.list()[0], 9600);  

  // If you know the name of the port used by the Arduino board, you
  // can specify it directly like this.
  //port = new Serial(this, "COM1", 9600);
}

void draw() {
  // draw a gradient from black to white
  for (int i = 0; i < 256; i++) {
    stroke(i);
    line(i, 0, i, 150);
  }
  
  stroke(255,0,0);
  line(0,height/3,width,height/3);
  line(0,height*2/3, width, height *2/3);
  maintain();
  
}

// lets the arduino know that I'm still here
void maintain() {
  if (millis() - time > 500) {
    port.write(maintain);
    port.write(0);
    time = millis();
  }
}

// send some data to the arduino
void mouseClicked() {
  time = millis();
  if (mouseY > height*2/3) {
    port.write(led2);
    println("led2");
  }
  else if (mouseY > height/3)  {
    port.write(led1);
    println("led1");
  }
  else {
    port.write(servo);
    println("servo");
  }
  port.write(mouseX);
}
