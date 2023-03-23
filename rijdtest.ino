struct Node{
	bool to_finish;
	uint8_t flag;
	int travel_time;
	Node* parent;
	Node* left;
	Node* forward;
	Node* right;
};

enum direction{
	LEFT,
	FORWARD,
	RIGHT,
	TSPLIT,
	INTERSECTION,
	FORWARD_AND_LEFT,
	FORWARD_AND_RIGHT
};

enum visited{
	unvisited,
	visited,
	exhausted
};



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

//maze visited tree.
bool backtracking = false;
Node* root = NULL;
Node* parent = NULL;
Node* current = NULL;
int timing = milis();

bool create_sub_node(int direction){
	Node* next = malloc(sizeof(Node*));
	next->to_finish = false;
	next->flag = unvisited;
	next->parent = current;
	next->left = NULL;
	next->forward = NULL;
	next->right = NULL;

	if(current->flag == exhausted){
		return false;

	}

	if(direction == LEFT){
		current->left = next;
	}else if(direction == FORWARD){
		current->forward = next;
	}else if(direction == RIGHT){
		current->right = next;
	}else if(direction == TSPLIT){
		current->left = next;
		Node* right = malloc(sizeof(Node*));
		memcpy(right, next, sizeof(Node*));
		current->right = right;
	}else if(direction == FORWARD_AND_LEFT){
		current->left = next;
		Node* forward = malloc(sizeof(Node*));
		memcpy(forward, next, sizeof(Node*));
		current->forward = forward;

	}else if(direction == FORWARD_AND_RIGHT){
		current->right = next;
		Node* forward = malloc(sizeof(Node*));
		memcpy(forward, next, sizeof(Node*));
		current->forward = forward;
	

	}else{
		current->left = next;
		Node* right = malloc(sizeof(Node*));
		Node* forward = malloc(sizeof(Node*));
		memcpy(right, next, sizeof(Node*));
		memcpy(forward, next, sizeof(Node*));
		current->right = right;
		current->forward = forward;
	}

	return true;

}

int decide_direction(){
	if(current->forward == NULL && current->right == NULL && current->left != NULL && current->left->flag != exhausted){
		//left
		current = current->left;
		parent = current;
		return 1;

	}
	if(current->left == NULL && current->right == NULL && current->forward != NULL && current->forward->flag != exhausted){
		//forward
		current = current->forward;
		parent = current;
		return 2;

	}
	if(current->left == NULL && current->forward == NULL && current->right != NULL && current->right->flag != exhausted){
		//right
		current = current->right;
		parent = current;
		return 3;

	}
	//u-turn
	current->flag = exhausted;

	return 0;
}

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

//maze nodes.
	root = malloc(sizeof(Node*));
	root->flag = unvisited;
	root->left = NULL;
	root->right = NULL;
	root->forward = NULL;
}

void set_node_timing(){
	current->travel_time = timing - milis();
	timing = milis();

}

void loop() {
// als de value LOW is is de lijn onder die sensor hij is dus HIGH als de sensor wit ziet
	
	lineposition = readSensors();

	if(lineposition == "10001"){
		if(backtracking && current->parent == parent){
			current = current->parent;
			forward();
		}else{
			set_node_timing();
			current->flag = visited;
			bool res = create_sub_node(INTERSECTION);
			if(res){
				int direction = decide_direction();
				if(direction == 1){
					left();
				}else if(direction == 2){
					forward();
				}else if(direction == 3){
					right();
				}else{
					current->flag = exhausted;
					backtracking = true;
					forward();

				}
			}
		}

	}else if(lineposition == "11001"){
		if(backtracking && current->parent == parent){
			current = current->parent;
			forward();
		}else{
			set_node_timing();
			bool res = create_sub_node(FORWARD_AND_RIGHT);
			if(res){
				int direction = decide_direction();
				if(direction == 3){
					right();
				}else if(direction == 2){
					forward();
				}else if(direction == 0){
					current->flag = exhausted;
					backtracking = true;
					forward();

				}

			}
		}
	}else if(lineposition == "10011"){
		if(backtracking && current->parent == parent){
			current = current->parent;
			forward();
		}else{
			set_node_timing();
			bool res = create_sub_node(FORWARD_AND_LEFT);
			if(res){
			int direction = decide_direction();
			if(direction == 1){
				left();
			}else if(direction == 2){
				forward();
			}else if(direction == 0){
				current->flag = exhausted;
				backtracking = true;
				forward();
			}

		}

	}else if(lineposition == "11101" || lineposition == "11000" || lineposition == "11100" || lineposition == "11110" || lineposition == "00001"){
		set_node_timing();
		create_sub_node(RIGHT);
		parent = current;
		current = current->right;
    right();
  }
  else if(lineposition == "11011"){
    forward();
  }
  else if(lineposition == "10111" || lineposition == "00011" || lineposition == "00111" || lineposition == "01111" || lineposition == "10000"){
		set_node_timing();
		create_sub_node(LEFT);
		parent = current;
		current = current->left;
    left();
  }
  else if(lineposition == "11111"){
    u_turn();
  }
  else if(lineposition == "00000"){
    checkfinish();
  }
	Serial.println((backtracking) ? "Currently backtracking" : "Not backtracking");


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
String readSensors(){
  String value_1 = String(digitalRead(s1));
  String value_2 = String(digitalRead(s2));
  String value_3 = String(digitalRead(s3));
  String value_4 = String(digitalRead(s4));
  String value_5 = String(digitalRead(s5));
  return value_1 + value_2 + value_3 + value_4 + value_5;
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
		print_start_to_end_turns();
  }
}

void print_start_to_end_turns(){
	current->to_finish = true;
	while(current->parent != NULL){
		current = current->parent;
		current->to_finish = true;

	}
	Serial.println("Moves from start to finish: ");
	Node* temp = current;

	while(temp != NULL){
		if(temp->left != NULL && temp->left->to_finish){
			Serial.print("Left");
		}else if(temp->forward != NULL && temp->forward->to_finish){
				Serial.print("Forward");
		}else if(temp->right != NULL && temp->right->to_finish){
				Serial.print("Right");
		}
		Serial.print("->");

	}



}
