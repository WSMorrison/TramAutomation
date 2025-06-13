
#define A 8 //'A' and 'B' are direction control pins connected to the motor driver.
#define B 9
#define PWM 7 //Motor speed control pin connected to the 'EN' pin of the motor driver.

void setup() {
  // put your setup code here, to run once:
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);

  delay(200);
}

void loop() {
  
  digitalWrite(A, HIGH);
  digitalWrite(B, LOW);
  analogWrite(PWM, 75);

  delay(600000);
  
  analogWrite(PWM, 200);

  delay(600000);

  analogWrite(PWM, 000);

  delay(50000);

  digitalWrite(A, LOW);
  digitalWrite(B, HIGH);
  analogWrite(PWM, 75);

  delay(600000);
  
  analogWrite(PWM, 200);

  delay(600000);

  analogWrite(PWM, 000);

  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  analogWrite(PWM, 0);

  exit(0);

}