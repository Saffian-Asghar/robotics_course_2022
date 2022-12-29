
#include <ZumoShield.h>


// these might need to be tuned for different motor types
#define TURN_SPEED      275
#define FORWARD_SPEED     200
#define FORWARD_DURATION 2850 // ms
#define TURN_LEFT_DURATION     245 // ms
#define TURN_RIGHT_DURATION     246 

#define MAP_ROWS 8
#define MAP_COLS 8


// pin for the sensor
int trigPin = 10;    // Trigger
int echoPin = 11;    // Echo

// We need to keep track of where our bot is facing
char orientation;
// Obstacle detection variables
long duration, cm; 

bool reachGoal = false;
ZumoMotors motors;
// Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12

char * pathToFollow;

int gridMap[MAP_ROWS][MAP_COLS] = {
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
};

/* A structure for each tile or spot on the grid */ 
struct spot {
    int col, row;
    int n_len;
    int f, g, h;
    int from;
    int * n; // pointer to array of neighbors if neighbors are in route
};
/* Index Array for valid tiles */
int ind[MAP_ROWS][MAP_COLS] = {
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1}
};



/* description of route between two nodes */
struct route {
    /* route has only one direction! */
    int x; /* index of spot in array of all spots of src of this route */
    int y; /* intex of spot in array of all spots od dst of this route */
    int d;
};

int found;
int pLen = 0;
int * path = NULL;
int cLen = 0;
int * closed = NULL;
int oLen = 1;
int * open = NULL;
int min, tempg;
int s;
int e;
int current;
int sLen = 0;
struct spot * spots = NULL;
int r_len = 0;
struct route * routes = NULL;
int count;
int success;

/**
  Arduino default setup function
*/
void setup(){  
    // uncomment if necessary to correct motor directions
    //motors.flipLeftMotor(true);
    //motors.flipRightMotor(true);
    orientation = 'n';
    Serial.begin(2000000);
    delay(5000);
    Serial.println("Building the grid...");
    //pinMode(LED, HIGH);
    //Define inputs and outputs for the ultrasonic sensor
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    open = (int*)calloc(oLen, sizeof(int));
    buildGrid();
    Serial.println("Grid DONE!");
    /* index of start spot */
    s =0;
    /* index of finish spot */
    e = sLen - 1;
    //Serial.println("Finding shortest Path...");

    //getShortestPath();
    //Serial.println("Shortest Path FOUND!");

    count = 0;
    success =0;
}

    // int count = 0;
// Arduino default loop function.
void loop()
{
    if(count < 1){
      Serial.println("finding path");
      getShortestPath();
      Serial.println("found path");
      count++;
    
    }

    // Serial.println("In loop");
    if(!found || success == 1){
        // stop the bot
        stop();
    }
    else{
      Serial.print("path cost is :");
    Serial.print(pLen); 
        for (int i = pLen - 1; i >= 0; i--) {
          Serial.print("(");
          Serial.print(spots[path[i]].row);
          Serial.print(",");
          Serial.print(spots[path[i]].col);
          Serial.print(")");
          Serial.println("");
        }
        success = driveBot();
    }
    Serial.print("success: ");
    Serial.println(success);
    Serial.print("found: ");
    Serial.println(found);

}


/**
  Builds the main grid for each valid tile

  0 is valid tile, 1 is obstacle

  initializing heuristics to some max value
*/

void buildGrid(){

    for (int i = 0; i < MAP_ROWS ; i++) {
        for (int j = 0; j < MAP_COLS ; j++) {
            if (!gridMap[i][j]) {
                ++sLen;
                spots = (struct spot *)realloc(spots, sLen * sizeof(struct spot));

                int t = sLen - 1;
                spots[t].col = j;
                spots[t].row = i;
                spots[t].from = -1;
                spots[t].g = 999;
                spots[t].n_len = 0;
                spots[t].n = NULL;
                ind[i][j] = t;

            }
        }
    }
}

/**
  Resets all variables 
*/
void resetPaths(){
    for (int i = 0; i < sLen; ++i) {
      free(spots[i].n);
    }
    free(spots);
    free(routes);
    free(path);
    free(open);
    free(closed);

    for (int i = 0; i < MAP_ROWS ; i++) {
        for (int j = 0; j < MAP_COLS ; j++) {
          ind[i][j] = -1;
        }
    }
    found = 0;
    pLen = 0;
    * path = NULL;
    cLen = 0;
    * closed = NULL;
    oLen = 1;
    * open = NULL;
    min = 0.0, tempg = 0.0;
    sLen = 0;
    spots = NULL;
    r_len = 0;
    routes = NULL;
    Serial.println("Done with the reset!");


}

/**
  Calculates the shortest path using A star algorithm

  0 is valid tile, 1 is obstacle

  initializing heuristics to some max value
*/

void getShortestPath(){
    for (int i = 0; i < MAP_ROWS ; i++) {
        for (int j = 0; j < MAP_COLS ; j++) {
             Serial.print(" ");
             Serial.print(gridMap[i][j]);
             Serial.print(" ");
        }
             Serial.println("");
    }


    for (int i = 0; i < sLen; i++) {
        spots[i].h = abs(spots[e].row - spots[i].row) + abs(spots[e].col - spots[i].col);
    }

    for (int i = 0; i < MAP_ROWS ; i++) {
        for (int j = 0; j < MAP_COLS ; j++) {
            if (ind[i][j] >= 0) {
                for (int k = i - 1; k <= i + 1; k++) {
                    for (int l = j - 1; l <= j + 1; l++) {
                        if ((k != i) && (l != j) || ((k < 0) || (l < 0) || (k > MAP_ROWS -1) || (l > MAP_COLS - 1))) {
                            continue;
                        }
                        if (ind[k][l] >= 0) {
                            ++r_len;


                            routes = (struct route *)realloc(routes, r_len * sizeof(struct route));
                            int t = r_len - 1;

                            routes[t].x = ind[i][j];
                            routes[t].y = ind[k][l];
                            routes[t].d = abs(spots[routes[t].y].row - spots[routes[t].x].row) + abs(spots[routes[t].y].col - spots[routes[t].x].col);
                            
                            ++spots[routes[t].x].n_len;
                            spots[routes[t].x].n = (int*)realloc(spots[routes[t].x].n, spots[routes[t].x].n_len * sizeof(int));
                            spots[routes[t].x].n[spots[routes[t].x].n_len - 1] = t;
                        }
                    }
                }
            }
        }
    }

    open[0] = s;
    spots[s].g = 0;
    spots[s].f = spots[s].g + spots[s].h;
    found = 0;

    while (oLen && !found) {
        min = 999;

        for (int i = 0; i < oLen; i++) {
            if (spots[open[i]].f < min) {
                current = open[i];
                min = spots[open[i]].f;
            }
        }

        if (current == e) {
            found = 1;

            ++pLen;
            path = (int*)realloc(path, pLen * sizeof(int));
            path[pLen - 1] = current;
            while (spots[current].from >= 0) {
                current = spots[current].from;
                ++pLen;
                path = (int*)realloc(path, pLen * sizeof(int));
                path[pLen - 1] = current;
            }
        }

        for (int i = 0; i < oLen; i++) {
            if (open[i] == current) {
                if (i != (oLen - 1)) {
                    for (int j = i; j < (oLen - 1); j++) {
                        open[j] = open[j + 1];
                    }
                }
                --oLen;
                open = (int*)realloc(open, oLen * sizeof(int));
                break;
            }
        }

        ++cLen;
        closed = (int*)realloc(closed, cLen * sizeof(int));
        closed[cLen - 1] = current;

        int b;

        for (int i = 0; i < spots[current].n_len; i++) {
            b = 0;

            for (int j = 0; j < cLen; j++) {
                if (routes[spots[current].n[i]].y == closed[j]) {
                    b = 1;
                }
            }

            if (b) {
                continue;
            }

            tempg = spots[current].g + routes[spots[current].n[i]].d;

            b = 1;

            if (oLen > 0) {
                for (int j = 0; j < oLen; j++) {
                    if (routes[spots[current].n[i]].y == open[j]) {
                        b = 0;
                    }
                }
            }

            if (b || (tempg < spots[routes[spots[current].n[i]].y].g)) {
                spots[routes[spots[current].n[i]].y].from = current;
                spots[routes[spots[current].n[i]].y].g = tempg;
                spots[routes[spots[current].n[i]].y].f = spots[routes[spots[current].n[i]].y].g + spots[routes[spots[current].n[i]].y].h;

                if (b) {
                    ++oLen;
                    open = (int*)realloc(open, oLen * sizeof(int));
                    open[oLen - 1] = routes[spots[current].n[i]].y;
                }
            }
        }
    }


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

/*
driveBot,
Drives the bot on the path calculated
* @return int : status of our followed path 
*/
int driveBot(){
    for(int i =  pLen -1 ; i >= 0 ; i--){
        int curX =spots[path[i]].col; 
        int curY= spots[path[i]].row;
        int nextX;
        int nextY;
        if(i == 0){
            nextX =curX; 
            nextY= curY;  
        }
        else{
            nextX =spots[path[i-1]].col; 
            nextY= spots[path[i-1]].row;  
        }

        success = runPath(curX,curY,nextX,nextY);
        if(success == 3){
            return 1;
            // reached goal
        }
        else if(success == 2){
            return 2;
        }
            
    }
    return 1;
}
/*
runPath,
This function makes the robot go to the next desired spot/tile.
@param curX: x current position.
@param curY: y current position.
@param nextX: x next square position.
@param nextY: y next square position.

@return int : status of our followed path (3 for goal reached, 2 if obstacle detected, 1 for no direction )

*/
int runPath(int curX,int curY,int nextX,int nextY ){
    
    int returnValue = 1;
 
    Serial.print("Current (");
    Serial.print(curY);
    Serial.print(" , ");
    Serial.print(curX);
    Serial.print(" ) ");
    Serial.print("Next (");
    Serial.print(nextY);
    Serial.print(" , ");
    Serial.print(nextX);
    Serial.println(" )");
    Serial.print("Current Orientation: ");
    Serial.println(orientation);

    char direction = goNext(curX, curY, nextX, nextY);
    if(direction == 'e'){
        return 3;
    }
        Serial.print("Next Direction: ");

    if (direction == 'r'){
        Serial.println("Right");
        turnRight();
    }
    else if (direction == 'l'){
        Serial.println("Left");
        turnLeft();
    }
    if (! detectObstacle()){
        if (direction != 'b'){
        Serial.println("Forward");
            goForward();
        }
        else{
        Serial.println("Backward");
            goBackward();
        }
    }
    else{
        Serial.println("Obstacle detected");
        Serial.print("Obstacle placed at ( ");
        Serial.print(nextY);
        Serial.print(" , ");
        Serial.print(nextX);
        Serial.println(" ) ");
        resetPaths();
        open = (int*)calloc(oLen, sizeof(int));
        gridMap[nextY][nextX] = 1;
        buildGrid();
        s = ind[curY][curX];
        
        e = sLen - 1;


        getShortestPath();
        returnValue = 2;
    }

return returnValue;

}


