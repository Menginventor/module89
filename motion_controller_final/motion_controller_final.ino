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
#define STATUS_LED_PIN 13
void port_write( uint8_t pin, uint8_t val) ;
class stepper {
  private:
    int ena_pin;
    int pul_pin;
    int dir_pin;
    int motor_stage = 0;
  public:
    float crr_speed;
    long crr_pos;
    float goal_speed;
    long goal_pos;
    float max_acc;
    unsigned long timer = micros();
    unsigned long pulse_period = 4000;
    uint8_t enable_logic = HIGH;
    stepper(int _ena_pin, int _pul_pin, int _dir_pin) {
      ena_pin = _ena_pin;
      pul_pin = _pul_pin;
      dir_pin = _dir_pin;
      pinMode(ena_pin, OUTPUT);
      pinMode(pul_pin, OUTPUT);
      pinMode(dir_pin, OUTPUT);
    }
    void enable() {
      digitalWrite(ena_pin, enable_logic);
    }
    void disable() {
      digitalWrite(ena_pin, !enable_logic);
    }
    
    void update() {
      if (goal_pos > crr_pos) {
        port_write(dir_pin, HIGH);
        if (micros() - timer > pulse_period) {
          timer = micros();
          crr_pos++;
        }
        else if (micros() - timer > pulse_period / 2) {
          port_write(pul_pin, HIGH);
        }
        else {
          port_write(pul_pin, LOW);

        }
      }
      else if (goal_pos < crr_pos) {
        port_write(dir_pin, LOW);

        if (micros() - timer > pulse_period) {
          timer = micros();
          crr_pos--;
        }
        else if (micros() - timer > pulse_period / 2) {
          port_write(pul_pin, HIGH);

        }
        else {
          port_write(pul_pin, LOW);

        }
      }
    }
    
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
  pinMode(STATUS_LED_PIN,OUTPUT);
  motor_w.pulse_period = 4000;
  motor_z.pulse_period = 1500;
  motor_x.pulse_period = 20000;
  motor_y.pulse_period = 2000;

  motor_w.enable();
  motor_x.enable();
  motor_y.enable();
  motor_z.enable();
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



    for (int i = 0; i < crr_cmd.param_available(); i++) {
      Serial.print("param");
      Serial.print(i);
      Serial.print(" : ");
      Serial.println(crr_cmd.param(i));
    }

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



void loop() {
  serial_loop();
  job_loop();
  motor_loop();
}
void motor_loop() {

  motor_w.update();
  motor_x.update();
  motor_y.update();
  motor_z.update();
 
}

void job_loop() {
  static void (*job_fc)(cmd_class);
  static cmd_class crr_cmd;
  if (!busy_flag) {
    port_write(STATUS_LED_PIN,LOW);
    if (cmd_buf.available() > 0) {
      Serial.println("Read from buffer : ");
      crr_cmd = cmd_buf.read();
      //Serial.println(crr_cmd.str()x);
      if (crr_cmd.param(0).equals("G0")) {
        busy_flag = true;
        port_write(STATUS_LED_PIN,HIGH);
        job_fc = &G0_loop;
        G0_init(crr_cmd);

      }
    }
  } else {
    
    job_fc(crr_cmd);
  }
}
