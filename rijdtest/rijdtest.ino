#include <NewPing.h>

#define segA 8
#define segB 9
#define segC 10
#define segD A0
#define segE A1
#define segF A2
#define segG A3

#define segU1 A4
#define segU2 A5

unsigned long correction;
int disp1, disp2;
bool displayStatus;

const int echoPin = 1;
const int trigPin = 0;
int MAX_DISTANCE = 100;  // Maximum distance we want to measure (in centimeters).

NewPing sonar(trigPin, echoPin, MAX_DISTANCE);  // NewPing setup of pins and maximum distance.

int directionPinR = 12;  //RECHTER MOTOR
int pwmPinR = 3;
//int brakePinR = 9;

int directionPinL = 13;  // LINKER MOTOR
int pwmPinL = 11;
//int brakePinL = 8;

String lineposition = "";
int starttijd = millis();

// int speed = 45;//stopcontact
int speed = 70;  //batterij
int defaultspeed = 75;

long duration;
int distance;
int tempDistance;
// sensors
#define s1 2
#define s2 4
#define s3 5
#define s4 6
#define s5 7

//boolean to switch direction
bool directionStateR;
bool directionStateL;
bool isGestart;

void setup() {
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);
  pinMode(segU1, OUTPUT);
  pinMode(segU2, OUTPUT);
  //define pins
  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);   // Sets the echoPin as an Input
  // motor Rechts
  pinMode(directionPinR, OUTPUT);
  pinMode(pwmPinR, OUTPUT);
  //pinMode(brakePinR, OUTPUT);
  // motor links
  pinMode(directionPinL, OUTPUT);
  pinMode(pwmPinL, OUTPUT);
  //pinMode(brakePinL, OUTPUT);

  //TCCR2B = TCCR2B & B11111000 | B00000110;  // for PWM frequency of 122.55 Hz
  TCCR2B = TCCR2B & B11111000 | B00000111;  // for PWM frequency of 30.64 Hz
  // sensors
  pinMode(s1, INPUT);
  pinMode(s2, INPUT);
  pinMode(s3, INPUT);
  pinMode(s4, INPUT);
  pinMode(s5, INPUT);
}

void loop() {
  tempDistance = sonar.ping_cm();
  // Prints the distance on the Serial Monitor
  // Serial.print("Distance: ");
  // Serial.println(tempDistance);
  lineposition = readSensors();
  // als de value LOW is is de lijn onder die sensor hij is dus HIGH als de sensor wit ziet
  int value_1 = digitalRead(s1);
  int value_2 = digitalRead(s2);
  int value_3 = digitalRead(s3);
  int value_4 = digitalRead(s4);
  int value_5 = digitalRead(s5);

  //Serial.println(lineposition);
  while (millis() - starttijd < 1000) {
    useDisplays('s', 't');
  }
  displays();
  //showChar('s');
  if (lineposition == "11011" || lineposition == "00011") {
    forward();
  } else if (lineposition == "11001" || lineposition == "11101" || lineposition == "11100" || lineposition == "11110" || lineposition == "11000") {
    right();
  } else if (lineposition == "10111" || lineposition == "10011" || lineposition == "00111" || lineposition == "01111") {
    left();
  } else if (lineposition == "11111") {
    u_turn();
  } else if (lineposition == "00000") {
    checkfinish();
  }

  if (tempDistance > 0) {
    distance = tempDistance;
    if (distance <= 10) { 
        u_turn();
        delay(1000);
    }
  }
}

String readSensors() {
  String value_1 = String(digitalRead(s1));
  String value_2 = String(digitalRead(s2));
  String value_3 = String(digitalRead(s3));
  String value_4 = String(digitalRead(s4));
  String value_5 = String(digitalRead(s5));
  return value_1 + value_2 + value_3 + value_4 + value_5;
}
// Function to go forward
void forward() {
  digitalWrite(directionPinR, LOW);
  digitalWrite(directionPinL, HIGH);
  analogWrite(pwmPinR, speed);
  analogWrite(pwmPinL, speed);
  //Serial.println("Going forward");
}


void right() {
  digitalWrite(directionPinL, HIGH);
  analogWrite(pwmPinR, 0);
  analogWrite(pwmPinL, speed);
}

void vergelijkPing() {
  if (tempDistance > 0) {
    distance = tempDistance;
  }
}

void turnright() {
  forward();
  lineposition = readSensors();
  if (lineposition == "10000" || lineposition == "11000") {
    right();
  }
}


void left() {
  digitalWrite(directionPinR, LOW);
  analogWrite(pwmPinR, speed);
  analogWrite(pwmPinL, 0);
}

void u_turn() {
  digitalWrite(directionPinL, LOW);
  digitalWrite(directionPinR, LOW);

  analogWrite(pwmPinR, speed);
  analogWrite(pwmPinL, speed);
}
void backward() {
  digitalWrite(directionPinR, HIGH);
  digitalWrite(directionPinL, LOW);
  analogWrite(pwmPinR, speed);
  analogWrite(pwmPinL, speed);
}

void stop() {
  digitalWrite(directionPinL, HIGH);
  digitalWrite(directionPinR, LOW);
  analogWrite(pwmPinR, 0);
  analogWrite(pwmPinL, 0);
}

void checkfinish() {
  delay(310);
  lineposition = readSensors();
  if (lineposition == "00000") {
    stop();
  }
}

void displays() {
  digitalWrite(segA, LOW);
  digitalWrite(segB, LOW);
  digitalWrite(segC, LOW);
  digitalWrite(segD, LOW);
  digitalWrite(segE, LOW);
  digitalWrite(segF, LOW);
  digitalWrite(segG, LOW);
  // digitalWrite(segU1, HIGH);
  // digitalWrite(segU2, HIGH);
}

void showChar(char c) {
  displays();
  switch (c) {
    case 's':
      digitalWrite(segA, HIGH);
      digitalWrite(segF, HIGH);
      digitalWrite(segG, HIGH);
      digitalWrite(segC, HIGH);
      digitalWrite(segD, HIGH);
      break;
    case 't':
      digitalWrite(segF, HIGH);
      digitalWrite(segG, HIGH);
      digitalWrite(segE, HIGH);
      digitalWrite(segD, HIGH);
      break;
    default:
      displays();
  }
}

void useDisplays(char c1, char c2) {
  if (millis() - correction > 4) {
    displayStatus = !displayStatus;
    correction = millis();
  }
  if (displayStatus) {
    digitalWrite(segU1, LOW);
    digitalWrite(segU2, HIGH);
    showChar(c1);
  }
  if (!displayStatus) {
    digitalWrite(segU1, HIGH);
    digitalWrite(segU2, LOW);
    showChar(c2);
  }
}
