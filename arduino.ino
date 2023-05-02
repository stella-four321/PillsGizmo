#include <Servo.h>
#include <stdio.h>

// defines Servos
Servo gripperServo;
Servo armServo;
Servo wristServo;
Servo gripper;

void setup() {
  gripperServo.attach(6, 100, 2300);
  armServo.attach(10, 100, 2500);
  wristServo.attach(9, 100, 2300);
  Serial.begin(9600); // Starts the serial communication 
}

void loop() {

  gripperServo.write(180);
  armServo.write(160);
  wristServo.write(160);

  if (Serial.available()) {
    int option = Serial.read();  
    switch (option) { 
      case 1:
        grab();
        delay(2000);
        deliver(); 
        break;
      default: 
        break;
    }
  }
}

void grab() 
{
  wristServo.write(180);
  delay(1000);
  armServo.write(180);
  delay(1000);
  gripperServo.write(0);
  delay(1000);
}

void deliver()
{
  armServo.write(0);
  delay(5000);
  wristServo.write(150);
  delay(5000);
}
