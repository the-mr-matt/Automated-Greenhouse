//----LIBRARIES----
#include <Arduino.h>
#include "A4988.h"

//----CONFIG----
#define MOTOR_STEPS 200
#define DIR 8
#define STEP 7

#define SEN1 2

A4988 stepper(MOTOR_STEPS, DIR, STEP);

void setup()
{
  //pins
  pinMode(SEN1, INPUT);
  pinMode(13, OUTPUT);

  //init serial
  Serial.begin(9600);

  //init motor
  //30RPM, Microstepping 1 (full step mode)
  stepper.begin(30, 1);

  //init position
  digitalWrite(13, HIGH);
//  int sen1 = 0;
//  while(sen1 == LOW)
//  {
//    sen1 = digitalRead(SEN1);
//    
//    stepper.rotate(1.8);
//    delay(25);
//  }
  digitalWrite(13, LOW);
}

void loop()
{
  stepper.rotate(360);

  delay(1000);

  stepper.rotate(-360);

  delay(1000);
}
