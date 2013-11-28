#include <NewPing.h>
#include <TimerOne.h>
#include "types.h"
#define PIN_LMOTA 3
#define PIN_LMOTB 4
#define PIN_RMOTA 5
#define PIN_RMOTB 6

#define PIN_TRIGGER 7
#define PIN_ECHO 8
#define MAXDIST 200 //CM

NewPing sonar(PIN_TRIGGER, PIN_ECHO, MAXDIST);
unsigned int dist;
turnDir roboDir;
void setup(){
  roboDir = tLeft;
  Timer1.initialize(0.5 * 1000000);
  Timer1.stop();
  Timer1.restart();
  Timer1.attachInterrupt( changeDir );
  pinMode(PIN_LMOTA, OUTPUT);
  pinMode(PIN_LMOTB, OUTPUT);
  pinMode(PIN_RMOTA, OUTPUT);
  pinMode(PIN_RMOTB, OUTPUT);
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
      digitalWrite(PIN_LMOTA, LOW);
      digitalWrite(PIN_LMOTB, LOW); 
    } 
    else if (roboDir == tRight) {
      //Stop right motor
      digitalWrite(PIN_RMOTA, LOW);
      digitalWrite(PIN_RMOTB, LOW);
    }
    delay(10);
    //Stop forward movement timer
    Timer1.stop();

    //reverse
    digitalWrite(PIN_LMOTA, LOW);
    digitalWrite(PIN_LMOTB, HIGH);
    digitalWrite(PIN_RMOTA, LOW);
    digitalWrite(PIN_RMOTB, HIGH);

    delay(750);
    
    
    
    //If robot was turning left
    if(roboDir == tLeft){
      //Stop right motor
      digitalWrite(PIN_RMOTA, LOW);
      digitalWrite(PIN_RMOTB, LOW);
      //Power left motor to turn right
      digitalWrite(PIN_LMOTA, HIGH);
      digitalWrite(PIN_LMOTB, LOW);
      roboDir = tRight;
    } else if (roboDir == tRight) {
      //Stop left motor
      digitalWrite(PIN_LMOTA, LOW);
      digitalWrite(PIN_LMOTB, LOW);
      //Power right motor to turn left
      digitalWrite(PIN_RMOTA, HIGH);
      digitalWrite(PIN_RMOTB, LOW);
      roboDir = tLeft;
    }
     delay(500);
    
    
    //Stop motors
    digitalWrite(PIN_LMOTA, LOW);
    digitalWrite(PIN_LMOTB, LOW);
    

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
    digitalWrite(PIN_RMOTA, LOW);
    digitalWrite(PIN_RMOTB, LOW);

    //Power left motor to turn right
    digitalWrite(PIN_LMOTA, HIGH);
    digitalWrite(PIN_LMOTB, LOW);

  } 
  else if (roboDir == tRight){
    //Set current dir to be left
    roboDir = tLeft;

    //Stop left motor
    digitalWrite(PIN_LMOTA, LOW);
    digitalWrite(PIN_LMOTB, LOW);

    //Power right motor to turn left
    digitalWrite(PIN_RMOTA, HIGH);
    digitalWrite(PIN_RMOTB, LOW);

  }
}

