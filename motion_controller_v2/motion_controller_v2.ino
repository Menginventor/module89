#define SERIAL_TIMEOUT 1000
#define CMD_MAX_PARAM 6
#define CMD_BUF_SIZE 4
/**/
#define ENA_1 4
#define PUL_1 5
#define DIR_1 6
//
#define ENA_2 7
#define PUL_2 8
#define DIR_2 9
//
#define ENA_3 10
#define PUL_3 11
#define DIR_3 12
//
#define ENA_4 A0
#define PUL_4 A1
#define DIR_4 A2
class stepper {
  private:
    int ena_pin;
    int pul_pin;
    int dir_pin;
    int motor_stage = 0;
  public:
    float motor_crr_speed;
    unsigned motor_crr_pos;
    float motor_goal_speed;
    unsigned motor_goal_pos;
    float max_acc;
    stepper(int _ena_pin, int _pul_pin, int _dir_pin) {
      ena_pin = _ena_pin;
      pul_pin = _pul_pin;
      dir_pin = _dir_pin;
      pinMode(ena_pin, OUTPUT);
      pinMode(pul_pin, OUTPUT);
      pinMode(dir_pin, OUTPUT);
    }
/*
    void loop() {

    }
    */
};


class cmd_class {
  private:
    String cmd_str = "";
    String cmd_ins = "";
    String cmd_param[CMD_MAX_PARAM];
    bool err_flag = false;
    int param_num = 0;

  public:
    int param_available();
    String param(uint8_t n);
    String param_val(String param_name);
    void set_cmd(String _cmd_str);
    String str() {

      return cmd_str;
    }
    void clear();
    bool isValid();
};


class cmd_buf_class {
  private:
    int buf_in = 0;
    int buf_out = 0;
  public:
    cmd_class buf[CMD_BUF_SIZE];
    int available();
    void insert(cmd_class cmd) {
      buf[buf_in] = cmd;
      buf_in++;
      if (buf_in >= CMD_BUF_SIZE)buf_in -= CMD_BUF_SIZE;
    }
    cmd_class read() {
      cmd_class cmd = buf[buf_out];
      buf_out++;
      if (buf_out >= CMD_BUF_SIZE)buf_out -= CMD_BUF_SIZE;
      return cmd;
    }
};

int cmd_buf_class::available() {
  int n = buf_in - buf_out;
  while (n < 0)n += CMD_BUF_SIZE;
  return n;
}

cmd_buf_class cmd_buf;

bool busy_flag = false;
bool buf_full_flag = false;
stepper  motor_w (ENA_1, PUL_1, DIR_1);
stepper  motor_x(ENA_2, PUL_2, DIR_2);
stepper  motor_y(ENA_3, PUL_3, DIR_3);
stepper  motor_z(ENA_4, PUL_4, DIR_4);

void setup() {

  Serial.begin(115200);

}

void process_cmd_str(String cmd_str) {
  cmd_class crr_cmd;
  crr_cmd.set_cmd(cmd_str);
  if (crr_cmd.isValid()) {
    cmd_buf.insert(crr_cmd);
    if (cmd_buf.available() < CMD_BUF_SIZE - 1) {
      Serial.println("OK");
    }
    else {
      buf_full_flag = true;
    }


    /*
      for (int i = 0; i < crr_cmd.param_available(); i++) {
      Serial.print("param");
      Serial.print(i);
      Serial.print(" : ");
      Serial.println(crr_cmd.param(i));
      }
    */
    /*
      if(crr_cmd.param(0).equals("G1")){
      busy_flag = true;
      Serial.println("Process job1");
      }
    */
  } else {
    Serial.println("ERR");
  }
}


void motor_loop() {
  /*
  motor_w.loop();
  motor_x.loop();
  motor_y.loop();
  motor_z.loop();
  */
}
void loop() {
  serial_loop();
  job_loop();
  motor_loop();
}


void G0_function (cmd_class cmd){

  Serial.println("W = "+cmd.param_val("W"));
  Serial.println("X = "+cmd.param_val("X"));
  Serial.println("Y = "+cmd.param_val("Y"));
  Serial.println("Z = "+cmd.param_val("Z"));
  busy_flag = false;
}
void job_loop() {
  if (!busy_flag){
    if (cmd_buf.available() > 0) {
      //Serial.print("Read from buffer : ");
      cmd_class crr_cmd= cmd_buf.read();
      //Serial.println(crr_cmd.str());
      if (crr_cmd.param(0).equals("G0")) {
        busy_flag = true;
        G0_function(crr_cmd);
        
      }
    }
  }
}
