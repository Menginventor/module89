void serial_loop() {
  static String cmd_str  = "";
  if(buf_full_flag){
    if(cmd_buf.available() < CMD_BUF_SIZE - 1){
      Serial.println("OK");
      buf_full_flag = false;
    }
  }
  if (Serial.available() > 0) {
    char ch_read = Serial.read();
    if (ch_read == '\n') {
      cmd_str.toUpperCase();
      process_cmd_str(cmd_str);
      
      cmd_str = "";
    }
    else if (ch_read != ' ' && ch_read != '\r' ) {
      cmd_str += ch_read;
    }
  }
}
