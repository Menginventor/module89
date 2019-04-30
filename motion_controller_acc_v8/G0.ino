void G0_init(cmd_class& cmd) {

  /*
    Serial.println("G0 function");
    Serial.println("W = " + w_str);
    Serial.println("X = " + x_str);
    Serial.println("Y = " + y_str);
    Serial.println("Z = " + z_str);
  */
  Serial.print(F("param = "));
  Serial.println(cmd.str());
  
  if (cmd.find_param('W').val_available) {
    
    motor_w.goal_pos = cmd.find_param('W').float_val* W_STEP_PER_DEG;
    Serial.println(F("move W"));
  }
  if (cmd.find_param('X').val_available) {
    motor_x.goal_pos = cmd.find_param('X').float_val * X_STEP_PER_DEG;
    Serial.println(F("move X"));
  }
  if (cmd.find_param('Y').val_available) {
    motor_y.goal_pos = cmd.find_param('Y').float_val * Y_STEP_PER_DEG;
    Serial.println(F("move Y"));
  }
  if (cmd.find_param('Z').val_available) {
    motor_z.goal_pos = cmd.find_param('Z').float_val * Z_STEP_PER_MM;
    Serial.println(F("move Z"));
  }
  if (motor_w.goal_pos == motor_w.crr_pos && motor_x.goal_pos == motor_x.crr_pos && motor_y.goal_pos == motor_y.crr_pos && motor_z.goal_pos == motor_z.crr_pos) {
    busy_flag = false;
  }
  motor_w.init_conacc();
  motor_x.init_conacc();
  motor_y.init_conacc();
  motor_z.init_conacc();

}

void G0_loop(cmd_class& cmd) {
  if (motor_w.goal_pos == motor_w.crr_pos && motor_x.goal_pos == motor_x.crr_pos && motor_y.goal_pos == motor_y.crr_pos && motor_z.goal_pos == motor_z.crr_pos) {

    busy_flag = false;
  } else {
    motor_w.update_conacc();
    motor_x.update_conacc();
    motor_y.update_conacc();
    motor_z.update_conacc();
  }

}
