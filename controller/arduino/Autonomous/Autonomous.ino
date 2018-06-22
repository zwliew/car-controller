#define BAUD_RATE 9600
#define MAX_SPEED 255
#define LOOP_DELAY_MS 3
#define TURN_DELAY_MS 225

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
unsigned int num_us_detected = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);

  //IR sensor
  pinMode(IR_L,INPUT);
  pinMode(IR_C,INPUT);

  //UltraSound Sensor
  pinMode(US_PING, OUTPUT);
  pinMode(US_ECHO, INPUT);

  stop();
  setSpeed(MAX_SPEED);
}

void loop() {
  moveForward();
  return;
  int left = 0;
  int ctr = 0;
  bool leftOnLine = false;
  bool ctrOnLine = false;

  switch (num_us_detected) {
    case 0:
      if (nearObstacle(false)) {
        num_us_detected = 1;
      }

      left = analogRead(IR_L);
      ctr = analogRead(IR_C);
      leftOnLine = onLine(left);
      ctrOnLine = onLine(ctr);

      if (leftOnLine && ctrOnLine) {
        moveForward();
      } else if (leftOnLine) {
        turnLeft();
      } else if (ctrOnLine) {
        turnRight();
      } else {
        turnLeft();
        delay(TURN_DELAY_MS);
        left = analogRead(IR_L);
        ctr = analogRead(IR_C);
        if (onLine(left) || onLine(ctr)) {
          moveForward();
        } else {
          turnRight();
          delay(TURN_DELAY_MS * 2);
          left = analogRead(IR_L);
          ctr = analogRead(IR_C);
          if (onLine(left) || onLine(ctr)) {
            moveForward();
          }
        }
      }
      delay(LOOP_DELAY_MS);
      break;
    case 1:
      static int case_1_turns = 0;

      if (case_1_turns == 2) {
        if (nearObstacle(true)) {
          num_us_detected = 2;
        }

        left = analogRead(IR_L);
        ctr = analogRead(IR_C);
        leftOnLine = onLine(left);
        ctrOnLine = onLine(ctr);

        if (leftOnLine && ctrOnLine) {
          moveForward();
        } else if (leftOnLine) {
          turnLeft();
        } else if (ctrOnLine) {
          turnRight();
        } else {
          prev_state();
        }
        delay(LOOP_DELAY_MS);
        break;
      }

      left = analogRead(IR_L);
      ctr = analogRead(IR_C);
      leftOnLine = onLine(left);
      ctrOnLine = onLine(ctr);

      if (leftOnLine && ctrOnLine) {
        moveForward();
      } else if (leftOnLine) {
        turnLeft();
      } else if (ctrOnLine) {
        turnRight();
      } else {
        turnLeft();
        delay(TURN_DELAY_MS);
        left = analogRead(IR_L);
        ctr = analogRead(IR_C);
        if (onLine(left) || onLine(ctr)) {
          moveForward();
          case_1_turns++;
        } else {
          turnRight();
          delay(TURN_DELAY_MS * 2);
          left = analogRead(IR_L);
          ctr = analogRead(IR_C);
          if (onLine(left) || onLine(ctr)) {
            moveForward();
            case_1_turns++;
          }
        }
      }
      delay(LOOP_DELAY_MS);
      break;
    case 2:
      static int case_2_turns = 0;

      if (nearObstacle(false) && case_2_turns == 1) {
        num_us_detected = 3;
        turnLeft();
        delay(TURN_DELAY_MS);
        moveForward();
        delay(750);
        turnRight();
        delay(TURN_DELAY_MS);
        moveForward();
        break;
      }

      left = analogRead(IR_L);
      ctr = analogRead(IR_C);
      leftOnLine = onLine(left);
      ctrOnLine = onLine(ctr);

      if (leftOnLine && ctrOnLine) {
        moveForward();
      } else if (leftOnLine) {
        turnLeft();
      } else if (ctrOnLine) {
        turnRight();
      } else {
        turnLeft();
        delay(TURN_DELAY_MS);
        left = analogRead(IR_L);
        ctr = analogRead(IR_C);
        if (onLine(left) || onLine(ctr)) {
          moveForward();
          case_2_turns++;
        }
      }
      break;
    case 3:
      static int case_3_turns = 0;
      left = analogRead(IR_L);
      ctr = analogRead(IR_C);
      leftOnLine = onLine(left);
      ctrOnLine = onLine(ctr);

      if (leftOnLine && ctrOnLine) {
        moveForward();
      } else if (leftOnLine) {
        turnLeft();
      } else if (ctrOnLine) {
        turnRight();
      } else {
        if (case_3_turns == 0) {
          turnRight();
          delay(TURN_DELAY_MS);
          left = analogRead(IR_L);
          ctr = analogRead(IR_C);
          if (onLine(left) || onLine(ctr)) {
            moveForward();
          }
          case_3_turns = 1;
        } else if (case_3_turns == 1) {
          turnLeft();
          delay(TURN_DELAY_MS);
          left = analogRead(IR_L);
          ctr = analogRead(IR_C);
          if (onLine(left) || onLine(ctr)) {
            moveForward();
          }
          case_3_turns = 2;
        } else if (case_3_turns == 2) {
          moveForward();
          if (nearObstacle(false)) {
            num_us_detected = 4;
            turnLeft();
            delay(TURN_DELAY_MS);
            moveForward();
            prev_dir = FRONT;
          }
        }
      }
      break;
    case 4:
      left = analogRead(IR_L);
      ctr = analogRead(IR_C);
      leftOnLine = onLine(left);
      ctrOnLine = onLine(ctr);

      if (leftOnLine && ctrOnLine) {
        moveForward();
      } else if (leftOnLine) {
        turnLeft();
      } else if (ctrOnLine) {
        turnRight();
      } else {
        prev_state();
      }
      delay(LOOP_DELAY_MS);
      break;
    default:
      break;
  }
}

bool nearObstacle(bool longer) {
  digitalWrite(US_PING, HIGH);
  delayMicroseconds(5);
  digitalWrite(US_PING, LOW);
  const int duration = pulseIn(US_PING, HIGH);
  return duration * 0.01715 < (longer ? 30 : 20);
}

void setSpeed(const int speed) {
  motorLF.setSpeed(speed);
  motorLB.setSpeed(speed);
  motorRF.setSpeed(speed);
  motorRB.setSpeed(speed);
}

// Define moving forward
void moveForward() { //move the car at max speed forward
  motorLF.run(FORWARD);
  motorLB.run(FORWARD);
  motorRF.run(FORWARD);
  motorRB.run(FORWARD);
  prev_dir = FRONT; // set the prev direction to moving straight
}

// Defines Turn Left
void turnLeft() { //make sure to include the reverse command for adjacent wheel
  motorLF.run(BACKWARD);
  motorLB.run(BACKWARD); //change this to nil
  motorRF.run(FORWARD);
  motorRB.run(FORWARD);
  prev_dir = LEFT; // set the prev direction to moving left
}

// Defines Turn Right
void turnRight() { //make sure to include the reverse command for adjacent wheel
  motorLF.run(FORWARD);
  motorLB.run(FORWARD);
  motorRF.run(BACKWARD);
  motorRB.run(BACKWARD); //change this to nil
  prev_dir = RIGHT; // set the prev direction to moving right
}

void stop() {
  motorLF.run(STOP);
  motorLB.run(STOP);
  motorRF.run(STOP);
  motorRB.run(STOP);
}

void prev_state() { //if no line sensed, move where?
  if (prev_dir == FRONT){ //if last detected on line, move forward
    moveForward();
  } else if (prev_dir == LEFT){ //if last detected left on line, left
    turnLeft();
  } else if (prev_dir == RIGHT){ //if last detcted right of line, right
    turnRight();
  }
}

bool onLine(int reading) {
  return reading <= ON_LINE + RANGE && reading >= ON_LINE - RANGE;
}
