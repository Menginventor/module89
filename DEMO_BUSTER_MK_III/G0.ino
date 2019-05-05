void G0(cmd_class& cmd) {

  
  Serial.print(F("param = "));
  Serial.println(cmd.str());

  if (cmd.find_param('W').val_available) {
    float val = cmd.find_param('W').float_val;
    if (val <= W_MAX && val >= W_MIN) {
      motor_w.goal_pos = val * W_STEP_PER_DEG;
      Serial.println(F("move W"));
    }
    else Serial.println(F("ERR,W LIMIT"));
  }
  if (cmd.find_param('X').val_available) {
    float val = cmd.find_param('X').float_val;
    if (val <= X_MAX && val >= X_MIN) {
      motor_x.goal_pos = val * X_STEP_PER_DEG;
      Serial.println(F("move X"));
    }
    else Serial.println(F("ERR,X LIMIT"));
  }
  if (cmd.find_param('Y').val_available) {
    float val = cmd.find_param('Y').float_val;
    if (val <= Y_MAX && val >= Y_MIN) {
      motor_y.goal_pos = val * Y_STEP_PER_DEG;
      Serial.println(F("move Y"));
    }
    else Serial.println(F("ERR,Y LIMIT"));
  }
  if (cmd.find_param('Z').val_available) {
    float val = cmd.find_param('Z').float_val;
    if (val <= Z_MAX && val >= Z_MIN) {
      motor_z.goal_pos = val * Z_STEP_PER_MM;
      Serial.println(F("move Z"));
    }
    else Serial.println(F("ERR,Z LIMIT"));
  }
  if (motor_w.goal_pos == motor_w.crr_pos && motor_x.goal_pos == motor_x.crr_pos && motor_y.goal_pos == motor_y.crr_pos && motor_z.goal_pos == motor_z.crr_pos) {
    busy_flag = false;
  }
  motor_w.init_conacc();
  motor_x.init_conacc();
  motor_y.init_conacc();
  motor_z.init_conacc();
  while (true) {
    if (motor_w.goal_pos == motor_w.crr_pos && motor_x.goal_pos == motor_x.crr_pos && motor_y.goal_pos == motor_y.crr_pos && motor_z.goal_pos == motor_z.crr_pos) {

      busy_flag = false;
      Serial.println(F("DONE"));
      break;
    } else {
      motor_w.update_conacc();
      motor_x.update_conacc();
      motor_y.update_conacc();
      motor_z.update_conacc();
    }
  }

}
