#define A 8 // Digital pins for motor driver direction.
#define B 9
#define PWM 7 // Analog pinout for motor driver speed.
#define Sensor1 2 // Sensor.
#define Sensor2 0

int moving;
int direction = 1;
int scheduledStop;
// User definable variables
int maxSpeed = 100;
int embarkTime = 5500;

void setup() {
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(Sensor1, INPUT);
  pinMode(Sensor2, INPUT);
  digitalWrite(A, HIGH);
  digitalWrite(B, LOW);
  analogWrite(PWM, 010);

  pinMode(25, OUTPUT);//Added for Pico, sice it has no onboard POWER LED.
  digitalWrite(25, HIGH);//Using the LED on pin GP25 as a power indicator.

  delay(200);
}

void trainStart(){
  analogWrite(PWM, (0.50 * maxSpeed));
  delay(2000);
  analogWrite(PWM, (0.75 * maxSpeed));
}

void trainMax(){
  analogWrite(PWM, maxSpeed);
  moving = 1;
  scheduledStop = 0;
}

void trainStop(){
  analogWrite(PWM, 75);
  delay(1000);
  analogWrite(PWM, 010);
}

void changeDirection(){
  if (direction == 1){
    digitalWrite(A, LOW);
    digitalWrite(B, HIGH);
    direction = 0;
  } else if (direction == 0) {
    digitalWrite(A, HIGH);
    digitalWrite(B, LOW);
    direction = 1;
  }
}

void loop() {

  int readSensor1 = digitalRead(Sensor1);
  int readSensor2 = digitalRead(Sensor2);

  if ((readSensor1 == LOW or readSensor2 == LOW) && moving == 1){ // LOW when detecting 
    moving = 0;
    trainStop();
    delay(2000);
    changeDirection();
    scheduledStop = millis();
  }

  if ((readSensor1 == LOW or readSensor2 == LOW) && moving == 0){
    if (millis() - scheduledStop >= embarkTime){
      trainStart();
    }
  }

  if ((readSensor1 == HIGH && readSensor2 == HIGH) && moving == 0){ // HIGH when not detecting
    if (millis() - scheduledStop >= (embarkTime + 5500)){
      trainMax();
    }
  }
}
