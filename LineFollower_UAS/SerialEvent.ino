void serialEvent() {
  while (Serial.available()) {
    char cmd = (char)Serial.read();
    cmd = toupper(cmd);  // biar case-insensitive

    switch (cmd) {
      case '1':
        Serial.println("Motor 1 Test");
        motor1.setSpeed(150);
        motor1.run(FORWARD);
        break;
      case '2':
        Serial.println("Motor 2 Test");
        motor2.setSpeed(150);
        motor2.run(FORWARD);
        break;
      case '3':
        Serial.println("Motor 3 Test");
        motor3.setSpeed(150);
        motor3.run(FORWARD);
        break;
      case '4':
        Serial.println("Motor 4 Test");
        motor4.setSpeed(150);
        motor4.run(FORWARD);
        break;
      case 'S':
        Serial.println("Stop Semua");
        motor1.run(RELEASE);
        motor2.run(RELEASE);
        motor3.run(RELEASE);
        motor4.run(RELEASE);
        break;
      case 'F':
        Serial.println("Maju");
        motor1.setSpeed(150);
        motor2.setSpeed(150);
        motor3.setSpeed(150);
        motor4.setSpeed(150);
        motor1.run(FORWARD);
        motor2.run(FORWARD);
        motor3.run(FORWARD);
        motor4.run(FORWARD);
        break;
      case 'B':
        Serial.println("Mundur");
        motor1.setSpeed(150);
        motor2.setSpeed(150);
        motor3.setSpeed(150);
        motor4.setSpeed(150);
        motor1.run(BACKWARD);
        motor2.run(BACKWARD);
        motor3.run(BACKWARD);
        motor4.run(BACKWARD);
        break;
      case 'L':
        Serial.println("Belok Kiri");
        motor1.setSpeed(150);
        motor2.setSpeed(150);
        motor3.setSpeed(150);
        motor4.setSpeed(150);
        motor1.run(BACKWARD);
        motor2.run(BACKWARD);
        motor3.run(FORWARD);
        motor4.run(FORWARD);
        break;
      case 'R':
        Serial.println("Belok Kanan");
        motor1.setSpeed(150);
        motor2.setSpeed(150);
        motor3.setSpeed(150);
        motor4.setSpeed(150);
        motor1.run(FORWARD);
        motor2.run(FORWARD);
        motor3.run(BACKWARD);
        motor4.run(BACKWARD);
        break;
      default:
        Serial.println("Perintah tidak dikenali");
        break;
    }
  }
}
