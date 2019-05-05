void M114(){
  Serial.print(F("MOTOR-X "));
  Serial.println(motor_x.crr_pos/X_STEP_PER_DEG);
  Serial.print(F("MOTOR-Y "));
  Serial.println(motor_y.crr_pos/Y_STEP_PER_DEG);
  Serial.print(F("MOTOR-Z "));
  Serial.println(motor_z.crr_pos/Z_STEP_PER_MM);
  Serial.print(F("MOTOR-W "));
  Serial.println(motor_w.crr_pos/W_STEP_PER_DEG);
}
void M119() {

  Serial.print(F("Z-ENDSTOP : "));
  Serial.println(digitalRead(ENDSTOP_PIN));
  Serial.print(F("HOMING_LIMIT_PIN : "));
  Serial.println(digitalRead(HOMING_LIMIT_PIN));



}
void M5() { //stop spindle
  digitalWrite(SOLENOID_PIN, LOW);
  Serial.println(F("DONE"));
}
void M3() { //turn on spindle
  digitalWrite(SOLENOID_PIN, HIGH);
   Serial.println(F("DONE"));
}
void M4() { //turn on spindle
  M3();
}
