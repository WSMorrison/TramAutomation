// Rail Control
//
// Arduino IDE developed control unit for rPi Nano W to control to model trams on two tracks,
// stopping at up to six stops and two terminals. 
//
//
// NORTHBOUND LINE
#define nbMotorDriverA 27 // Digital pins for Nothbound motor driver direction.
#define nbMotorDriverB 26
#define nbPWM 28 // Analog pinout for Northbound motor driver speed.

#define nbSouthTerminal 0 // Northbound start position sensor.
#define nbNorthTerminal 7 // Northbound end postion sensor.
#define nbStop1 1 // Northbound middle stop sensors.
#define nbStop2 2
#define nbStop3 3
#define nbStop4 4
#define nbStop5 5
#define nbStop6 6
// NORTHBOUND LINE

// SOUTHBOUND LINE
#define sbMotorDriverA 21 // Digital pins for Southbound motor driver direction.
#define sbMotorDriverB 20
#define sbPWM 22 // Analog pinout for Southbound motor driver speed.

#define sbNorthTerminal 8 // Southbound start position sensor.
#define sbSouthTerminal 15// Southbound end position sensor.
#define sbStop1 9 // Southbound middle stop sensors.
#define sbStop2 10
#define sbStop3 11
#define sbStop4 12
#define sbStop5 13
#define sbStop6 14
// SOUTHBOUND LINE

#define emergencyStopSensor 18 // Emergency Stop sensor circuit, multiple sensors in parallel.
#define emergencyIndicatorLED 19 // Emergency Stop indicator LED, currently used to indicate loop operation.

// Northbound global variables
int nbMoving; 
int nbDirection;
int nbTrainSpeed;
int nbTrainGo;
int nbTrainStop;

// Southbound global variables
int sbMoving; 
int sbDirection;
int sbTrainSpeed;
int sbTrainGo;
int sbTrainStop;

void nbChangeDirection(){ // Change direction of Northbound train and direction value.
  if (nbDirection == 0){
    digitalWrite(nbMotorDriverA, LOW);
    digitalWrite(nbMotorDriverB, HIGH);
    nbDirection = 1;
  } else if (nbDirection == 1) {
    digitalWrite(nbMotorDriverA, HIGH);
    digitalWrite(nbMotorDriverB, LOW);
    nbDirection = 0;
  }
}

void sbChangeDirection(){ // Change direction of Southbound train and direction value.
  if (sbDirection == 0){
    digitalWrite(sbMotorDriverA, LOW);
    digitalWrite(sbMotorDriverB, HIGH);
    sbDirection = 1;
  } else if (sbDirection == 1) {
    digitalWrite(sbMotorDriverA, HIGH);
    digitalWrite(sbMotorDriverB, LOW);
    sbDirection = 0;
  }
}

void setup() {

  pinMode(nbMotorDriverA, OUTPUT); // Set Northbound rPi pinouts.
  pinMode(nbMotorDriverB, OUTPUT);
  pinMode(nbSouthTerminal, INPUT);
  pinMode(nbNorthTerminal, INPUT);
  pinMode(nbStop1, INPUT);
  digitalWrite(nbMotorDriverA, HIGH); // Set Northbound train direction and "speed" to start.
  digitalWrite(nbMotorDriverB, LOW);
  analogWrite(nbPWM, nbTrainSpeed); 

  pinMode(sbMotorDriverA, OUTPUT); // Set Southbound rPi pinouts.
  pinMode(sbMotorDriverB, OUTPUT);
  pinMode(sbNorthTerminal, INPUT);
  pinMode(sbSouthTerminal, INPUT);
  pinMode(sbStop1, INPUT);
  digitalWrite(sbMotorDriverA, HIGH); // Set Southbound train direction and "speed" to start.
  digitalWrite(sbMotorDriverB, LOW);
  analogWrite(sbPWM, sbTrainSpeed); 

  pinMode(emergencyStopSensor, INPUT);
  pinMode(emergencyIndicatorLED, OUTPUT);

  nbMoving = 0; // Set start values for Northbound variables.
  nbDirection = 0;
  nbTrainSpeed = 10; // Enough voltage to turn on the lights but not drive the train.
  nbTrainStop = (millis() + 2000); // Delay before starting.

  sbMoving = 0; // Set start values for Southbound variables.
  sbDirection = 0;
  sbTrainSpeed = 10; // Enough voltage to turn on the lights but not drive the train.

  delay(5000);
}

void loop() {

  digitalWrite(emergencyIndicatorLED, LOW); // Turns out it's just pretty much constantly on, so no delay I guess?

  int readNbSouthTerminal = digitalRead(nbSouthTerminal); // Read sensor values each loop.
  int readNbNorthTerminal = digitalRead(nbNorthTerminal);
  int readNbStop1 = digitalRead(nbStop1);
  int readNbStop2 = digitalRead(nbStop2);
  int readNbStop3 = digitalRead(nbStop3);
  int readNbStop4 = digitalRead(nbStop4);
  int readNbStop5 = digitalRead(nbStop5);
  //int readNbStop6 = digitalRead(nbStop6);
  int readSbNorthTerminal = digitalRead(sbNorthTerminal);
  int readSbSouthTerminal = digitalRead(sbSouthTerminal);
  int readSbStop1 = digitalRead(sbStop1);
  int readSbStop2 = digitalRead(sbStop2);
  int readSbStop3 = digitalRead(sbStop3);
  int readSbStop4 = digitalRead(sbStop4);
  int readSbStop5 = digitalRead(sbStop5);
  //int readSbStop6 = digitalRead(sbStop6);

  int readEmergencyStop = digitalRead(emergencyStopSensor);

  if (readEmergencyStop == LOW) {
    nbTrainSpeed = 0;
    sbTrainSpeed = 0;
    analogWrite(nbPWM, nbTrainSpeed);
    analogWrite(sbPWM, sbTrainSpeed);
    for (int i = 0; i <= 10; i++) {
        digitalWrite(emergencyIndicatorLED, HIGH);
        delay(500);
        digitalWrite(emergencyIndicatorLED, LOW);
        delay(500);
      }
    exit(0);
  }
  
  if ((nbMoving == 1) && millis() >= nbTrainGo) { // Handle Northbound moving train at terminals, check time to ignore repeat inputs.
    if ((readNbSouthTerminal == LOW) or (readNbNorthTerminal == LOW)) {
      nbMoving = 0;
      nbTrainSpeed = 10;
      analogWrite(nbPWM, nbTrainSpeed);
      nbChangeDirection();
      nbTrainStop = (millis() + 4000);
    }
    else if ((millis() >= nbTrainGo) && ((readNbStop1 == LOW) or (readNbStop2 == LOW) or (readNbStop3 == LOW) or (readNbStop4 == LOW) or (readNbStop5 == LOW))) { // Handle Northbound moving train at midstops, check time to ignore repeat inputs.
      nbMoving = 0;
      nbTrainSpeed = 10;
      analogWrite(nbPWM, nbTrainSpeed);
      nbTrainStop = (millis() + 4000);
    }
  } else if ((nbMoving == 0) && (millis() >= nbTrainStop)) { // Handle Northbound stopped train, check time to allow passengers to board.
    nbMoving = 1;
    nbTrainGo = (millis() + 2000);
    nbTrainSpeed = 75;
    analogWrite(nbPWM, nbTrainSpeed);
  }

  if ((sbMoving == 1) && millis() >= sbTrainGo) { // Handle Southbound moving train at terminals, check time to ignore repeat inputs.
    if ((readSbNorthTerminal == LOW) or (readSbSouthTerminal == LOW)) {
      sbMoving = 0;
      sbTrainSpeed = 10;
      analogWrite(sbPWM, sbTrainSpeed);
      sbChangeDirection();
      sbTrainStop = (millis() + 4000);
    }
    else if ((millis() >= sbTrainGo) && ((readSbStop1 == LOW) or (readSbStop2 == LOW) or (readSbStop3 == LOW) or (readSbStop4 == LOW) or (readSbStop5 == LOW))) { // Handle Southbound moving train at midstops, check time to ignore repeat inputs.
      sbMoving = 0;
      sbTrainSpeed = 010;
      analogWrite(sbPWM, sbTrainSpeed);
      sbTrainStop = (millis() + 4000);
    }
  } else if ((sbMoving == 0) && (millis() >= sbTrainStop)) { // Handle Southbound stopped train, check time to allow passengers to board.
    sbMoving = 1;
    sbTrainGo = (millis() + 2000);
    sbTrainSpeed = 85;
    analogWrite(sbPWM, sbTrainSpeed);
  }

  digitalWrite(emergencyIndicatorLED, HIGH); // Turn on loop indicator LED for testing frequency of loop.
  delay(1);
  digitalWrite(emergencyIndicatorLED, LOW); // Turns out it's just pretty much constantly on, so no delay I guess?

}