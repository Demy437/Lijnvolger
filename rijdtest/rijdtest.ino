int directionPinR = 12; //RECHTER MOTOR
int pwmPinR = 3;
int brakePinR = 9;

int directionPinL = 13; // LINKER MOTOR
int pwmPinL = 11;
int brakePinL = 8;

int speed = 50;
String position ="";
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

TCCR2B = TCCR2B & B11111000 | B00000111;

// sensors
pinMode(s1, INPUT);
pinMode(s2, INPUT);
pinMode(s3, INPUT);
pinMode(s4, INPUT);
pinMode(s5, INPUT);

Serial.begin(9600);
}

void loop() {
position = readSensors();
// als de value LOW is is de lijn onder die sensor hij is dus HIGH als de sensor wit ziet
int value_1 = digitalRead(s1);
int value_2 = digitalRead(s2);
int value_3 = digitalRead(s3);
int value_4 = digitalRead(s4);
int value_5 = digitalRead(s5);

  if ( position =="11011") { //Line is in center
    forward();
    Serial.println("Going Forward");
  } 
  else if (
    (position == "10011")|| 
    (position == "00011")||
    (position == "00111")||
    (position == "01111")||
    (position == "10010")||
    (position == "10111")||
    (position == "00110")||
    (position == "10100")||
    (position == "00001"))
    { //line is on the left side
    left();
  }
    else if (
    (position == "01111")|| 
    (position == "00111")||
    (position == "00011")||
    (position == "11101")||
    (position == "01100")||
    (position == "10000"))

    { //line is on the left side
    right();
  }
    else{
    stop_robot();
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
  analogWrite(pwmPinR, 40);
  digitalWrite(directionPinR, HIGH);
  analogWrite(pwmPinL, 40);
  digitalWrite(directionPinL, LOW);
  Serial.println("Going backward");
}

void right() {
  digitalWrite(pwmPinR, 0);
  digitalWrite(directionPinR, LOW);
  digitalWrite(pwmPinL, speed);
  digitalWrite(directionPinL, HIGH);
  Serial.println("Turning right");
}

void left() {
  digitalWrite(pwmPinR, speed);
  digitalWrite(directionPinR, LOW);
  digitalWrite(pwmPinL, 0);
  digitalWrite(directionPinL, HIGH);
  Serial.println("Turning left");
}

void u_turn() {
  digitalWrite(pwmPinR, speed);
  digitalWrite(directionPinR, HIGH);
  digitalWrite(pwmPinL, speed);
  digitalWrite(directionPinL, HIGH);
  Serial.println("making a U-turn");
}

void stop_robot() {
  digitalWrite(pwmPinR, 0);
  digitalWrite(brakePinR, HIGH);
  digitalWrite(directionPinR, LOW);
  digitalWrite(pwmPinL, 0);
  digitalWrite(brakePinL, HIGH);
  digitalWrite(directionPinL, HIGH);
  Serial.println("Stopping robot");
}