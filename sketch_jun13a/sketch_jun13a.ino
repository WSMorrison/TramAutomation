#define A 8 // Digital pins for motor driver direction.
#define B 9
#define PWM 7 // Analog pinout for motor driver speed.
#define Sensor 2 // Sensor.

int moving;
int direction = 1;
int scheduledStop;
// User definable variables
int maxSpeed = 150;
int embarkTime = 7500;

void setup() {
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(Sensor, INPUT);

  pinMode(25, OUTPUT);//Added for Pico, sice it has no onboard POWER LED.
  digitalWrite(25, HIGH);//Using the LED on pin GP25 as a power indicator.

  delay(200);
}

void trainStart(){
  analogWrite(PWM, (0.33 * maxSpeed));
  delay(2000);
  analogWrite(PWM, (0.66 * maxSpeed));
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

int toggleDirection(int direction){
  if (direction == 1){
    digitalWrite(A, LOW);
    digitalWrite(B, HIGH);
    direction = 0;
  } else if (direction == 0){
    digitalWrite(A, HIGH);
    digitalWrite(B,LOW);
    direction = 1;
  } else {
    exit(0);
  }
  return direction;
}

void loop() {

  int readSensor = digitalRead(Sensor);

  if (readSensor == LOW && moving == 1){ // LOW when detecting 
    moving = 0;
    trainStop();
    toggleDirection(direction);
    scheduledStop = millis();
  }

  if (readSensor == LOW && moving == 0){
    if (millis() - scheduledStop >= embarkTime){
      trainStart();
    }
  }

  if (readSensor == HIGH && moving == 0){ // HIGH when not detecting
    if (millis() - scheduledStop >= (embarkTime + 3500)){
      trainMax();
    }
  }
}
