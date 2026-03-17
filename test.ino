#include <Arduino.h>
#include <Servo.h> // include the Servo library

const int runTrigger = 7;
const int runEcho = 6;
const int outTrigger = 10;
const int outEcho = 9;

int distanceCm;
long duration;

void setup() {
  Serial.begin(9600);
  pinMode(runTrigger, OUTPUT);
  pinMode(runEcho, INPUT);
  pinMode(outTrigger, OUTPUT);
  pinMode(outEcho, INPUT);
}

void loop() {
  //detectRun();
  detectOut();
}

void detectRun() {
  digitalWrite(runTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(runTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(runTrigger, LOW);
  
  duration = pulseIn(runEcho, HIGH);
  distanceCm = duration / 58.0;
  
  print(distanceCm);
  println(" run");
}

void detectOut() {
  digitalWrite(outTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(outTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(outTrigger, LOW);
  
  duration = pulseIn(outEcho, HIGH);
  distanceCm = duration / 58.0;
  
  print(distanceCm);
  println(" out");
}