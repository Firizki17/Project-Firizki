#include <Servo.h>
#include <AFMotor.h>

// Sensor Ultrasonik
#define TRIGGER_PIN A0
#define ECHO_PIN A1

// Sensor Garis
#define irLeft A4
#define irRight A3

Servo servo;
AFMotor motor1(1, MOTOR12_1KHZ);
AFMotor motor2(2, MOTOR12_1KHZ);
AFMotor motor3(3, MOTOR34_1KHZ);
AFMotor motor4(4, MOTOR34_1KHZ);

int distance = 0;
int position = 0;

void setup() {
  Serial.begin(9600);
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  servo.attach(10);
  servo.write(90);
}

void loop() {
  distance = getDistance();
  position = getPosition();
  fuzzyControl(distance, position);
  delay(50);
}
