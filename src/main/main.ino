
#include <ZumoShield.h>


// these might need to be tuned for different motor types
#define TURN_SPEED      275
#define FORWARD_SPEED     200
#define FORWARD_DURATION 2850 // ms
#define TURN_LEFT_DURATION     245 // ms
#define TURN_RIGHT_DURATION     246 


void setup(){  
}

void loop()
{
  // put your main code here, to run repeatedly:
    turnLeft();
    delay(1000);
    turnRight();
    delay(1000);
    goForward();
    delay(1000);
}

// Functions move the bot
/*
goForward,
This function makes the bot go forward for 50 CM. 
*/
void goForward(){
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    delay(FORWARD_DURATION);
    stop();
}
/*
goBackward,
This function makes the bot go backward for 50 CM. 
*/
void goBackward(){
    motors.setSpeeds(-FORWARD_SPEED, -FORWARD_SPEED);
    delay(FORWARD_DURATION);
    stop(); 
}
/*
stop,
This function makes the bot stop. 
*/
void stop(){
    motors.setSpeeds(0, 0);
    delay(4000);
}
/*
turnLeft,
This function makes the bot rotate left in a 90° angle. 
*/
void turnLeft(){
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_LEFT_DURATION);
    
    stop();
}
/*
turnRight,
This function makes the bot rotate right in a 90° angle. 
*/
void turnRight(){
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_RIGHT_DURATION);
    
    stop();
}



