int g1;
int g2;
const int GROUND_BLACK_L = 952; //assigns the value of the light sensitivy where sensor 1 picks up white
const int GROUND_BLACK_R = 954; //assigns the value of the light sensitivity where sensor 2 picks up white

//assigns PWM values to the left and right motor depending on the wanted condition:
const  int SPEED_FWD_L = 28; //left motor forward
const  int SPEED_FWD_R = 28; //right motor forward
const int  SPEED_TURN_FAST = 21; //used for the direction we want to turn in
const int  SPEED_TURN_SLOW = 15; //used to counter that direction and stabilise the turn
const int  SPEED_REVERSE = 17;

void setup(){
  DDRJ |= 0b00000110; // initialises the J port

  Serial.begin(9600);
  //randomSeed(analogRead(A15));
}

void readGround1(){
  g1 = analogRead(A9);
}

void readGround2(){
    g2 = analogRead(A10);
}
void Forward(){
 digitalWrite(7, LOW);
 digitalWrite(2, LOW);
analogWrite(6,SPEED_FWD_L);
analogWrite(5,SPEED_FWD_R);
}
void reverseMove(){
 digitalWrite(5, LOW);
 digitalWrite(6, LOW);
analogWrite(7,SPEED_FWD_L);
analogWrite(2,SPEED_FWD_R);
}
void rightTurn(){
  digitalWrite(7,LOW);
  digitalWrite(5, LOW);
  analogWrite(6, SPEED_TURN_SLOW);
  analogWrite(2,SPEED_TURN_FAST);
  
}

void leftTurn(){
  digitalWrite(6, LOW);
  digitalWrite(2, LOW);
  analogWrite(7,SPEED_TURN_FAST);
  analogWrite(5,SPEED_TURN_SLOW);
  
  
}
void brake(){
  digitalWrite(7, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  
}

void randomTurnAfterEdge() {
int turnChoice = random(0,2);
int turnTime = random(300,900);
brake();
delay(100);
reverseMove();
delay(750);
if (turnChoice == 0){
  leftTurn();
  delay(turnTime);
} else{
  rightTurn();
  delay(turnTime);
 
}
brake();
delay(100);
}
  
  




void challenge3() {
  PORTJ &= 0b11111001;
  delay(10);
  readGround1();
  readGround2();
  Serial.print("Sensor 1 is: ");
  Serial.println(g1);
  Serial.print("Sensor 2 is: ");
  Serial.println(g2);
  if (g1> GROUND_BLACK_L || g2> GROUND_BLACK_R){
  
    randomTurnAfterEdge();
    
  }
  
  else {
    Forward();
    delay(90);
  }
  }
  void loop() {
    challenge3();
  }
