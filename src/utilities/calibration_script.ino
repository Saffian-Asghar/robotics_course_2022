#include <ZumoShield.h>
#define TURN_SPEED      275
#define FORWARD_SPEED     200
#define FORWARD_DURATION 2850 // ms
#define TURN_LEFT_DURATION     245 // ms
#define TURN_RIGHT_DURATION     246 

ZumoMotors motors;
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
turnLeft();
delay(1000);
turnRight();
delay(1000);
goForward();
delay(1000);

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
void stop(){
    motors.setSpeeds(0, 0);
    delay(2000);
}
void goForward(){
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    delay(FORWARD_DURATION);
    stop();
}