int s; //Integer variable to store train's speed in the range from 0 to 255.

#define A 8 //'A' and 'B' are direction control pins connected to the motor driver.
#define B 9
#define PWM 7 //Motor speed control pin connected to the 'EN' pin of the motor driver.
#define Sensor 2 //Sensor
int moving;
int tramStop;

void setup() {
  // put your setup code here, to run once:
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(Sensor, INPUT);

  pinMode(25, OUTPUT);//Added for Pico, sice it has no onboard POWER LED.
  digitalWrite(25, HIGH);//Using the LED on pin GP25 as a power indicator.

  delay(200);
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
    if (millis() - tramStop >= 10000){
      analogWrite(PWM, 050);
      delay(2000);
      analogWrite(PWM, 075);
    }
  }

  if (readSensor == HIGH && moving == 0){ // HIGH when not detecting
    if (millis() - tramStop >= 9000){
      analogWrite(PWM, 150);
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

