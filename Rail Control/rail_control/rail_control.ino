#define motorDriverNBA 27 // Digital pins for Nothbound motor driver direction.
#define motorDriverNBB 26
#define nbPWM 28 // Analog pinout for Northbound motor driver speed.

#define startNBA 0 // Northbound Start A Sensor.
#define endNBA 6 // Northbound End A Sensor.
#define stop1NBA 2 // Northbound Stop 1 A Sensor.

#define motorDriverSBA 21 // Digital pins for Southbound motor driver direction.
#define motorDriverSBB 20
#define sbPWM 22 // Analog pinout for Southbound motor driver speed.

#define startSBA 8 // Southbound Start A Sensor.
#define endSBA 14// Southbound End A Sensor.
#define stop1SBA 10 // Southbound Stop 1 A Sensor.

#define loopIndicatorLED 15 // LED that indicates when loop cycles.

int nbMoving; // Northbound global variables
int nbDirection;
int nbTrainSpeed;
int nbTrainGo;
int nbTrainStop;

int sbMoving; // Southbound global variables
int sbDirection;
int sbTrainSpeed;
int sbTrainGo;
int sbTrainStop;

void nbChangeDirection(){ // Change direction of Northbound train and direction value.
  if (nbDirection == 0){
    digitalWrite(motorDriverNBA, LOW);
    digitalWrite(motorDriverNBB, HIGH);
    nbDirection = 1;
  } else if (nbDirection == 1) {
    digitalWrite(motorDriverNBA, HIGH);
    digitalWrite(motorDriverNBB, LOW);
    nbDirection = 0;
  }
}

void sbChangeDirection(){ // Change direction of Southbound train and direction value.
  if (sbDirection == 0){
    digitalWrite(motorDriverSBA, LOW);
    digitalWrite(motorDriverSBB, HIGH);
    sbDirection = 1;
  } else if (sbDirection == 1) {
    digitalWrite(motorDriverSBA, HIGH);
    digitalWrite(motorDriverSBB, LOW);
    sbDirection = 0;
  }
}

void setup() {

  pinMode(motorDriverNBA, OUTPUT); // Set Northbound rPi pinouts.
  pinMode(motorDriverNBB, OUTPUT);
  pinMode(startNBA, INPUT);
  pinMode(endNBA, INPUT);
  pinMode(stop1NBA, INPUT);
  digitalWrite(motorDriverNBA, HIGH); // Set Northbound train direction and "speed" to start.
  digitalWrite(motorDriverNBB, LOW);
  analogWrite(nbPWM, nbTrainSpeed); 

  pinMode(motorDriverSBA, OUTPUT); // Set Southbound rPi pinouts.
  pinMode(motorDriverSBB, OUTPUT);
  pinMode(startSBA, INPUT);
  pinMode(endSBA, INPUT);
  pinMode(stop1SBA, INPUT);
  digitalWrite(motorDriverSBA, HIGH); // Set Southbound train direction and "speed" to start.
  digitalWrite(motorDriverSBB, LOW);
  analogWrite(sbPWM, sbTrainSpeed); 

  pinMode(loopIndicatorLED, OUTPUT);

  nbMoving = 0; // Set start values for Northbound variables.
  nbDirection = 0;
  nbTrainSpeed = 10; // Enough voltage to turn on the lights but not drive the train.
  nbTrainStop = (millis() + 2000); // Delay before starting.

  sbMoving = 0; // Set start values for Southbound variables.
  sbDirection = 0;
  sbTrainSpeed = 10; // Enough voltage to turn on the lights but not drive the train.

  delay(5000);

  //sbTrainStop = (millis() + 2500); // Delay before starting.
}

void loop() {

  digitalWrite(loopIndicatorLED, HIGH); // Turn on loop indicator LED for testing frequency of loop.

  int readStartNBA = digitalRead(startNBA); // Read sensor values each loop.
  int readEndNBA = digitalRead(endNBA);
  int readStop1NBA = digitalRead(stop1NBA);
  int readStartSBA = digitalRead(startSBA);
  int readEndSBA = digitalRead(endSBA);
  int readStop1SBA = digitalRead(stop1SBA);

  digitalWrite(loopIndicatorLED, LOW); // Turns out it's just pretty much constantly on, so no delay I guess?
  
  if ((nbMoving == 1) && millis() >= nbTrainGo) { // Handle Northbound moving train at terminals, check time to ignore repeat inputs.
    if ((readStartNBA == LOW) or (readEndNBA == LOW)) {
      nbMoving = 0;
      nbTrainSpeed = 10;
      analogWrite(nbPWM, nbTrainSpeed);
      nbChangeDirection();
      nbTrainStop = (millis() + 4000);
    }
    else if (readStop1NBA == LOW && millis() >= nbTrainGo) { // Handle Northbound moving train at midstops, check time to ignore repeat inputs.
      nbMoving = 0;
      nbTrainSpeed = 10;
      analogWrite(nbPWM, nbTrainSpeed);
      nbTrainStop = (millis() + 4000);
    }
  } else if ((nbMoving == 0) && (millis() >= nbTrainStop)) { // Handle Northbound stopped train, check time to allow passengers to board.
    nbMoving = 1;
    nbTrainGo = (millis() + 3000);
    nbTrainSpeed = 75;
    analogWrite(nbPWM, nbTrainSpeed);
  }

  if ((sbMoving == 1) && millis() >= sbTrainGo) { // Handle Southbound moving train at terminals, check time to ignore repeat inputs.
    if ((readStartSBA == LOW) or (readEndSBA == LOW)) {
      sbMoving = 0;
      sbTrainSpeed = 10;
      analogWrite(sbPWM, sbTrainSpeed);
      sbChangeDirection();
      sbTrainStop = (millis() + 4000);
    }
    else if (readStop1SBA == LOW && millis() >= sbTrainGo) { // Handle Southbound moving train at midstops, check time to ignore repeat inputs.
      sbMoving = 0;
      sbTrainSpeed = 010;
      analogWrite(sbPWM, sbTrainSpeed);
      sbTrainStop = (millis() + 4000);
    }
  } else if ((sbMoving == 0) && (millis() >= sbTrainStop)) { // Handle Southbound stopped train, check time to allow passengers to board.
    sbMoving = 1;
    sbTrainGo = (millis() + 3000);
    sbTrainSpeed = 85;
    analogWrite(sbPWM, sbTrainSpeed);
  }
}