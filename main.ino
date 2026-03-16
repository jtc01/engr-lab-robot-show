// C++ code
//
#include <Servo.h> // include the Servo library

#define NOTE_G4  392
#define NOTE_C5  523
#define NOTE_E5  659
#define NOTE_G5  784
#define NOTE_B4  494
#define NOTE_AS4 466
#define NOTE_A4  440
#define NOTE_GS4 415


Servo bat; //create the servo object
Servo pitcher;
int pos=0; //variable to store the servo position
int state = 0;
const int runTrigger = 7;
const int runEcho = 6;
const int outTrigger = 10;
const int outEcho = 9;
const int buzzer = 13;
long duration;
float distanceCm;

int chargeMelody[]    = { NOTE_G4, NOTE_C5, NOTE_E5, NOTE_G5, NOTE_E5, NOTE_G5 };
int chargeDurations[] = { 150,     150,     150,      300,     150,     500     };
int tromboneMelody[]    = { NOTE_B4, NOTE_AS4, NOTE_A4, NOTE_GS4 };
int tromboneDurations[] = { 300,     300,      300,      900      };



int read;
void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT);
  pitcher.attach(4);//Attach the pitcher motor to 4
  pitcher.write(0);
  bat.attach(3);
  bat.write(0);
  pinMode(runTrigger, OUTPUT);
  pinMode(runEcho, INPUT);
}

void loop()
{
  //Read button press
  read=digitalRead(2);
  //swing bat if button pressed and its not swinging already
  if (read == 1 && state == 0){
    pitch();
  }
  if (read == 1 && state == 1){
    swing();
  }
  if (state == 2) {
    detectRun();
    detectOut();
    if (read == 1) {
      state = 0;
    }
  }
}

void pitch()
{
  state = 1;
  for (int i=0; i<90; i++){
    pitcher.write(i);
    delay(10);
  }
  delay(500);
  pitcher.write(0);
}

void swing() {
  state = 2;
  for (int i=0; i<180; i++){
    bat.write(i);
    delay(1);
  }
  for (int i=180; i>=0; i--){
    bat.write(i);
    delay(10);
  }
}
  
void detectRun() {
  digitalWrite(runTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(runTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(runTrigger, LOW);
  
  duration = pulseIn(runEcho, HIGH);
  distanceCm = duration / 58.0;
  
  if (distanceCm < 30){
    Serial.println("Home Run!");
    charge();
    delay(500);
    bat=false;
  }
}

void detectOut() {
  digitalWrite(outTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(outTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(outTrigger, LOW);
  
  duration = pulseIn(outEcho, HIGH);
  distanceCm = duration / 58.0;
  
  if (distanceCm < 30){
    Serial.println("Out!");
    trombone();
    bat=false;
  }
}

void charge(){
  for (int i = 0; i < 6; i++) {
    tone(buzzer, chargeMelody[i], chargeDurations[i]);
    delay(chargeDurations[i] + 30);
  }
  noTone(buzzer);
}

void trombone(){
  for (int i = 0; i < 4; i++) {
    tone(buzzer, tromboneMelody[i], tromboneDurations[i]);
    delay(tromboneDurations[i] + 30);
  }
  noTone(buzzer);
}