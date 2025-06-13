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

int nbMoving;
int nbDirection = 1;
int nbEndStop;
int nbMidStop;
int nbTrainSpeed;

int sbMoving;
int sbDirection = 1;
int sbEndStop;
int sbMidStop;
int sbTrainSpeed;

// User definable variables
int maxSpeed = 75;
int embarkTime = 5500;

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
  analogWrite(nbPWM, 010);

  digitalWrite(motorDriverSBA, HIGH);
  digitalWrite(motorDriverSBB, LOW);
  analogWrite(sbPWM, 010);

  delay(2000);
}

// Northbound functions
void nbTrainStart(){
  nbTrainSpeed = 010;
  for (nbTrainSpeed = nbTrainSpeed;
        nbTrainSpeed <= maxSpeed;
        nbTrainSpeed++){
          analogWrite(nbPWM, nbTrainSpeed);
          delay(75);
        }
}

void nbTrainMax(){
  analogWrite(nbPWM, maxSpeed);
  nbMoving = 1;
  nbEndStop = 0;
  nbMidStop = 0;
}

void nbTrainStop(){
  for (nbTrainSpeed = nbTrainSpeed;
          nbTrainSpeed >= 010;
          nbTrainSpeed--){
            analogWrite(nbPWM, nbTrainSpeed);
            delay(30);
          }
  analogWrite(nbPWM, 010);
}

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

// Souththbound functions
void sbTrainStart(){
  sbTrainSpeed = 010;
  for (sbTrainSpeed = sbTrainSpeed;
        sbTrainSpeed <= maxSpeed;
        sbTrainSpeed++){
          analogWrite(sbPWM, sbTrainSpeed);
          delay(75);
        }
}

void sbTrainMax(){
  analogWrite(sbPWM, maxSpeed);
  sbMoving = 1;
  sbEndStop = 0;
  sbMidStop = 0;
}

void sbTrainStop(){
  for (sbTrainSpeed = sbTrainSpeed;
          sbTrainSpeed >= 010;
          sbTrainSpeed--){
            analogWrite(sbPWM, sbTrainSpeed);
            delay(30);
          }
  analogWrite(sbPWM, 010);
}

void sbChangeDirection(){
  if (sbDirection == 1){
    digitalWrite(motorDriverSBA, LOW);
    digitalWrite(motorDriverSBB, HIGH);
    sbDirection = 0;
  } else if (sbDirection == 0) {
    digitalWrite(motorDriverSBA, HIGH);
    digitalWrite(motorDriverSBB, LOW);
    sbDirection = 1;
  }
}

void loop() {

// Reading inputs as part of loop
  int readstartNBA = digitalRead(startNBA);
  int readendNBA = digitalRead(endNBA);
  int readstop1NBA = digitalRead(stop1NBA);
  
  int readstartSBA = digitalRead(startSBA);
  int readendSBA = digitalRead(endSBA);
  int readstop1SBA = digitalRead(stop1SBA);

//Northbound
// End Stop
  if ((readstartNBA == LOW or readendNBA == LOW) && readstop1NBA == HIGH && nbMoving == 1){ // LOW when detecting 
    nbMoving = 0;
    nbTrainStop();
    delay(2000);
    nbChangeDirection();
    nbEndStop = millis();
  }

  if ((readstartNBA == LOW or readendNBA == LOW) && readstop1NBA == HIGH && nbMoving == 0){
    if (millis() - nbEndStop >= embarkTime){
      nbTrainStart();
    }
  }

  if ((readstartNBA == HIGH && readendNBA == HIGH) && readstop1NBA == HIGH && nbMoving == 0){ // HIGH when not detecting
    if (millis() - nbEndStop >= (embarkTime + 5500)){
      nbTrainMax();
    }
  }

// Mid Stop
  if ((readstartNBA == HIGH && readendNBA == HIGH) && readstop1NBA == LOW && nbMoving == 1){
    nbMoving = 0;
    nbTrainStop();
    delay(2000);
    nbMidStop = millis();  
  }

  if ((readstartNBA == HIGH && readendNBA == HIGH) && readstop1NBA == LOW && nbMoving == 0){
    if (millis() - nbMidStop >= embarkTime){
      nbTrainStart();
    }
  }

  if ((readstartNBA == HIGH && readendNBA == HIGH) && readstop1NBA == HIGH && nbMoving == 0){
    if (millis() - nbMidStop >= (embarkTime + 5500)){
      nbTrainMax();
    }
  }

//Southbound
// End Stop
  if ((readstartSBA == LOW or readendSBA == LOW) && readstop1SBA == HIGH && sbMoving == 1){ // LOW when detecting 
    sbMoving = 0;
    sbTrainStop();
    delay(2000);
    sbChangeDirection();
    sbEndStop = millis();
  }

  if ((readstartSBA == LOW or readendSBA == LOW) && readstop1SBA == HIGH && sbMoving == 0){
    if (millis() - sbEndStop >= embarkTime){
      sbTrainStart();
    }
  }

  if ((readstartSBA == HIGH && readendSBA == HIGH) && readstop1SBA == HIGH && sbMoving == 0){ // HIGH when not detecting
    if (millis() - sbEndStop >= (embarkTime + 5500)){
      sbTrainMax();
    }
  }

// Mid Stop
  if ((readstartSBA == HIGH && readendSBA == HIGH) && readstop1SBA == LOW && sbMoving == 1){
    sbMoving = 0;
    sbTrainStop();
    delay(2000);
    sbMidStop = millis();  
  }

  if ((readstartSBA == HIGH && readendSBA == HIGH) && readstop1SBA == LOW && sbMoving == 0){
    if (millis() - sbMidStop >= embarkTime){
      sbTrainStart();
    }
  }

  if ((readstartSBA == HIGH && readendSBA == HIGH) && readstop1SBA == HIGH && sbMoving == 0){
    if (millis() - sbMidStop >= (embarkTime + 5500)){
      sbTrainMax();
    }
  }

}
