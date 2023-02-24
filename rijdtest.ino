int directionPinR = 12; //RECHTER MOTOR
int pwmPinR = 3;
int brakePinR = 9;

int directionPinL = 13; // LINKER MOTOR
int pwmPinL = 11;
int brakePinL = 8;
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

// sensors
pinMode(s1, INPUT);
pinMode(s2, INPUT);
pinMode(s3, INPUT);
pinMode(s4, INPUT);
pinMode(s5, INPUT);

Serial.begin(9600);
}

void loop() {
// als de value LOW is is de lijn onder die sensor hij is dus HIGH als de sensor wit ziet
int value_1 = digitalRead(s1);
int value_2 = digitalRead(s2);
int value_3 = digitalRead(s3);
int value_4 = digitalRead(s4);
int value_5 = digitalRead(s5);

if (value_3 == LOW){ //T Point
    forward();
  }
  else{
    stop_robot();
  }


}
// Function to go forward
void forward() {
  analogWrite(pwmPinR, 100);
  digitalWrite(directionPinR, LOW);
  analogWrite(pwmPinL, 100);
  digitalWrite(directionPinL, HIGH);
  Serial.println("Going forward");
}

void backward() {
  analogWrite(pwmPinR, 100);
  digitalWrite(directionPinR, HIGH);
  analogWrite(pwmPinL, 100);
  digitalWrite(directionPinL, LOW);
  Serial.println("Going backward");

}

void right() {
  digitalWrite(pwmPinR, 0);
  digitalWrite(directionPinR, LOW);
  digitalWrite(pwmPinL, 100);
  digitalWrite(directionPinL, HIGH);
  Serial.println("Turning right");

}

void left() {
  digitalWrite(pwmPinR, 100);
  digitalWrite(directionPinR, LOW);
  digitalWrite(pwmPinL, 0);
  digitalWrite(directionPinL, HIGH);
  Serial.println("Turning left");

}

void u_turn() {
  digitalWrite(pwmPinR, 100);
  digitalWrite(directionPinR, HIGH);
  digitalWrite(pwmPinL, 100);
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
