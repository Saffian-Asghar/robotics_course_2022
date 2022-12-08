#include <Wire.h>
#include <ZumoShield.h>
#include <arduino-timer.h>
#define LED 13

// these might need to be tuned for different motor types
#define TURN_SPEED      275
#define FORWARD_SPEED     100
#define FORWARD_DURATION 4150 // ms
#define TURN_LEFT_DURATION     307 // ms
#define TURN_RIGHT_DURATION     299 


int trigPin = 10;    // Trigger
int echoPin = 11;    // Echo
long duration, cm;

ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12


void goForward(){
  motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  delay(FORWARD_DURATION);
  stop();
  
  }

  void goBackward(){
  motors.setSpeeds(-FORWARD_SPEED, -FORWARD_SPEED);
  delay(FORWARD_DURATION);
  stop();
  }

void stop(){
  motors.setSpeeds(0, 0);
  delay(4000);
  }

void turnLeft(){
   motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
   delay(TURN_LEFT_DURATION);
   stop();
}

void turnRight(){
   motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
   delay(TURN_RIGHT_DURATION);
   stop();
}
void setup()
{  
  // uncomment if necessary to correct motor directions
  //motors.flipLeftMotor(true);
  //motors.flipRightMotor(true);
  pinMode(LED, HIGH);
  //Define inputs and outputs for the ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  }

void loop()
{

  // goForward();
   // goBackward();
 //  turnLeft();
//   turnRight();
   getDistance();
}

void getDistance()
{
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  
  
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  delay(250);
  }
