void G28_init(cmd_class cmd) {


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
  /*
    if (w_str.length() > 0) {
    motor_w.goal_pos = w_str.toInt();


    }
    if (x_str.length() > 0) {
    motor_x.goal_pos = x_str.toInt();

    }
    if (y_str.length() > 0) {
    motor_y.goal_pos = y_str.toInt();

    }

    if (z_str.length() > 0) {
    motor_z.goal_pos = z_str.toInt();

    }
  */
  motor_z.goal_pos = 30000;
  if (motor_w.goal_pos == motor_w.crr_pos && motor_x.goal_pos == motor_x.crr_pos && motor_y.goal_pos == motor_y.crr_pos && motor_z.goal_pos == motor_z.crr_pos) {
    busy_flag = false;
  }

  motor_w.init_conacc();
  motor_x.init_conacc();
  motor_y.init_conacc();
  motor_z.init_conacc();
  Serial.println("Homing");
}

void G28_loop(cmd_class cmd) {
  //Serial.println("G28 loop");
  if (motor_w.goal_pos == motor_w.crr_pos && motor_x.goal_pos == motor_x.crr_pos && motor_y.goal_pos == motor_y.crr_pos && motor_z.goal_pos == motor_z.crr_pos) {

  } else {
    /*
        motor_w.update_conacc();
        motor_x.update_conacc();
        motor_y.update_conacc();
    */

    motor_z.update_conacc();
  if (digitalRead(ENDSTOP_PIN) == HIGH) {
      busy_flag = false;
      Serial.println("Homing complete!");
      motor_z.goal_pos = Z_MAX;
       motor_z.crr_pos = Z_MAX;
    }


  }

}
