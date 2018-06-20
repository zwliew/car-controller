#define BAUD_RATE 9600
#define MAX_SPEED 255
#define LOOP_DELAY_MS 3

//For IR sensor
#define IR_L A0
#define IR_C A1
#define OFF_LINE 0
#define ON_LINE 1000
#define RANGE 20

//For wheels
#include "UCMotor.h"
#define FORWARD 2
#define BACKWARD 1

//For turns
#define LEFT 1
#define RIGHT 0
#define FRONT -1

//For UltraSound
#define US_PING 12
#define US_ECHO 13

//For defining motors
UC_DCMotor motorRF(3, MOTOR34_64KHZ); //Front Right
UC_DCMotor motorLF(4, MOTOR34_64KHZ); //Front Left
UC_DCMotor motorRB(1, MOTOR34_64KHZ); //Back Right
UC_DCMotor motorLB(2, MOTOR34_64KHZ); //Back Left

int prev_dir = FRONT;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);

  //IR sensor
  pinMode(IR_L,INPUT);
  pinMode(IR_C,INPUT);

  //UltraSound Sensor
  pinMode(US_PING, OUTPUT);
  pinMode(US_ECHO, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  const int left = analogRead(IR_L);
  const int centre = analogRead(IR_C);

  // Navigation on line
  if (onLine(left) && onLine(centre)) { //It is on the line
    moveForward(); //set the car to move forward
  } else if (onLine(left) && offLine(centre)) { //It is right of line
    moveLeft(); // set the car to move left
  } else if (offLine(left) && onLine(centre)) { //It is left of line
    moveRight(); // set the car to move right
  } else {
    prev_state();
  }
  delay(LOOP_DELAY_MS); //dont know if neededs
}




// Set all speeds of wheels to maximum
void setAllSpeedsMAX(){
  motorLF.setSpeed(MAX_SPEED);
  motorLB.setSpeed(MAX_SPEED);
  motorRF.setSpeed(MAX_SPEED);
  motorRB.setSpeed(MAX_SPEED);
}

// Define moving forward
void moveForward(){ //move the car at max speed forward
  setAllSpeedsMAX();
  motorLF.run(FORWARD);
  motorLB.run(FORWARD);
  motorRF.run(FORWARD);
  motorRB.run(FORWARD);
  prev_dir = FRONT; // set the prev direction to moving straight
}

// Defines Turn Left
void moveLeft(){ //make sure to include the reverse command for adjacent wheel
  setAllSpeedsMAX();
  motorLF.run(BACKWARD);
  motorLB.run(BACKWARD); //change this to nil
  motorRF.run(FORWARD);
  motorRB.run(FORWARD);
  prev_dir = LEFT; // set the prev direction to moving left
}

// Defines Turn Right
void moveRight(){ //make sure to include the reverse command for adjacent wheel
  setAllSpeedsMAX();
  motorLF.run(FORWARD);
  motorLB.run(FORWARD);
  motorRF.run(BACKWARD);
  motorRB.run(BACKWARD); //change this to nil
  prev_dir = RIGHT; // set the prev direction to moving right
}

void prev_state(){ //if no line sensed, move where?
  if (prev_dir == FRONT){ //if last detected on line, move forward
    moveForward();
  } else if (prev_dir == LEFT){ //if last detected left on line, left
    moveLeft();
  } else if (prev_dir == RIGHT){ //if last detcted right of line, right
    moveRight();
  }
}

bool onLine(int reading){
  return reading <= ON_LINE + RANGE && reading >= ON_LINE - RANGE;
}

bool offLine(int reading){
  return reading <= OFF_LINE + RANGE && reading >= OFF_LINE - RANGE;
}
