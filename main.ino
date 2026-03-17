// C++ code
//
#include <Servo.h> // include the Servo library

//Notes for speaker to play
#define NOTE_G4  392
#define NOTE_C5  523
#define NOTE_E5  659
#define NOTE_G5  784
#define NOTE_B4  494
#define NOTE_AS4 466
#define NOTE_A4  440
#define NOTE_GS4 415


Servo bat; //create the servo objects
Servo pitcher; 
int pos=0; //variable to store the servo position

int state = 0; //int variable to store game state. 0: ready to play. 1: ball pitched. 2: bat swung

// sensor pins
const int runTrigger = 7;
const int runEcho = 6;
const int outTrigger = 10;
const int outEcho = 9;

//buzzer pins and variables
const int buzzer = 13;
long duration;
float distanceCm;

//buzzer melody notes and durations
int chargeMelody[]    = { NOTE_G4, NOTE_C5, NOTE_E5, NOTE_G5, NOTE_E5, NOTE_G5 };
int chargeDurations[] = { 150,     150,     150,      300,     150,     500     };
int tromboneMelody[]    = { NOTE_B4, NOTE_AS4, NOTE_A4, NOTE_GS4 };
int tromboneDurations[] = { 300,     300,      300,      900      };

//variable to read input from button
int read;

void setup()
{
  Serial.begin(9600);
  //set pinmodes
  pinMode(2, INPUT);
  pinMode(runTrigger, OUTPUT);
  pinMode(runEcho, INPUT);
  pinMode(outTrigger, OUTPUT);
  pinMode(outEcho, INPUT);

  //attach and write servos to default positions
  pitcher.attach(4);
  pitcher.write(90);
  bat.attach(3);
  bat.write(0);
}

void loop()
{
  //Read button press
  read=digitalRead(2);

  //WHEN BUTTON IS PRESSED

  //pitch the ball if state is 0
  if (read == 1 && state == 0){
    pitch();
  }

  //swing bat if state is 1
  if (read == 1 && state == 1){
    swing();
  }

  //reset if state is 2
  if (read == 1 && state == 2){
    trombone();
    state=0;
  }

  //whenever state is 2, repeatedly detect for outs or runs
  if (state == 2) {
    detectRun();
    detectOut();
  }
}

void pitch()
{
  Serial.println("pitching!");

  tone(buzzer, NOTE_G4);
  
  state = 1;//update state
  delay(180);
  noTone(buzzer);

  pitcher.write(0);//push ball with servo
  delay(500);
  
  pitcher.write(90);//reset pitcher
  read=0;//reset read value
}

void swing() {
  tone(buzzer, NOTE_C5);
  Serial.println("crack!");

  state = 2;//update state
  bat.write(180);//swing bat with servo
  noTone(buzzer);

  //reset bat
  for (int i=180; i>=0; i--){
    bat.write(i);
    delay(10);
  }

  read=0;//reset read value
}
  
void detectRun() {
  digitalWrite(runTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(runTrigger, HIGH);//send out ultrasonic pulse
  delayMicroseconds(10);
  digitalWrite(runTrigger, LOW);
  
  duration = pulseIn(runEcho, HIGH);//read ultrasonic pulse
  distanceCm = duration / 58.0;//calculate distance
  

  if (distanceCm < 5){//if the ball passes over the win sensor
    Serial.println("Home Run!");
    charge();//play win sound
    delay(500);
    state = 0;//reset state
  }
}

void detectOut() {
  digitalWrite(outTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(outTrigger, HIGH);//send out ultrasonic pulse
  delayMicroseconds(10);
  digitalWrite(outTrigger, LOW);
  

  duration = pulseIn(outEcho, HIGH);//read ultrasonic pulse
  distanceCm = duration / 58.0;//calculate distance
  
  if (distanceCm < 5){//if ball passes over lose sensor
    Serial.println("Out!");
    trombone();//play lose sound
    state = 0;//reset state
  }
}

void charge() {//win sound
  for (int i = 0; i < 6; i++) {//play each note in chargeMelody for each duration in chargeDurations
    tone(buzzer, chargeMelody[i], chargeDurations[i]);
    delay(chargeDurations[i] + 30);
  }
  noTone(buzzer);
}

void trombone(){//lose sound
  for (int i = 0; i < 4; i++) {
    tone(buzzer, tromboneMelody[i], tromboneDurations[i]);//play each note in tromboneMelody for each duration in chargeDurations
    delay(tromboneDurations[i] + 30);
  }
  noTone(buzzer);
}