// C++ code
//
#include <Servo.h> // include the Servo library

Servo myservo; //create the servo object
int pos=0; //variable to store the servo position
bool bat=false;
const int runTrigger = 7;
const int runEcho = 6;
const int outTrigger = 10;
const int outEcho = 9;
const int buzzer = 13;
long duration;
float distanceCm;



int read;
void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT);
  myservo.attach(3);
  myservo.write(0);
  pinMode(runTrigger, OUTPUT);
  pinMode(runEcho, INPUT);
}

void loop()
{
  //Read button press
  read=digitalRead(2);
  //swing bat if button pressed and its not swinging already
  if (read == 1 && bat==false){
    swing();
  }
  if (bat == true) {
    detectRun();
    detectOut();
  }
}

void swing() {
  bat=true;
  for (int i=0; i<180; i++){
    myservo.write(i);
    delay(1);
  }
  for (int i=180; i>=0; i--){
    myservo.write(i);
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
    tone(buzzer, 1000);
    delay(500);

    noTone(buzzer);
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
    bat=false;
  }
}