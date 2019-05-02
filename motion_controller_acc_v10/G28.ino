void G28_X() {
  Serial.println(F("HOME-X"));
  port_write(motor_x.dir_pin, X_HOMING_DIR ^ motor_x.dir_inv);
  while (digitalRead(HOMING_LIMIT_PIN) == HIGH) {
    port_write(motor_x.pul_pin, LOW);
    delayMicroseconds(1000000.0f / ( X_HOMING_VEL * X_STEP_PER_DEG));
    port_write(motor_x.pul_pin, HIGH);
    delayMicroseconds(1000000.0f / ( X_HOMING_VEL * X_STEP_PER_DEG));

  }
  motor_x.crr_pos = X_HOMING_POS * X_STEP_PER_DEG;
  motor_x.goal_pos = 0;
  motor_x.init_conacc();
  while (motor_x.goal_pos != motor_x.crr_pos) {
    motor_x.update_conacc();
  }
}
void G28_Y() {
  Serial.println(F("HOME-Y"));
  port_write(motor_y.dir_pin, Y_HOMING_DIR ^ motor_y.dir_inv);
  while (digitalRead(HOMING_LIMIT_PIN) == HIGH) {
    port_write(motor_y.pul_pin, LOW);
    delayMicroseconds(1000000.0f / ( Y_HOMING_VEL * Y_STEP_PER_DEG));
    port_write(motor_y.pul_pin, HIGH);
    delayMicroseconds(1000000.0f / ( Y_HOMING_VEL * Y_STEP_PER_DEG));

  }
  motor_y.crr_pos = Y_HOMING_POS * Y_STEP_PER_DEG;
  motor_y.goal_pos = 0;
  motor_y.init_conacc();
  while (motor_y.goal_pos != motor_y.crr_pos) {
    motor_y.update_conacc();
  }
}
void G28_W() {
  Serial.println(F("HOME-W"));
  port_write(motor_w.dir_pin, W_HOMING_DIR ^ motor_w.dir_inv);
  while (digitalRead(HOMING_LIMIT_PIN) == HIGH) {
    port_write(motor_w.pul_pin, LOW);
    delayMicroseconds(1000000.0f / ( W_HOMING_VEL * W_STEP_PER_DEG));
    port_write(motor_w.pul_pin, HIGH);
    delayMicroseconds(1000000.0f / ( W_HOMING_VEL * W_STEP_PER_DEG));

  }
  motor_w.crr_pos = W_HOMING_POS * W_STEP_PER_DEG;
  motor_w.goal_pos = 0;
  motor_w.init_conacc();
  while (motor_w.goal_pos != motor_w.crr_pos) {
    motor_w.update_conacc();

  }
}

void G28_Z() {
  Serial.println(F("HOME-Z"));
  port_write(STATUS_LED_PIN, HIGH);
  motor_z.crr_pos = 0;
  motor_z.goal_pos = (Z_MAX+100) * Z_STEP_PER_MM;
  motor_z.init_conacc();
  while (motor_z.goal_pos != motor_z.crr_pos) {
    motor_z.update_conacc();
    if (digitalRead(ENDSTOP_PIN) == HIGH) {
      Serial.println("Homing complete!");


      motor_z.goal_pos = Z_MAX * Z_STEP_PER_MM;
      motor_z.crr_pos = Z_MAX * Z_STEP_PER_MM;
      Serial.print("CRR-Z = ");
      Serial.println(motor_z.crr_pos / Z_STEP_PER_MM);
      break;
    }


  }
}
void G28_init(cmd_class& cmd) {

  /*
    String w_str = cmd.param_val('W');
    String x_str = cmd.param_val('X');
    String y_str = cmd.param_val('Y');
    String z_str = cmd.param_val('Z');
  */
  /*
    Serial.println("G0 function");
    Serial.println("W = " + w_str);
    Serial.println("X = " + x_str);
    Serial.println("Y = " + y_str);
    Serial.println("Z = " + z_str);
  */
  bool HOME_X = cmd.ins_available('X');
  bool HOME_Y = cmd.ins_available('Y');
  bool HOME_Z = cmd.ins_available('Z');
  bool HOME_W = cmd.ins_available('W');
  /*
    for (int i = 0; i < cmd.param_available(); i++) {

    if (cmd.param(i).ins == 'X') {
      HOME_X = true;
    }
    else if (cmd.param(i).ins == 'Y') {
      HOME_Y = true;
    }
    else if (cmd.param(i).ins == 'Z') {
      HOME_Z = true;

    }
    else if (cmd.param(i).ins == 'W') {
      HOME_W = true;
    }
    }
  */
  if (!HOME_X && !HOME_Y && !HOME_Z && !HOME_W ) {
    Serial.println("Home All");


    G28_Z();

    G28_W();

    G28_Y();
    G28_X();


  }
  else {
    if (HOME_Z) {
   
      G28_Z();
    }
    if (HOME_W) {
  
      G28_W();
    }

    if (HOME_Y) {

      G28_Y();
    }
    if (HOME_X) {
     
      G28_X();
    }


  }




  /*
    motor_w.init_conacc();
    motor_x.init_conacc();
    motor_y.init_conacc();
  */
  
  Serial.println("DONE");
}
