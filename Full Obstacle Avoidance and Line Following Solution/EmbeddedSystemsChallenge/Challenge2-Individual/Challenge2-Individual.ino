int g1;
int g2;
int g3;
int g4;
const int DEADBAND = 10;

void setup() {
  DDRJ |= 0b00001111; // initialises the J port

  Serial.begin(9600);
}

void readGround1() {
  g1 = analogRead(A9);
}

void readGround2() {
  g2 = analogRead(A10);
}
void readGround3(){
  g3 = analogRead(A8);
}
void readGround4(){
  g4 = analogRead(A11);
}
void Forward() {
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  analogWrite(6, 14);
  analogWrite(5, 14);

}
void rightTurn() {
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  analogWrite(6,0);
  analogWrite(5, 24);

}

void leftTurn() {
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  analogWrite(6, 24);
  analogWrite(5, 0);


}
//void brake() {
//  digitalWrite(7, HIGH);
//  digitalWrite(2, HIGH);
//  digitalWrite(6, HIGH);
//  digitalWrite(5, HIGH);
//
//}

//void sharpLeft() {
//  digitalWrite(6, LOW);
//  digitalWrite(2, LOW);
//  analogWrite(7, 33);
//  analogWrite(5, 33);
//}
////void sharpRight(){
//  digitalWrite(7, LOW);
//  digitalWrite(5, LOW);
//  analogWrite(6, 33);
//  analogWrite(2, 33);
//  
//}


void loop() {
  PORTJ &= 0b11110000;
  delay(10);
  readGround1();
  readGround2();
//  readGround3();
//  readGround4();
  Serial.print("Sensor 1 is: ");
  Serial.println(g1);
  Serial.print("Sensor 2 is: ");
  Serial.println(g2);
//  Serial.print("Sensor 3 is: ");
//  Serial.println(g3);
//  Serial.print("Sensor 4 is: ");
//  Serial.println(g4);
  if (g1 < 950 && g2 < 950) {
    Forward();

  }

  else if (g1 < 950 && g2 > 950) {
    leftTurn();
  }
  else if (g1 > 950 && g2 < 950) {
    rightTurn();

  }
//  else if(g4<859){
//    
// sharpRight();
//  }
//  else if( g3<859){
//    
//    sharpLeft();
//  }
  else if(g1>950 && g2>950){
    Forward();
  }
}
//  //line correection, when its about to deviate off path
//  else if (g1 > 972 && g2 < 954){
//    brake();
//    leftTurn();
//    delay(5);
//  }
//  else if (g1 < 952 && g2 > (974)) {
//    brake();
//    rightTurn();
//    delay(5);
//  }
//
//
//
//
//}
