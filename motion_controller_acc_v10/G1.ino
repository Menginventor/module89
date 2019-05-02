void G1(cmd_class& cmd) {
  busy_flag = false;
  static float feedrate = 10.0;
  /*
    Serial.println("G0 function");
    Serial.println("W = " + w_str);
    Serial.println("X = " + x_str);
    Serial.println("Y = " + y_str);
    Serial.println("Z = " + z_str);
  */
  Serial.print(F("param = "));
  Serial.println(cmd.str());
  if (cmd.find_param('F').val_available) {
    float val = cmd.find_param('F').float_val;
    feedrate = val;
  }
  if (feedrate == 0.0) {
    Serial.println("ERR, G1 F = 0");
    return;
  }
  else {
    if (cmd.find_param('W').val_available) {
      float val = cmd.find_param('W').float_val;
      if (val <= W_MAX && val >= W_MIN) {
        motor_w.goal_pos = val * W_STEP_PER_DEG;
        Serial.println(F("move W"));
      }
      else {
        Serial.println(F("ERR,W LIMIT"));
        return;
      }
    }
    if (cmd.find_param('X').val_available) {
      float val = cmd.find_param('X').float_val;
      if (val <= X_MAX && val >= X_MIN) {
        motor_x.goal_pos = val * X_STEP_PER_DEG;
        Serial.println(F("move X"));
      }
      else {
        Serial.println(F("ERR,X LIMIT"));
        return;
      }
    }
    if (cmd.find_param('Y').val_available) {
      float val = cmd.find_param('Y').float_val;
      if (val <= Y_MAX && val >= Y_MIN) {
        motor_y.goal_pos = val * Y_STEP_PER_DEG;
        Serial.println(F("move Y"));
      }
      else {
        Serial.println(F("ERR,Y LIMIT"));
        return;
      }
    }
    if (cmd.find_param('Z').val_available) {
      float val = cmd.find_param('Z').float_val;
      if (val <= Z_MAX && val >= Z_MIN) {
        motor_z.goal_pos = val * Z_STEP_PER_MM;
        Serial.println(F("move Z"));
      }
      else {
        Serial.println(F("ERR,Z LIMIT"));
        return;
      }
    }

    //
    motor_w.init_convel();
    motor_x.init_convel();
    motor_y.init_convel();
    motor_z.init_convel();

    float w_pos = motor_w.relative_goal_pos / W_STEP_PER_DEG;
    float x_pos = motor_x.relative_goal_pos / X_STEP_PER_DEG;
    float y_pos = motor_y.relative_goal_pos / Y_STEP_PER_DEG;
    float z_pos = motor_z.relative_goal_pos / Z_STEP_PER_MM;

    float diag_pos = sqrt(pow(w_pos, 2) + pow(x_pos, 2) + pow(y_pos, 2) + pow(z_pos, 2));
    if (diag_pos == 0)return;
    motor_w.crr_vel = feedrate * w_pos * W_STEP_PER_DEG / diag_pos;
    motor_x.crr_vel = feedrate * x_pos * X_STEP_PER_DEG / diag_pos;
    motor_y.crr_vel = feedrate * y_pos * Y_STEP_PER_DEG / diag_pos;
    motor_z.crr_vel = feedrate * z_pos * Z_STEP_PER_MM / diag_pos;
    Serial.println(motor_w.crr_vel);
    Serial.println(motor_x.crr_vel);
    Serial.println(motor_y.crr_vel);
    Serial.println(motor_z.crr_vel);
  


    while (true) {
      if (motor_w.goal_pos == motor_w.crr_pos && motor_x.goal_pos == motor_x.crr_pos && motor_y.goal_pos == motor_y.crr_pos && motor_z.goal_pos == motor_z.crr_pos) {

        busy_flag = false;
        Serial.println("DONE");
        break;
      } else {
        motor_w.update_convel();
        motor_x.update_convel();
        motor_y.update_convel();
        motor_z.update_convel();
      }
    }
  }

}
