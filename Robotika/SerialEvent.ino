void serialEvent() {
  while (Serial.available()) {
    char cmd = Serial.read();

    // Debug perintah yg diterima
    Serial.print("Received command: ");
    Serial.println(cmd);

    if (cmd == 'f') {
      Serial.println("Moving Forward");
      moveForward();
    }
    else if (cmd == 'b') {
      Serial.println("Moving Backward");
      moveBackward();
    }
    else if (cmd == 'l') {
      Serial.println("Turning Left");
      moveLeft();
    }
    else if (cmd == 'r') {
      Serial.println("Turning Right");
      moveRight();
    }
    else if (cmd == 's') {
      Serial.println("Stop");
      Stop();
    }
  }
}
