
#include <ZumoShield.h>


// these might need to be tuned for different motor types
#define TURN_SPEED      275
#define FORWARD_SPEED     200
#define FORWARD_DURATION 2850 // ms
#define TURN_LEFT_DURATION     245 // ms
#define TURN_RIGHT_DURATION     246 


// pin for the sensor
int trigPin = 10;    // Trigger
int echoPin = 11;    // Echo

// We need to keep track of where our bot is facing
char orientation;
// Obstacle detection variables
long duration, cm; 

ZumoMotors motors;
// Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12


/**
  Arduino default setup function
*/
void setup(){  
    orientation = 'n';
    //Define inputs and outputs for the ultrasonic sensor
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

// Arduino default loop function.
void loop()
{

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
    if (orientation == 'n'){
    orientation = 'o';
    }
    else if (orientation == 'o'){orientation = 's';}
    else if(orientation == 's'){orientation = 'e';}
    else if(orientation == 'e'){orientation = 'n';}
    stop();
}
/*
turnRight,
This function makes the bot rotate right in a 90° angle. 
*/
void turnRight(){
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_RIGHT_DURATION);
    if (orientation == 'n'){
    orientation = 'e';
    }
    else if (orientation == 'e'){orientation = 's';}
    else if(orientation == 's'){orientation = 'o';}
    else if(orientation == 'o'){orientation = 'n';}
    stop();
}

/*
detectObstacle,
This function uses the ultrasonic sensor to detect if there is an obstacle on the path.
* @return : bool, return a bollean indicating if an obstacle is found in the comming 50 cm 
*/
bool detectObstacle(){
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
    Serial.print("US Dist: ");
    Serial.println(cm);
    delay(100);
    if (cm > 50){return false; }
    else{return true;}
    // return false;
}

/*
goNext,
This function makes the robot go from one square to the other.
@param x: x current position.
@param y: y current position.
@param w: x next square position.
@param z: y next square position.
*/
char goNext(int x,int y,int w,int z){

    if (x == w && y < z){
        if (orientation == 'n'){
        return 'f';
        }
    else if (orientation == 'o'){return 'r';}
    else if(orientation == 's'){return 'b';}
    else if(orientation == 'e'){return 'l';}
    }
    if (x == w && y > z){
        if (orientation == 'n'){
        return 'b';}
    else if (orientation == 'o'){return 'l';}
    else if(orientation == 's'){return 'f';}
    else if(orientation == 'e'){return 'r';}
    }

    if (x < w && y == z){
        if (orientation == 'n'){
        return 'l';}
    else if (orientation == 'o'){return 'f';}
    else if(orientation == 's'){return 'r';}
    else if(orientation == 'e'){return 'b';}
    }
    if (x > w && y == z){
        if (orientation == 'n'){
        return 'r';}
    else if (orientation == 'o'){return 'b';}
    else if(orientation == 's'){return 'l';}
    else if(orientation == 'e'){return 'f';}
    }
    if ((x == w && y == z) || (x !=w && y != z)){
        return 'e';
    }

}
