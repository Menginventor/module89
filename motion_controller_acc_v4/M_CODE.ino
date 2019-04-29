void M119() {

  Serial.print("Z-ENDSTOP : ");
  Serial.println(digitalRead(ENDSTOP_PIN));
  Serial.print("MOTOR-X : ");
  float val = 0;
  for (int i = 0; i < 100; i++) {
    val += read_ADC(MOTOR_X_ADC);
  }
  val = val / 100;
  Serial.println(val);
  Serial.print("MOTOR-Y : ");
  val = 0;
  for (int i = 0; i < 100; i++) {
    val += read_ADC(MOTOR_Y_ADC);
  }
  val = val / 100;
  Serial.println(val);
  Serial.print("MOTOR-W : ");
  val = 0;
  for (int i = 0; i < 100; i++) {
    val += read_ADC(MOTOR_W_ADC);
  }
  val = val / 100;
  Serial.println(val);
  Serial.print("GRIPPER-SENSOR : ");
  val = 0;
  for (int i = 0; i < 100; i++) {
    val += read_ADC(GRIPPER_ADC);
  }
  val = val / 100;
  Serial.println(val);


}
void M114(){
  Serial.print("MOTOR-X ");
  Serial.println(motor_x.crr_pos/X_STEP_PER_DEG);
  Serial.print("MOTOR-Y ");
  Serial.println(motor_y.crr_pos/Y_STEP_PER_DEG);
  Serial.print("MOTOR-Z ");
  Serial.println(motor_z.crr_pos/Z_STEP_PER_MM);
  Serial.print("MOTOR-W ");
  Serial.println(motor_w.crr_pos/W_STEP_PER_DEG);
}
void M5() { //stop spindle
  digitalWrite(SOLENOID_PIN, LOW);
}
void M3() { //turn on spindle
  digitalWrite(SOLENOID_PIN, HIGH);
}
void M4() { //turn on spindle
  M3();
}
