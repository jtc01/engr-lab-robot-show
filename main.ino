// C++ code
//
#include <Servo.h> // include the Servo library

Servo myservo; //create the servo object
int pos=0; //variable to store the servo position
bool bat=false;
const int runTrigger = 7;
const int runEcho = 6;
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
    bat=false;
  }
}

void detectOut(){
  pinMode(outPin, OUTPUT);
  digitalWrite(outPin, LOW);
  delayMicroseconds(2);
  digitalWrite(outPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(outPin, LOW);
  
  pinMode(outPin, INPUT);
  duration = pulseIn(outPin, HIGH);
  distanceCm = duration / 58.0;
  
  if (distanceCm < 30){
    Serial.println("Out!");
    bat=false;
  }
}