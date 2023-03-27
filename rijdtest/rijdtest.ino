const int echoPin = 9;
const int trigPin = 10;

int directionPinR = 12;  //RECHTER MOTOR
int pwmPinR = 3;
//int brakePinR = 9;

int directionPinL = 13;  // LINKER MOTOR
int pwmPinL = 11;
//int brakePinL = 8;

String lineposition = "";
int starttijd = millis();

// int speed = 45;//stopcontact
int speed = 50;  //batterij
int defaultspeed = 75;

long duration;
int distance;

// sensors
#define s1 2
#define s2 4
#define s3 5
#define s4 6
#define s5 7

//boolean to switch direction
bool directionStateR;
bool directionStateL;

void setup() {

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

  Serial.begin(9600);
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  //delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  //delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  lineposition = readSensors();
  // als de value LOW is is de lijn onder die sensor hij is dus HIGH als de sensor wit ziet
  int value_1 = digitalRead(s1);
  int value_2 = digitalRead(s2);
  int value_3 = digitalRead(s3);
  int value_4 = digitalRead(s4);
  int value_5 = digitalRead(s5);

  //Serial.println(lineposition);

  if (lineposition == "11011" || lineposition == "00011") {
    forward();
  }
  else if (lineposition == "11001" || lineposition == "11101" || lineposition == "11100" || lineposition == "11110" || lineposition == "11000") {
    right();
  }
  else if (lineposition == "10111" || lineposition == "10011" || lineposition == "00111" || lineposition == "01111") {
    left();
  } 
  // else if (lineposition == "11000") {
  //   forward();
  //   delay(250);
  //   engage_brakes();
  //   if (lineposition == "11011" || lineposition == "10001" || lineposition == "11001" || lineposition == "10011") {
  //     disengage_brakes();
  //     forward();
  //   } else {
  //     disengage_brakes();
  //     backward();
  //     delay(500);
  //     engage_brakes();
  //     delay(250);
  //     disengage_brakes();
  //     turnright();
  //     delay(1000);
  //   }
  // }
  else if (lineposition == "11111") {
    u_turn();
  } else if(distance <= 10){
    u_turn();
  } else if (lineposition == "00000") {
    checkfinish();
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
  Serial.println("Going forward");
}


void right() {
  digitalWrite(directionPinL, HIGH);
  analogWrite(pwmPinR, 0);
  analogWrite(pwmPinL, speed);
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
  analogWrite(pwmPinR, speed);
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

// void engage_brakes() {
//   digitalWrite(brakePinL, HIGH);
//   digitalWrite(brakePinR, HIGH);
// }
// void disengage_brakes() {
//   digitalWrite(brakePinL, LOW);
//   digitalWrite(brakePinR, LOW);
// }

void checkfinish() {
  delay(350);
  lineposition = readSensors();
  if (lineposition == "00000") {
    stop();
  }
}