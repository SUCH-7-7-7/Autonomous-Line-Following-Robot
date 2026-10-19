int g1;
int g2;
int g3;
const int DEADBAND = 10;

void setup() {
  DDRJ |= 0b00000110; // initialises the J port

  Serial.begin(9600);
}

void readGround1() {
  g1 = analogRead(A9);
}

void readGround2() {
  g2 = analogRead(A10);
}


void Forward() {
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  analogWrite(6, 20);
  analogWrite(5, 20);

}
void rightTurn() {
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  analogWrite(6, 0);
  analogWrite(5, 20);

}

void leftTurn() {
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  analogWrite(6, 20);
  analogWrite(5, 0);


}
void brake() {
  digitalWrite(7, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);

}

void loop() {
  PORTJ &= 0b11111001;
  delay(10);
  readGround1();
  readGround2();
  readGround3();
  Serial.print("Sensor 1 is: ");
  Serial.println(g1);
  Serial.print("Sensor 2 is: ");
  Serial.println(g2);
  Serial.print("Sensor 3 is: ");
  Serial.println(g3);
  if (g1 < 952 && g2 < 954) {
    Forward();

  }

  else if (g1 < 952 && g2 > 954) {
    leftTurn();
  }
  else if (g1 > 952 && g2 < 954) {
    rightTurn();

  }
  else if(g1>952 && 954){
    brake();
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
