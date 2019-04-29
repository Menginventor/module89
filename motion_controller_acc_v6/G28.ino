void G28_X() {
  float val = 0;
  for (int i = 0; i < 100; i++) {
    val += read_ADC(MOTOR_X_ADC);
  }
  val = val / 100;

  motor_x.crr_pos = float_map(val, ADC_X_MAX, ADC_X_MIN, 90, -90) * X_STEP_PER_DEG;
  motor_x.goal_pos =  motor_x.crr_pos ;
}
void G28_Y() {
  float val = 0;
  for (int i = 0; i < 100; i++) {
    val += read_ADC(MOTOR_Y_ADC);
  }
  val = val / 100;

  motor_y.crr_pos = float_map(val, ADC_Y_MAX, ADC_Y_MIN, 90, -90) * Y_STEP_PER_DEG;
  motor_y.goal_pos =  motor_y.crr_pos ;
}
void G28_W() {
  float val = 0;
  for (int i = 0; i < 100; i++) {
    val += read_ADC(MOTOR_W_ADC);
  }
  val = val / 100;

  motor_w.crr_pos = float_map(val, ADC_W_MAX, ADC_W_MIN, 90, -90) * W_STEP_PER_DEG;
  motor_w.goal_pos =  motor_w.crr_pos ;
}
void G28_Z() {
  busy_flag = true;
  port_write(STATUS_LED_PIN, HIGH);
  motor_z.crr_pos = 0;
  motor_z.goal_pos = Z_MAX * Z_STEP_PER_MM;
  if (motor_w.goal_pos == motor_w.crr_pos && motor_x.goal_pos == motor_x.crr_pos && motor_y.goal_pos == motor_y.crr_pos && motor_z.goal_pos == motor_z.crr_pos) {
    busy_flag = false;
  }
}
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
  bool HOME_X = false;
  bool HOME_Y = false;
  bool HOME_Z = false;
  bool HOME_W = false;
  for (int i = 0; i < cmd.param_available(); i++) {

    if (cmd.param(i).equals("X")) {
      HOME_X = true;
    }
    else if (cmd.param(i).equals("Y")) {
      HOME_Y = true;
    }
    else if (cmd.param(i).equals("Z")) {
      HOME_Z = true;

    }
    else if (cmd.param(i).equals("W")) {
      HOME_W = true;
    }
  }
  if (!HOME_X && !HOME_Y && !HOME_Z && !HOME_W ) {
    Serial.println("Home All");
    G28_X();
    G28_Y();
    G28_W();

    G28_Z();


  }
  else {
    if (HOME_X) {
      Serial.println("HOME_X");
      G28_X();
    }
    if (HOME_Y) {
      Serial.println("HOME_Y");
      G28_Y();
    }
    if (HOME_Z) {
      Serial.println("HOME_Z");
      G28_Z();
    }
    if (HOME_W) {
      Serial.println("HOME_W");
      G28_W();
    }
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
    busy_flag = false;
  } else {
    /*
        motor_w.update_conacc();
        motor_x.update_conacc();
        motor_y.update_conacc();
    */

    motor_z.update_conacc();
    if (digitalRead(ENDSTOP_PIN) == HIGH) {
      Serial.println("Homing complete!");
      busy_flag = false;
      motor_z.goal_pos = Z_MAX * Z_STEP_PER_MM;
      motor_z.crr_pos = Z_MAX * Z_STEP_PER_MM;
      Serial.print("CRR-Z = ");
      Serial.println(motor_z.crr_pos / Z_STEP_PER_MM);
    }


  }

}
