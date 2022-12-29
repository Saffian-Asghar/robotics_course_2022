
#include <ZumoShield.h>


// these might need to be tuned for different motor types
#define TURN_SPEED      275
#define FORWARD_SPEED     200
#define FORWARD_DURATION 2850 // ms
#define TURN_LEFT_DURATION     245 // ms
#define TURN_RIGHT_DURATION     246 


// We need to keep track of where our bot is facing
char orientation;

ZumoMotors motors;


/**
  Arduino default setup function
*/
void setup(){  

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

