bool case0Init = false;
bool case1Init = false;
bool case2Init = false;
bool case3Init = false;

int s0, s1, s2, s6, s7;
int g1, g2, g3, g4;
int lights[5];
int junctionCount = 0;
int loopCount = 0;

const int totJunc = 4;
const int sensorPins[5] = { A0, A1, A2, A6, A7 };
const int ledIndex[5] = {0, 1, 2, 6, 7};

const int GROUND_BLACK_L = 952;
const int GROUND_BLACK_R = 954;

const int C1_SPEED = 20;
const int C1_TURN_FAST = 20;
const int C1_TURN_SLOW = 0;

const int C2_SPEED = 17;
const int C2_TURN_FAST = 26;
const int C2_TURN_SLOW = 0;

const int C3_SPEED_L = 28;
const int C3_SPEED_R = 28;
const int C3_TURN_FAST = 21;
const int C3_TURN_SLOW = 15;
const int C3_REVERSE = 17;

const int C4_SPEED_L = 16;
const int C4_SPEED_R = 16;
const int C4_SPEED_PIVOT = 58;
const int C4_SPEED_PIVOT_STOP = 12;
const int C4_SPEED_STOP = 16;

void setup() {
  DDRA = 0xFF;
  DDRJ |= 0b00001111;
  Serial.begin(9600);
}

unsigned char RotaryPositionChecker() {
  int Pos = PINC & 0b00001111;
  return Pos;
}

void proxLEDoff(unsigned char proxIndex) {
  if (proxIndex < 8) PORTA &= ~(1 << proxIndex);
}

void proxLEDon(unsigned char proxIndex) {
  if (proxIndex < 8) PORTA |= (1 << proxIndex);
}

void readProxSensors() {
  for (int i = 0; i < 5; i++) {
    proxLEDon(ledIndex[i]);
    delay(5);
    lights[i] = analogRead(sensorPins[i]);
    proxLEDoff(ledIndex[i]);
  }
}

void readGround1() { g1 = analogRead(A9); }
void readGround2() { g2 = analogRead(A10); }
void readGround3() { g3 = analogRead(A8); }
void readGround4() { g4 = analogRead(A11); }

void brake() {
  digitalWrite(7, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  delay(20);
}

void resetState() {
  brake();
  PORTA = 0x00;
  PORTJ &= 0b11110000;
  junctionCount = 0;
  loopCount = 0;
}

void C1_Forward() {
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  analogWrite(6, C1_SPEED);
  analogWrite(5, C1_SPEED);
}

void C1_rightTurn() {
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  analogWrite(6, C1_TURN_SLOW);
  analogWrite(5, C1_TURN_FAST);
}

void C1_leftTurn() {
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  analogWrite(6, C1_TURN_FAST);
  analogWrite(5, C1_TURN_SLOW);
}

void C2_Forward() {
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  analogWrite(6, C2_SPEED);
  analogWrite(5, C2_SPEED);
}

void C2_rightTurn() {
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  analogWrite(6, C2_TURN_SLOW);
  analogWrite(5, C2_TURN_FAST);
}

void C2_leftTurn() {
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  analogWrite(6, C2_TURN_FAST);
  analogWrite(5, C2_TURN_SLOW);
}

void C3_Forward() {
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  analogWrite(6, C3_SPEED_L);
  analogWrite(5, C3_SPEED_R);
}

void C3_reverseMove() {
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  analogWrite(7, C3_REVERSE);
  analogWrite(2, C3_REVERSE);
}

void C3_rightTurn() {
  digitalWrite(7, LOW);
  digitalWrite(5, LOW);
  analogWrite(6, C3_TURN_SLOW);
  analogWrite(2, C3_TURN_FAST);
}

void C3_leftTurn() {
  digitalWrite(6, LOW);
  digitalWrite(2, LOW);
  analogWrite(7, C3_TURN_FAST);
  analogWrite(5, C3_TURN_SLOW);
}

void C4_Forward() {
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  analogWrite(6, C4_SPEED_L);
  analogWrite(5, C4_SPEED_R);
}

void C4_rightTurn() {
  digitalWrite(7, LOW);
  digitalWrite(5, LOW);
  analogWrite(6, C4_SPEED_L);
  analogWrite(2, C4_SPEED_STOP);
}

void C4_leftTurn() {
  digitalWrite(6, LOW);
  digitalWrite(2, LOW);
  analogWrite(7, C4_SPEED_R);
  analogWrite(5, C4_SPEED_STOP);
}

void C4_sharpLeft() {
  delay(150);
  while (true) {
    if (RotaryPositionChecker() != 3) {
      brake();
      return;
    }
    analogWrite(6, C4_SPEED_PIVOT_STOP);
    analogWrite(5, C4_SPEED_PIVOT);
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

void C4_sharpRight() {
  delay(150);
  while (true) {
    if (RotaryPositionChecker() != 3) {
      brake();
      return;
    }
    analogWrite(6, C4_SPEED_PIVOT);
    analogWrite(5, C4_SPEED_PIVOT_STOP);
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

void C4_barrierSharpLeft() {
  analogWrite(6, C4_SPEED_PIVOT_STOP);
  analogWrite(5, C4_SPEED_PIVOT);
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  delay(120);
  brake();
}

void C4_barrierSharpRight() {
  analogWrite(6, C4_SPEED_PIVOT);
  analogWrite(5, C4_SPEED_PIVOT_STOP);
  digitalWrite(7, LOW);
  digitalWrite(2, LOW);
  delay(120);
  brake();
}

void Challenge1Loop() {
  PORTJ &= 0b11110000;
  readGround1();
  readGround2();

  if (g1 < 952 && g2 < 954) {
    C1_Forward();
  }
  else if (g1 < 952 && g2 > 954) {
    C1_leftTurn();
  }
  else if (g1 > 952 && g2 < 954) {
    C1_rightTurn();
  }
  else if (g1 > 952 && g2 > 954) {
    brake();
  }
}

void Challenge2Loop() {
  PORTJ &= 0b11110000;
  readGround1();
  readGround2();

  if (g1 < 950 && g2 < 950) {
    C2_Forward();
  }
  else if (g1 < 950 && g2 > 950) {
    C2_leftTurn();
  }
  else if (g1 > 950 && g2 < 950) {
    C2_rightTurn();
  }
  else if (g1 > 950 && g2 > 950) {
    C2_Forward();
  }
}

void C3_randomTurnAfterEdge() {
  int turnChoice = random(0, 2);
  int turnTime = random(300, 900);
  brake();
  delay(100);
  C3_reverseMove();
  delay(750);
  if (turnChoice == 0) {
    C3_leftTurn();
    delay(turnTime);
  } else {
    C3_rightTurn();
    delay(turnTime);
  }
  brake();
  delay(100);
}

void Challenge3Loop() {
  PORTJ &= 0b11111001;
  readGround1();
  readGround2();

  if (g1 > GROUND_BLACK_L || g2 > GROUND_BLACK_R) {
    C3_randomTurnAfterEdge();
  } else {
    C3_Forward();
    delay(90);
  }
}

void barrierFollow() {
  while (true) {
    if (RotaryPositionChecker() != 3) {
      brake();
      return;
    }
    C4_Forward();
    readProxSensors();
    s0 = lights[0];
    s2 = lights[2];
    s6 = lights[3];

    if (s0 < 740) {
      brake();
      if (s2 < s6) {
        C4_barrierSharpLeft();
        C4_Forward();
        delay(400);
      } else {
        C4_barrierSharpRight();
        C4_Forward();
        delay(400);
      }
      break;
    }
  }
  C4_Forward();
  delay(2000);
  brake();
  while (true) {
    if (RotaryPositionChecker() != 3) return;
  }
}

void Challenge4Loop() {
  PORTJ &= 0b11111001;

  readGround1();
  readGround2();

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

  if (g1 < GROUND_BLACK_L && g2 < GROUND_BLACK_R) {
    C4_Forward();
  }
  else if (g1 > GROUND_BLACK_L && g2 < GROUND_BLACK_R) {
    C4_leftTurn();
  }
  else if (g1 < GROUND_BLACK_L && g2 > GROUND_BLACK_R) {
    C4_rightTurn();
  }
  else if (g1 > GROUND_BLACK_L && g2 > GROUND_BLACK_R) {
    brake();
    readProxSensors();
    s0 = lights[0];
    s1 = lights[1];
    s2 = lights[2];
    s6 = lights[3];
    s7 = lights[4];

    if (s6 < 500) {
      C4_sharpRight();
      junctionCount++;
    }
    else if (s2 < 800) {
      C4_sharpLeft();
      junctionCount++;
    }
    else if (s1 < 910) {
      C4_sharpLeft();
      junctionCount++;
    }
    else if (s1 > 910) {
      C4_sharpRight();
      junctionCount++;
    }

    if (junctionCount >= totJunc) {
      barrierFollow();
    }
  }
}

void loop() {
  int Pos = RotaryPositionChecker();

  switch (Pos) {
    case 0:
      PORTL = ~(0b00000001);
      PORTG |= 0b00001000;
      if (!case0Init) {
        case0Init = true;
        case1Init = false;
        case2Init = false;
        case3Init = false;
        resetState();
        DDRJ |= 0b00000110;
      }
      Challenge1Loop();
      break;

    case 1:
      PORTL = ~(0b00000011);
      PORTG |= 0b00001000;
      if (!case1Init) {
        case1Init = true;
        case0Init = false;
        case2Init = false;
        case3Init = false;
        resetState();
        DDRJ |= 0b00001111;
      }
      Challenge2Loop();
      break;

    case 2:
      PORTL = ~(0b00000111);
      PORTG |= 0b00001000;
      if (!case2Init) {
        case2Init = true;
        case0Init = false;
        case1Init = false;
        case3Init = false;
        resetState();
        DDRJ |= 0b00000110;
        randomSeed(analogRead(A15));
      }
      Challenge3Loop();
      break;

    case 3:
      PORTL = ~(0b00000111);
      PORTG = ~(0b00001000);
      if (!case3Init) {
        case3Init = true;
        case0Init = false;
        case1Init = false;
        case2Init = false;
        resetState();
        DDRA = 0xFF;
        DDRJ |= 0b00000110;
        delay(7000);
        C4_Forward();
        delay(500);
        brake();
      }
      Challenge4Loop();
      break;

    default:
      case0Init = false;
      case1Init = false;
      case2Init = false;
      case3Init = false;
      resetState();
      PORTL = 0xFF;
      PORTG = 0xFF;
      break;
  }
}
