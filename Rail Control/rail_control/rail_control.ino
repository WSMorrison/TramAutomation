#define motorDriverNBA 27 // Digital pins for motor driver direction.
#define motorDriverNBB 26
#define motorDriverSBA 21
#define motorDriverSBB 20
#define nbPWM 28 // Analog pinout for motor driver speed.
#define sbPWM 22
#define startNBA 0 // Northbound Start A Sensor
#define endNBA 6 // Northbound End A Sensor
#define stop1NBA 2 // Northbound Stop 1 A Sensor
#define startSBA 8 // Southbound Start A Sensor
#define endSBA 10 // Souththbound End A Sensor
#define stop1SBA 14 // Souththbound Stop 1 A Sensor

int nbMoving = 0;
int nbDirection = 1;
int nbEndStop;
int nbMidStop;
int nbTrainSpeed;
int nbStarting = 0;

int sbMoving = 0;
int sbDirection = 1;
int sbEndStop;
int sbMidStop;
int sbTrainSpeed;
int sbStarting = 0;

// Future iteration user definable variables
int maxSpeed = 75;
int embarkTime = 5500;

void nbChangeDirection(){
  if (nbDirection == 1){
    digitalWrite(motorDriverNBA, LOW);
    digitalWrite(motorDriverNBB, HIGH);
    nbDirection = 0;
  } else if (nbDirection == 0) {
    digitalWrite(motorDriverNBA, HIGH);
    digitalWrite(motorDriverNBB, LOW);
    nbDirection = 1;
  }
}

void sbChangeDirection(){
  if (nbDirection == 1){
    digitalWrite(motorDriverSBA, LOW);
    digitalWrite(motorDriverSBB, HIGH);
    sbDirection = 0;
  } else if (nbDirection == 0) {
    digitalWrite(motorDriverSBA, HIGH);
    digitalWrite(motorDriverSBB, LOW);
    sbDirection = 1;
  }
}

void emergencyStop(){
  analogWrite(nbPWM, 000);
  analogWrite(sbPWM, 000);
  // Future iteration turn on an indicator light or something
  exit(0);
}

void setup() {
  pinMode(motorDriverNBA, OUTPUT);
  pinMode(motorDriverNBB, OUTPUT);
  pinMode(startNBA, INPUT);
  pinMode(endNBA, INPUT);
  pinMode(stop1NBA, INPUT);
  
  pinMode(motorDriverSBA, OUTPUT);
  pinMode(motorDriverSBB, OUTPUT);
  pinMode(startSBA, INPUT);
  pinMode(endSBA, INPUT);
  pinMode(stop1SBA, INPUT);

  digitalWrite(motorDriverNBA, HIGH);
  digitalWrite(motorDriverNBB, LOW);
  analogWrite(nbPWM, 010); // Enough voltage to turn on the lights but not drive the train

  digitalWrite(motorDriverSBA, HIGH);
  digitalWrite(motorDriverSBB, LOW);
  analogWrite(sbPWM, 010); // Enough voltage to turn on the lights but not drive the train

  delay(2000); // Two seconds before start

}

void loop() {

// Reading inputs as part of loop
  int readstartNBA = digitalRead(startNBA);
  int readendNBA = digitalRead(endNBA);
  int readstop1NBA = digitalRead(stop1NBA);
  
  int readstartSBA = digitalRead(startSBA);
  int readendSBA = digitalRead(endSBA);
  int readstop1SBA = digitalRead(stop1SBA);

// Handle NB moving train
  // If in starting 
  if ((nbStarting == 1) && (nbTrainSpeed < maxSpeed)) { // If train is in starting, increment speed and ignore sensors.
    //nbTrainSpeed++;
    nbTrainSpeed = maxSpeed;
    analogWrite(nbPWM, nbTrainSpeed);
  } else if ((nbStarting == 1) && (nbTrainSpeed >= maxSpeed)) { // If train is at maximum speed, no incrementing, no longer "starting"
    nbTrainSpeed = maxSpeed;
    analogWrite(nbPWM, nbTrainSpeed);
    nbStarting == 0;
  // If not in starting, check stop sensors
  } else if (nbStarting == 0){
    // If at end stop
    if ((nbMoving == 1) && ((readstartNBA == LOW or readendNBA == LOW) && readstop1NBA == HIGH)) { // Low when train present, hight when not
      if (nbTrainSpeed > 10) {
        //nbTrainSpeed--;
        nbTrainSpeed = 010;
        analogWrite(nbPWM, nbTrainSpeed);
      } else if (nbTrainSpeed <= 10) {
        nbTrainSpeed = 10;
        analogWrite(nbPWM, nbTrainSpeed);
        nbMoving = 0;
        nbEndStop = millis();
        nbChangeDirection();
      } else {
        emergencyStop();
      }
    } 
    // If at middle stop
    else if ((nbMoving == 1) && (readstop1NBA == LOW && (readstartNBA == HIGH && readendNBA == HIGH))) {
      if (nbTrainSpeed > 10) {
        //nbTrainSpeed--;
        nbTrainSpeed = 010;
        analogWrite(nbPWM, nbTrainSpeed);
      } else if (nbTrainSpeed <= 10) {
       nbTrainSpeed = 10;
       analogWrite(nbPWM, nbTrainSpeed);
        nbMoving = 0;
        nbMidStop = millis();
      } else {
       emergencyStop();
      }
    }
  }

// Handle SB moving train
  // If in starting 
  if ((sbStarting == 1) && (sbTrainSpeed < maxSpeed)) { // If train is in starting, increment speed and ignore sensors.
    //sbTrainSpeed++;
    sbTrainSpeed = maxSpeed;
    analogWrite(sbPWM, sbTrainSpeed);
  } else if ((sbStarting == 1) && (sbTrainSpeed >= maxSpeed)) { // If train is at maximum speed, no incrementing, no longer "starting"
    sbTrainSpeed = maxSpeed;
    analogWrite(sbPWM, sbTrainSpeed);
    sbStarting == 0;
  // If not in starting, check stop sensors
  } else if (sbStarting == 0){
    // If at end stop
    if ((sbMoving == 1) && ((readstartSBA == LOW or readendSBA == LOW) && readstop1SBA == HIGH)) { // Low when train present, hight when not
      if (sbTrainSpeed > 10) {
        //sbTrainSpeed--;
        sbTrainSpeed = 010;
        analogWrite(sbPWM, sbTrainSpeed);
      } else if (sbTrainSpeed <= 10) {
        sbTrainSpeed = 10;
        analogWrite(sbPWM, sbTrainSpeed);
        sbMoving = 0;
        sbEndStop = millis();
        sbChangeDirection();
      } else {
        emergencyStop();
      }
    } 
    // If at middle stop
    else if ((sbMoving == 1) && (readstop1SBA == LOW && (readstartSBA == HIGH && readendSBA == HIGH))) {
      if (sbTrainSpeed > 10) {
        //sbTrainSpeed--;
        sbTrainSpeed = 010;
        analogWrite(sbPWM, sbTrainSpeed);
      } else if (sbTrainSpeed <= 10) {
       sbTrainSpeed = 10;
       analogWrite(sbPWM, sbTrainSpeed);
        sbMoving = 0;
        sbMidStop = millis();
      } else {
       emergencyStop();
      }
    }
  }

// Handle NB stopped train.
  if ((nbMoving == 0) && (millis() - nbEndStop >= embarkTime)) {
    nbStarting = 1;
    nbTrainSpeed = 11;
    analogWrite(nbPWM, nbTrainSpeed);
  } else if ((nbMoving == 0) && (millis() - nbMidStop >= embarkTime)) {
    nbStarting = 1;
    nbTrainSpeed = 11;
    analogWrite(nbPWM, nbTrainSpeed);
  }

// Handle /SB stopped train.
  if ((sbMoving == 0) && (millis() - sbEndStop >= embarkTime)) {
    sbStarting = 1;
    sbTrainSpeed = 11;
    analogWrite(sbPWM, sbTrainSpeed);
  } else if ((sbMoving == 0) && (millis() - sbMidStop >= embarkTime)) {
    sbStarting = 1;
    sbTrainSpeed = 11;
    analogWrite(sbPWM, sbTrainSpeed);
  }

}
