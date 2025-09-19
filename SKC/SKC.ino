#include <Servo.h>
#include <AFMotor.h>

#define TRIG_PIN A0
#define ECHO_PIN A1
#define IR_LEFT A3
#define IR_RIGHT A2

// Enum arah motor untuk efisiensi
enum Direction { STOP, FORWARD, LEFT, RIGHT };

struct FuzzyOutput {
  Direction direction;
  int speed;  // 0-255
};

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

void setup() {
  Serial.begin(9600);
  setupMotors();
  setupSensors();
}

void loop() {
  int dist = readUltrasonic();
  int irL = digitalRead(IR_LEFT);
  int irR = digitalRead(IR_RIGHT);

  FuzzyOutput out = fuzzyInference(dist, irL, irR);
  controlMotor(out);

  delay(100);
}

void setupMotors() {
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  motor3.setSpeed(0);
  motor4.setSpeed(0);
}

void setupSensors() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);
}

int readUltrasonic() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 20000);
  int distance = duration * 0.034 / 2;
  if (distance == 0 || distance > 100) distance = 100;
  return distance;
}

// Fungsi keanggotaan fuzzy
float membershipNear(int dist) {
  if (dist <= 10) return 1.0;
  else if (dist > 10 && dist <= 20) return (20 - dist) / 10.0;
  else return 0.0;
}

float membershipMedium(int dist) {
  if (dist > 10 && dist <= 20) return (dist - 10) / 10.0;
  else if (dist > 20 && dist <= 40) return (40 - dist) / 20.0;
  else return 0.0;
}

float membershipFar(int dist) {
  if (dist > 30 && dist <= 50) return (dist - 30) / 20.0;
  else if (dist > 50) return 1.0;
  else return 0.0;
}

FuzzyOutput fuzzyInference(int dist, int irL, int irR) {
  FuzzyOutput output;

  float near = membershipNear(dist);
  float medium = membershipMedium(dist);
  float far = membershipFar(dist);

  if (near > 0.5) {
    output.direction = STOP;
    output.speed = 0;
  } else if (medium > 0.5) {
    if (irL == HIGH && irR == LOW) {
      output.direction = RIGHT;
      output.speed = 150;
    } else if (irR == HIGH && irL == LOW) {
      output.direction = LEFT;
      output.speed = 150;
    } else {
      output.direction = FORWARD;
      output.speed = 100;
    }
  } else if (far > 0.5) {
    output.direction = FORWARD;
    output.speed = 255;
  } else {
    output.direction = FORWARD;
    output.speed = 150;
  }

  Serial.print("Fuzzy Decision: ");
  switch (output.direction) {
    case STOP: Serial.print("STOP"); break;
    case FORWARD: Serial.print("FORWARD"); break;
    case LEFT: Serial.print("LEFT"); break;
    case RIGHT: Serial.print("RIGHT"); break;
  }
  Serial.print(", Speed: ");
  Serial.println(output.speed);

  return output;
}

void controlMotor(FuzzyOutput out) {
  int speed = out.speed;
  switch (out.direction) {
    case STOP:
      motor1.run(RELEASE);
      motor2.run(RELEASE);
      motor3.run(RELEASE);
      motor4.run(RELEASE);
      break;

    case FORWARD:
      motor1.setSpeed(speed);
      motor2.setSpeed(speed);
      motor3.setSpeed(speed);
      motor4.setSpeed(speed);
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      motor3.run(FORWARD);
      motor4.run(FORWARD);
      break;

    case LEFT:
      motor1.setSpeed(speed);
      motor2.setSpeed(speed);
      motor3.setSpeed(speed);
      motor4.setSpeed(speed);
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      motor3.run(BACKWARD);
      motor4.run(BACKWARD);
      break;

    case RIGHT:
      motor1.setSpeed(speed);
      motor2.setSpeed(speed);
      motor3.setSpeed(speed);
      motor4.setSpeed(speed);
      motor1.run(BACKWARD);
      motor2.run(BACKWARD);
      motor3.run(FORWARD);
      motor4.run(FORWARD);
      break;
  }
}
