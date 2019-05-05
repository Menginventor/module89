param_class cmd_class::param(uint8_t n) {
  return cmd_param[n];
}
param_class cmd_class::find_param(char _ins) {

  for (int i = 0; i < param_num; i++) {
    if (cmd_param[i].equal_ins(_ins)) {
      return cmd_param[i];
    }
  }
  return param_class();
}
int cmd_class::param_available() {
  return param_num;
}

void cmd_class::set_cmd(String _cmd_str) {
 
  cmd_class::clear();
  cmd_str = _cmd_str;
  int param_count = -1;

  bool numeric_point_flag = false;//allow one '.' in numeric value.
  //Serial.println(F("set_cmd"));
  //Serial.println(cmd_str);
  String str_hold;
   str_hold.reserve(24);
  for (int i = 0; i < _cmd_str.length(); i++) {
    char ch = _cmd_str.charAt(i);

    if (ch >= 'A' && ch <= 'Z') {

      if (param_count >= 0) {
        //Serial.println(str_hold);
        if (str_hold.length() > 0) {
          cmd_param[param_count].val_available = true;
          cmd_param[param_count].int_val = str_hold.toInt();
          cmd_param[param_count].float_val = str_hold.toFloat();
        }
        str_hold = "";
      }
      param_count++;
      cmd_param[param_count].ins = ch;
      cmd_param[param_count].ins_available = true;


    }
    else {
      str_hold += ch;
      if (i == _cmd_str.length() - 1) {
        //Serial.println(str_hold);
        if (str_hold.length() > 0) {
          cmd_param[param_count].val_available = true;
          cmd_param[param_count].int_val = str_hold.toInt();
          cmd_param[param_count].float_val = str_hold.toFloat();
        }
      }
    }

  }


  param_num = param_count + 1;

}
bool cmd_class::isValid() {
  return !err_flag;
}

void cmd_class::clear() {
  param_num = 0;
  //  cmd_ins = "";
  for (int i = 0; i < CMD_MAX_PARAM; i++) {
    cmd_param[i].ins = ' ';
    cmd_param[i].ins_available = false;
    cmd_param[i].val_available = false;
  }
  err_flag = false;
}
