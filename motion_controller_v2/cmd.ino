String cmd_class::param(uint8_t n) {
  return cmd_param[n];
}
String cmd_class::param_val(String param_name) {

  for(int i = 0;i<param_num;i++){
    if(cmd_param[i].startsWith(param_name)){
      return cmd_param[i].substring(1);
    }
  }
  return "";
}
int cmd_class::param_available() {
  return param_num;
}
void cmd_class::set_cmd(String _cmd_str) {
  //test g0  x123.456 y-456.789 z.789
  cmd_class::clear();
  cmd_str = _cmd_str;
  int param_count = 0;
  int param_ch_count = 0;
  bool numeric_piont_flag = false;//allow one '.' in numeric value.
  for (int i = 0; i < _cmd_str.length(); i++) {
    char ch = _cmd_str.charAt(i);
    if (param_ch_count == 0) {
      if (ch >= 'A' && ch <= 'Z') {
        cmd_param[param_count] += ch;
      }
      else {
        err_flag = true;
        //Serial.println("err 0");
        return;
      }
      param_ch_count++;
    }
    else if (param_ch_count == 1) {
      if ((ch >= '0' && ch <= '9') || ch == '-' || ch == '+') {
        cmd_param[param_count] += ch;
      }
      else if ( ch == '.') {
        if (numeric_piont_flag) {
          err_flag = true;
          //Serial.println("err 1");
          return;
        }
        else {
          cmd_param[param_count] += ch;
          numeric_piont_flag = true;
        }

      }
      param_ch_count++;
    }
    else {
      if (ch >= '0' && ch <= '9') {
        cmd_param[param_count] += ch;
        param_ch_count++;
      }
      else if ( ch == '.') {
        if (numeric_piont_flag) {
          err_flag = true;
          //Serial.println("err 2");
          return;
        }
        else {
          cmd_param[param_count] += ch;
          numeric_piont_flag = true;
        }
        param_ch_count++;
      }
      else if (ch >= 'A' && ch <= 'Z') {
        param_count++;
        param_ch_count = 0;
        numeric_piont_flag = false;
        /**/
        if (ch >= 'A' && ch <= 'Z') {
          cmd_param[param_count] += ch;
        }
        else {
          err_flag = true;
          //Serial.println("err 3");
          return;
        }
        param_ch_count++;
        /**/
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
  cmd_ins = "";
  for (int i = 0; i < CMD_MAX_PARAM; i++) {
    cmd_param[i] = "";
  }
  err_flag = false;
}
