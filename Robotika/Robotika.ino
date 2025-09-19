#include <Servo.h>
#include <AFMotor.h>

// === Ultrasonik ===
#define TRIGGER_PIN A0
#define ECHO_PIN A1

// === IR sensor ===
#define irLeft A4
//#define irCenter A2
#define irRight A3

Servo servo;
AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

int distance = 0;
int leftDistance = 0;
int rightDistance = 0;
bool object = false;

void setup() {
  Serial.begin(9600);

  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  servo.attach(10);
  servo.write(90);

  motor1.setSpeed(80);
  motor2.setSpeed(80);
  motor3.setSpeed(80);
  motor4.setSpeed(80);
}

void loop() {
  distance = getDistance();
  
  if (distance <= 15) {
    objectAvoid();
    return;
  }

  int left = digitalRead(irLeft);
  int right = digitalRead(irRight);

  Serial.print("IR Left: "); Serial.print(left);
  Serial.print(" | IR Right: "); Serial.println(right);

  if (left == 0 && right == 1) {
    // Kiri kena garis, belok kiri
    moveLeft();
  } 
  else if (right == 0 && left == 1) {
    // Kanan kena garis, belok kanan
    moveRight();
  } 
  else if (left == 1 && right == 1) {
    // Tidak kena garis, jalan lurus
    moveForward();
  }
  else if (left == 0 && right == 0) {
    // Keduanya kena garis (misal simpangan): maju pelan
    moveForward();
  }
  else {
    Stop();
  }

  delay(20);
  serialEvent();
}


void objectAvoid() {
  Stop();
  lookLeft();
  lookRight();
  delay(100);

  if (rightDistance <= leftDistance) {
    object = true;
    turn();
  } else {
    object = false;
    turn();
  }

  delay(100);
}
