int getDistance() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  int cm = duration * 0.034 / 2;
  if (cm == 0) cm = 100;
  return cm;
}

int getPosition() {
  int l = digitalRead(irLeft);
  int r = digitalRead(irRight);

  if (l == 0 && r == 1) return -1;  
  else if (l == 1 && r == 0) return 1;
  else return 0;
}
