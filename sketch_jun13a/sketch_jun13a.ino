#define A 8 // Pinouts for motor driver digital direction control.
#define B 9
#define PWM 7 // Pinout for motor driver analog speed control.
#define Sensor 2 // Pinout for sensor signal.
#define Pwr 25 // "Pinout" for onboard LED.

int moving;
int direction;
int tramStop;
// User definable variables
int maxSpeed = 150;
int embarkTime = 7500;

void setup() {
  // put your setup code here, to run once:
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(Sensor, INPUT);

  pinMode(Pwr, OUTPUT); // Pico onboard LED.
  digitalWrite(25, HIGH); // On for a power indicator.

  delay(2000); // 2 Seconds before doing anything.
  analogWrite(PWM, 010); // Illuminate train lights.
}

void loop() {
  
  digitalWrite(A, HIGH);
  digitalWrite(B, LOW);

  int readSensor = digitalRead(Sensor);

  if (readSensor == LOW && moving == 1){ // LOW when detecting 
    moving = 0;
    analogWrite(PWM, 75);
    delay(1000);
    analogWrite(PWM, 010);
    tramStop = millis();
  }

  if (readSensor == LOW && moving == 0){
    if (millis() - tramStop >= embarkTime){
      analogWrite(PWM, (0.33 * maxSpeed));
      delay(2000);
      analogWrite(PWM, (0.66 * maxSpeed));
    }
  }

  if (readSensor == HIGH && moving == 0){ // HIGH when not detecting
    if (millis() - tramStop >= (embarkTime + 3500)){
      analogWrite(PWM, maxSpeed);
      moving = 1;
      tramStop = 0;
    }
  }
}
  //middleStationOne=0;
  
  //if (sensor==HIGH){
  //  middleStationOne=1;
  //}

  //if (middleStationOne==1){
  //  analogWrite(PWM, 010);
  //  delay(5000);
  //  analogWrite(PWM, 200);
  //  middleStationOne=0;
  //  delay(5000);
  //}

  //if (middleStationOne==0){
  //  analogWrite(PWM, 200);
  //}

