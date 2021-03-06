void G0_init(cmd_class& cmd) {
  String w_str = cmd.param_val("W");
  String x_str = cmd.param_val("X");
  String y_str = cmd.param_val("Y");
  String z_str = cmd.param_val("Z");
  /*
    Serial.println("G0 function");
    Serial.println("W = " + w_str);
    Serial.println("X = " + x_str);
    Serial.println("Y = " + y_str);
    Serial.println("Z = " + z_str);
  */
  Serial.print("param = ");
  Serial.println();
  for (int i = 0; i < cmd.param_available(); i++) {
    Serial.print(i);
    Serial.print(" : ");
    Serial.println(cmd.param(i));
  }
  if (w_str.length() > 0) {
    motor_w.goal_pos = w_str.toFloat() * W_STEP_PER_DEG;
    Serial.println("move W");
  }
  if (x_str.length() > 0) {
    motor_x.goal_pos = x_str.toFloat() * X_STEP_PER_DEG;
    Serial.println("move X");
  }
  if (y_str.length() > 0) {
    motor_y.goal_pos = y_str.toFloat() * Y_STEP_PER_DEG;
    Serial.println("move Y");
  }
  if (z_str.length() > 0) {
    motor_z.goal_pos = z_str.toFloat() * Z_STEP_PER_MM;
    Serial.println("move Z");
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
