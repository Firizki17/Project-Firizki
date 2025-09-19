void setMotor(float pwmL, float pwmR) {
  pwmL = constrain(pwmL, 0, 255);
  pwmR = constrain(pwmR, 0, 255);
  Serial.print("L: "); Serial.print(pwmL);
  Serial.print(" | R: "); Serial.println(pwmR);

  motor1.setSpeed(pwmL);
  motor2.setSpeed(pwmL);
  motor3.setSpeed(pwmR);
  motor4.setSpeed(pwmR);

  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}
