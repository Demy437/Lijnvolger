int directionPinR = 12; //RECHTER MOTOR
int pwmPinR = 3;
int brakePinR = 9;

int directionPinL = 13; // LINKER MOTOR
int pwmPinL = 11;
int brakePinL = 8;

String lineposition ="";
int starttijd = millis();

// int speed = 45;//stopcontact
int speed = 85;//batterij
int defaultspeed = 75;

// sensors
#define s1 A0
#define s2 A1
#define s3 A2
#define s4 A3
#define s5 A4

//boolean to switch direction
bool directionStateR;
bool directionStateL;

void setup() {

//define pins
// motor Rechts
pinMode(directionPinR, OUTPUT);
pinMode(pwmPinR, OUTPUT);
pinMode(brakePinR, OUTPUT);
// motor links
pinMode(directionPinL, OUTPUT);
pinMode(pwmPinL, OUTPUT);
pinMode(brakePinL, OUTPUT);

TCCR2B = TCCR2B & B11111000 | B00000110; // for PWM frequency of 122.55 Hz
// TCCR2B = TCCR2B & B11111000 | B00000111; // for PWM frequency of 30.64 Hz
// sensors
pinMode(s1, INPUT);
pinMode(s2, INPUT);
pinMode(s3, INPUT);
pinMode(s4, INPUT);
pinMode(s5, INPUT);

Serial.begin(9600);
}

void loop() {

lineposition = readSensors();
// als de value LOW is is de lijn onder die sensor hij is dus HIGH als de sensor wit ziet
int value_1 = digitalRead(s1);
int value_2 = digitalRead(s2);
int value_3 = digitalRead(s3);
int value_4 = digitalRead(s4);
int value_5 = digitalRead(s5);

 
 
  if(lineposition == "11001" || lineposition == "11101" || lineposition == "11000" || lineposition == "11100" || lineposition == "11110" || lineposition == "00001"){
    right();
  }
  else if(lineposition == "11011"){
    forward();
  }
  else if(lineposition == "10111" || lineposition == "10011" || lineposition == "00011" || lineposition == "00111" || lineposition == "01111" || lineposition == "10000"){
    left();
  }
  else if(lineposition == "11111"){
    u_turn();
  }
  else if(lineposition == "00000"){
    checkfinish();
  }
}

String readSensors(){
  String value_1 = String(digitalRead(s1));
  String value_2 = String(digitalRead(s2));
  String value_3 = String(digitalRead(s3));
  String value_4 = String(digitalRead(s4));
  String value_5 = String(digitalRead(s5));
  return value_1 + value_2 + value_3 + value_4 + value_5;
}
// Function to go forward
void forward() {
  analogWrite(pwmPinR, speed);
  digitalWrite(directionPinR, LOW);
  analogWrite(pwmPinL, speed);
  digitalWrite(directionPinL, HIGH);
  Serial.println("Going forward");
}

void backward() {
  analogWrite(pwmPinR, speed);
  digitalWrite(directionPinR, HIGH);
  analogWrite(pwmPinL, speed);
  digitalWrite(directionPinL, LOW);
  Serial.println("Going backward");
}

void right() {
  //  if(millis()-starttijd <1000){
  //   speed = (speed - 10);
  // }
  analogWrite(pwmPinR, 0);
  analogWrite(pwmPinL, speed);
  digitalWrite(directionPinL, HIGH);
    // speed = defaultspeed;
}

void left() {
  //  if(millis()-starttijd <1000){
  //   speed = (speed - 10);
  // }
  analogWrite(pwmPinR, speed);
  digitalWrite(directionPinR, LOW);
  analogWrite(pwmPinL, 0);
  // speed = defaultspeed;
}

void u_turn() {
  //  if(millis()-starttijd <1000){
  //   speed = (speed - 10);
  // }
  analogWrite(pwmPinR, speed);
  digitalWrite(directionPinR, HIGH);
    analogWrite(pwmPinR, speed);
  digitalWrite(directionPinL, HIGH);
  // speed = defaultspeed;
}

void stop() {
  analogWrite(pwmPinR, 0);
  digitalWrite(brakePinR, HIGH);
  digitalWrite(directionPinR, LOW);
  analogWrite(pwmPinL, 0);
  digitalWrite(brakePinL, HIGH);
  digitalWrite(directionPinL, HIGH);
  Serial.println("Stopping robot");
}

void checkfinish(){
  delay(250);
  lineposition = readSensors();
  if(lineposition == "00000"){
    stop();
  }
}