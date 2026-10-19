int s0;
int s1;
int s2;
int s6;
int s7;
int g1;
int g2;
int lights[5];
int junctionCount = 0;
const int totJunc = 4;


const int sensorPins[5] = { A0, A1, A2, A6, A7 };
const int ledIndex[5] = {0, 1, 2, 6, 7};
const int DEADBAND = 0;
const int SPEED_L = 16;
const int SPEED_R = 16;
const int SPEED_LF = 22;
const int SPEED_RF = 22;
const int SPEED_PIVOT = 58;
const int SPEED_PIVOT_STOP = 12;
const int SPEED_STOP = 16;


const int GROUND_BLACK_L = 952;
const int GROUND_BLACK_R = 954;
int loopCount =0;
void setup() {
  DDRA = 0xFF;
  DDRJ |= 0b00000110;
  Serial.begin(9600);
  delay(7000);
  Forward();
  delay(500);
  brake();
}

void proxLEDoff(unsigned char proxIndex) {
  if (proxIndex < 8) {
    PORTA &= ~(1 << proxIndex);
  }
}

void proxLEDon(unsigned char proxIndex) {
  if (proxIndex < 8) {
    PORTA |= (1 << proxIndex);
  }
}

void readProxSensors() {
  for (int i = 0; i < 5; i++) {
    proxLEDon(ledIndex[i]);
    delay(5);
    lights[i] = analogRead(sensorPins[i]);
    proxLEDoff(ledIndex[i]);
  }
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
  analogWrite(6, SPEED_L);
  analogWrite(5, SPEED_R);
}
void ForwardF() {
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  analogWrite(6, SPEED_LF);
  analogWrite(5, SPEED_RF);
}

void sharpLeft() {
  delay(150);
  while (true) {
    // left REVERSE, right FORWARD = pivot left
    analogWrite(6, SPEED_PIVOT_STOP);  // left SLOW
    analogWrite(5, SPEED_PIVOT);  // right FAST
    digitalWrite(7, LOW);
    digitalWrite(2, LOW);
    delay(60);
    readGround1();
    readGround2();
    if (g1 < GROUND_BLACK_L && g2 < GROUND_BLACK_R) {
      brake();
      break;
    }
  }
}

void sharpRight() {
  delay(150);
  while (true) {
    // right REVERSE, left FORWARD = pivot right
    analogWrite(6, SPEED_PIVOT);  // left FAST
    analogWrite(5, SPEED_PIVOT_STOP);  // right SLOW
    digitalWrite(7, LOW);
    digitalWrite(2, LOW);
    delay(60);
    readGround1();
    readGround2();
    if (g1 < GROUND_BLACK_L && g2 < GROUND_BLACK_R) {
      brake();
      break;
    }
  }
}
void barrierSharpLeft() {
  analogWrite(6, SPEED_PIVOT_STOP);
  analogWrite(5, SPEED_PIVOT);
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  delay(120); // tune this
  brake();
}

void barrierSharpRight() {
  analogWrite(6, SPEED_PIVOT);
  analogWrite(5, SPEED_PIVOT_STOP);
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  delay(120); // tune this
  brake();
}

void rightTurn() {
  digitalWrite(7, LOW);
  digitalWrite(5, LOW);
  analogWrite(6, SPEED_L);
  analogWrite(2, SPEED_STOP);
}

void leftTurn() {
  digitalWrite(6, LOW);
  digitalWrite(2, LOW);
  analogWrite(7, SPEED_R);
  analogWrite(5, SPEED_STOP);
}

void brake() {
  digitalWrite(7, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  delay(20); // reduced from 1000 so it doesn't stall too long
}
void barrierFollow() {
  // move forward until obstacle detected ahead
  while (true) {
    Forward();
    readProxSensors();
    s0 = lights[0];
    s2 = lights[2];
    s6 = lights[3];

    if (s0 < 740) {
      // obstacle ahead — brake and decide which way to turn
      brake();

      if (s2 < s6) {
        // s2 lower = obstacle closer on right = turn left
        barrierSharpLeft();
        Forward();
        delay(400);
      }
      else {
        // s6 lower = obstacle closer on left = turn right
        barrierSharpRight();
        Forward();
        delay(400);
      }
      break;
    }
  }

  Forward();
  delay(2000); // tune this
  brake();
  while (true);
}
void loop() {
  PORTJ &= 0b11111001;
  
  //readProxSensors();
  

  for (int i = 0; i < 5; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(lights[i]);
  }
 
  delay(10);
  readGround1();
  readGround2();
// only read prox sensors every 5 loops
    if (loopCount >= 5) {
    readProxSensors();
    s0 = lights[0];
    s1 = lights[1];
    s2 = lights[2];
    s6 = lights[3];
    s7 = lights[4];
    loopCount = 0;
  }
  loopCount++;

 

  Serial.print("Sensor 1 is: ");
  Serial.println(g1);
  Serial.print("Sensor 2 is: ");
  Serial.println(g2);

   // line following
  if (g1 < GROUND_BLACK_L && g2 < GROUND_BLACK_R) {
    Forward();
  }
  else if (g1 > (GROUND_BLACK_L + DEADBAND) && g2 < GROUND_BLACK_R) {
    leftTurn();
  }
  else if (g1 < GROUND_BLACK_L && g2 > (GROUND_BLACK_R + DEADBAND)) {
    rightTurn();
  }
  // BOTH ground sensors see black = junction reached
  else if (g1 > GROUND_BLACK_L && g2 > GROUND_BLACK_R) {
    
    // stop at junction
    brake();

    // now take a fresh prox reading while stationary
    readProxSensors();
    s0 = lights[0];
    s1 = lights[1];
    s2 = lights[2];
    s6 = lights[3];
    s7 = lights[4];

    // decide which way to turn based on where obstacle is
    if (s6 < 500) {
  sharpRight();
  junctionCount++;
}
else if (s2 < 800) {
  sharpLeft();
  junctionCount++;
}
else if (s7 < 913) {
  sharpLeft();
  junctionCount++;
}
else if (s7 > 913) {
  sharpRight();
  junctionCount++;
}
if (junctionCount >= totJunc){
  unsigned long barrierTime  = millis();
  while (millis() - barrierTime < 8000){
  barrierFollow();
  }
  brake();
  while (true);
}
  
//    if (s1 < 930 && s7>920 && s0<930) {
//      sharpRight();
//    }
//    else if (s7<920){// && s1>940) {
//      sharpLeft();
//    }
////    else if (s1<940 && s7<910){
////      if (s0<930){
////      sharpRight();
////    }
////    else{
////      sharpLeft();
////    }
////
////    }
//    else if (s7>920 && s1>930){
//      if (s2 < 800){
//        sharpLeft();
//       
//      }
//      else if (s6< 500){
//        sharpRight();
//      }
//      else{
//      Forward();
//    }
//    }
//    
//    else {
// 
//      Forward();
//    }
  }
}
