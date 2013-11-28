#include <NewPing.h>
#include <TimerOne.h>
#include "types.h"
#define leftMotorA 3
#define leftMotorB 4
#define rightMotorA 5
#define rightMotorB 6

#define trigger 7
#define echo 8
#define maxDist 200 //CM

NewPing sonar(trigger, echo, maxDist);
unsigned int dist;
turnDir roboDir;
void setup(){
  roboDir = tLeft;
  Timer1.initialize(0.5 * 1000000);
  Timer1.stop();
  Timer1.restart();
  Timer1.attachInterrupt( changeDir );
  pinMode(leftMotorA, OUTPUT);
  pinMode(leftMotorB, OUTPUT);
  pinMode(rightMotorA, OUTPUT);
  pinMode(rightMotorB, OUTPUT);
  Serial.begin(9600);
  dist = 0;
}

void loop(){

  //Get distance to obstacles with UltraSonic sensor
  dist = sonar.ping() / US_ROUNDTRIP_CM;
  Serial.print("Ping: ");
  Serial.print(dist);
  Serial.print("cm\n");

  // If obstacle in range
  if(dist > 0 && dist < 5) {
    //Stop current motors
    if(roboDir == tLeft) {
      //Stop left motor
      digitalWrite(leftMotorA, LOW);
      digitalWrite(leftMotorB, LOW); 
    } 
    else if (roboDir == tRight) {
      //Stop right motor
      digitalWrite(rightMotorA, LOW);
      digitalWrite(rightMotorB, LOW);
    }
    delay(10);
    //Stop forward movement timer
    Timer1.stop();

    //reverse
    digitalWrite(leftMotorA, LOW);
    digitalWrite(leftMotorB, HIGH);
    digitalWrite(rightMotorA, LOW);
    digitalWrite(rightMotorB, HIGH);

    delay(750);
    roboDir = tLeft;
    //Stop motors
    digitalWrite(leftMotorA, LOW);
    digitalWrite(leftMotorB, LOW);

    //Restart timer
    Timer1.restart();
    Timer1.attachInterrupt( changeDir );
     
  }  
}

void changeDir(){
  //If robot was turning left
  if(roboDir == tLeft){
    //set current dir to be right
    roboDir = tRight;

    //Stop right motor
    digitalWrite(rightMotorA, LOW);
    digitalWrite(rightMotorB, LOW);

    //Power left motor to turn right
    digitalWrite(leftMotorA, HIGH);
    digitalWrite(leftMotorB, LOW);

  } 
  else if (roboDir == tRight){
    //Set current dir to be left
    roboDir = tLeft;

    //Stop left motor
    digitalWrite(leftMotorA, LOW);
    digitalWrite(leftMotorB, LOW);

    //Power right motor to turn left
    digitalWrite(rightMotorA, HIGH);
    digitalWrite(rightMotorB, LOW);

  }
}

