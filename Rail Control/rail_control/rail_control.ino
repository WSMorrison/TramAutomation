//#define pwrLight LED
#define motorDriverNBA 27 // Digital pins for motor driver direction.
#define motorDriverNBB 26
#define PWM 28 // Analog pinout for motor driver speed.
#define startNBA 0 // Northbound Start A Sensor
#define endNBA 6 // Northbound End A Sensor
#define stop1NBA 2 // Northbound Stop 1 A Sensor
#define stop2NBA 4 // Northbound Stop 2 A Sensor

int moving;
int direction = 1;
int nBEndStop;
int nBMidStop;
int trainSpeed;
// User definable variables
int maxSpeed = 75;
int embarkTime = 5500;

void setup() {
  pinMode(motorDriverNBA, OUTPUT);
  pinMode(motorDriverNBB, OUTPUT);
  pinMode(startNBA, INPUT);
  pinMode(endNBA, INPUT);
  pinMode(stop1NBA, INPUT);
  pinMode(stop2NBA, INPUT);
  digitalWrite(motorDriverNBA, HIGH);
  digitalWrite(motorDriverNBB, LOW);
  analogWrite(PWM, 010);

  //pinMode(pwrLight, OUTPUT);//Added for Pico, sice it has no onboard POWER LED.
  //digitalWrite(pwrLight, HIGH);//Using the LED on pin GP25 as a power indicator.

  delay(200);
}

void trainStart(){
  trainSpeed = 010;
  for (trainSpeed = trainSpeed;
        trainSpeed <= maxSpeed;
        trainSpeed++){
          analogWrite(PWM, trainSpeed);
          delay(75);
        }
  //analogWrite(PWM, (0.50 * maxSpeed));
  //delay(2500);
  //analogWrite(PWM, (0.75 * maxSpeed));
}

void trainMax(){
  analogWrite(PWM, maxSpeed);
  moving = 1;
  nBEndStop = 0;
  nBMidStop = 0;
}

void trainStop(){
  for (trainSpeed = trainSpeed;
          trainSpeed >= 010;
          trainSpeed--){
            analogWrite(PWM, trainSpeed);
            delay(30);
          }
  analogWrite(PWM, 010);
  //analogWrite(PWM, (0.50 * maxSpeed));
  //delay(1000);
  //analogWrite(PWM, 010);
}

void changeDirection(){
  if (direction == 1){
    digitalWrite(motorDriverNBA, LOW);
    digitalWrite(motorDriverNBB, HIGH);
    direction = 0;
  } else if (direction == 0) {
    digitalWrite(motorDriverNBA, HIGH);
    digitalWrite(motorDriverNBB, LOW);
    direction = 1;
  }
}

void loop() {

// Reading inputs as part of loop
  int readstartNBA = digitalRead(startNBA);
  int readendNBA = digitalRead(endNBA);
  int readstop1NBA = digitalRead(stop1NBA);
  int readstop2NBA = digitalRead(stop2NBA);

// End Stop
  if (((readstartNBA == LOW or readendNBA == LOW) && (readstop1NBA == HIGH && readstop2NBA == HIGH)) && moving == 1){ // LOW when detecting 
    moving = 0;
    trainStop();
    delay(2000);
    changeDirection();
    nBEndStop = millis();
  }

  if (((readstartNBA == LOW or readendNBA == LOW) && (readstop1NBA == HIGH && readstop2NBA == HIGH)) && moving == 0){
    if (millis() - nBEndStop >= embarkTime){
      trainStart();
    }
  }

  if (((readstartNBA == HIGH && readendNBA == HIGH) && (readstop1NBA == HIGH && readstop2NBA == HIGH)) && moving == 0){ // HIGH when not detecting
    if (millis() - nBEndStop >= (embarkTime + 5500)){
      trainMax();
    }
  }

// Mid Stop
  if ((readstartNBA == HIGH && readendNBA == HIGH) && (readstop1NBA == LOW or readstop2NBA == LOW) && moving == 1){
    moving = 0;
    trainStop();
    delay(2000);
    nBMidStop = millis();  
  }

  if ((readstartNBA == HIGH && readendNBA == HIGH) && (readstop1NBA == LOW or readstop2NBA == LOW) && moving == 0){
    if (millis() - nBMidStop >= embarkTime){
      trainStart();
    }
  }

  if ((readstartNBA == HIGH && readendNBA == HIGH) && (readstop1NBA == HIGH && readstop2NBA == HIGH) && moving == 0){
    if (millis() - nBMidStop >= (embarkTime + 5500)){
      trainMax();
    }
  }

}
