#define BAUD_RATE 115200

//For wheels
#include "UCMotor.h"
#define FORWARD 2
#define BACKWARD 1

int max_speed = 0;
String received_cmd = "";

//For defining motors
UC_DCMotor motorRF(3, MOTOR34_64KHZ); //Front Right
UC_DCMotor motorLF(4, MOTOR34_64KHZ); //Front Left
UC_DCMotor motorRB(1, MOTOR34_64KHZ); //Back Right
UC_DCMotor motorLB(2, MOTOR34_64KHZ); //Back Left

void setup() {
  Serial.begin(BAUD_RATE);
}

void loop() {
  if (!Serial.available()) return;

  const char cur_char = Serial.read();
  received_cmd.concat(cur_char);
  if (cur_char != ';') return;

  if (received_cmd.charAt(0) == 's') {
    const int speed = received_cmd.substring(2).toInt();
    motorLF.setSpeed(speed);
    motorLB.setSpeed(speed);
    motorRF.setSpeed(speed);
    motorRB.setSpeed(speed);
  } else if (received_cmd == "d_f") {
    moveForward();
  } else if (received_cmd == "d_b") {
    moveBackward();
  } else if (received_cmd == "d_l") {
    moveLeft();
  } else if (received_cmd == "d_r") {
    moveRight();
  }

  received_cmd = "";
}

// Define moving forward
void moveForward() { //move the car at max speed forward
  motorLF.run(FORWARD);
  motorLB.run(FORWARD);
  motorRF.run(FORWARD);
  motorRB.run(FORWARD);
}

void moveBackward() {
  motorLF.run(BACKWARD);
  motorLB.run(BACKWARD);
  motorRF.run(BACKWARD);
  motorRB.run(BACKWARD);
}

// Defines Turn Left
void moveLeft(){ //make sure to include the reverse command for adjacent wheel
  motorLF.run(BACKWARD);
  motorLB.run(BACKWARD); //change this to nil
  motorRF.run(FORWARD);
  motorRB.run(FORWARD);
}

// Defines Turn Right
void moveRight(){ //make sure to include the reverse command for adjacent wheel
  motorLF.run(FORWARD);
  motorLB.run(FORWARD);
  motorRF.run(BACKWARD);
  motorRB.run(BACKWARD); //change this to nil
}

bool onLine(int reading){
  return reading <= ON_LINE + RANGE && reading >= ON_LINE - RANGE;
}

bool offLine(int reading){
  return reading <= OFF_LINE + RANGE && reading >= OFF_LINE - RANGE;
}
