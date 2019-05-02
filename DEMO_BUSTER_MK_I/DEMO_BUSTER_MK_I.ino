


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
//
#define SOLENOID_PIN A3
#define STATUS_LED_PIN 13
#define ENDSTOP_PIN 3
#define HOMING_LIMIT_PIN A4
#define Z_MAX 950//mm
//
/*ADC Setup*/


//
#define W_STEP_PER_DEG ((800.0*2.0)/360.0)// Drive 800 Steps per Refolution , Redution ratio (Out/In) = 2
#define Y_STEP_PER_DEG ((200.0*2*25.0)/360.0)// Drive 800 Steps per Refolution , Redution ratio (Out/In) = 2*25
#define X_STEP_PER_DEG ((1600.0*3)/360.0)// Drive 800 Steps per Refolution , Redution ratio (Out/In) = 2
#define Z_STEP_PER_MM ((400.0/10.0)/2.0)// Drive 800 Steps per Refolution , Redution ratio (Out/In) = 2*25
#define W_MAX 130
#define W_MIN -130
#define X_MAX 120
#define X_MIN -120
#define Y_MAX 130
#define Y_MIN -130
#define Z_MAX 950
#define Z_MIN -20
#define W_MAX_VEL 90//Deg/sec
#define X_MAX_VEL 60//Deg/sec
#define Y_MAX_VEL 60//Deg/sec
#define Z_MAX_VEL 40//mm/sec
//
//Homing
#define W_HOMING_VEL 10
#define X_HOMING_VEL 10
#define Y_HOMING_VEL 20
//
#define W_HOMING_POS (-120+9)
#define X_HOMING_POS (120-2)
#define Y_HOMING_POS (-130-1)
//
#define POSITIVE_DIR 1
#define NEGATIVE_DIR 0
#define W_HOMING_DIR NEGATIVE_DIR
#define X_HOMING_DIR POSITIVE_DIR
#define Y_HOMING_DIR NEGATIVE_DIR
//ADC Calib MAX = 90 GEG , MIN = -90 DEG

/*
  Z-ENDSTOP : 1
  MOTOR-X : 1250.79
  MOTOR-Y : 1207.00
  MOTOR-W : 1285.25
  GRIPPER-SENSOR : -41.15
  OK
  param0 : G0
  param1 : W-90
  param2 : X-90
  param3 : Y-90
  OK
  param0 : M119
  Z-ENDSTOP : 1
  MOTOR-X : 148.97
  MOTOR-Y : 21.36
  MOTOR-W : 211.52
  GRIPPER-SENSOR : -36.91



*/
void port_write( uint8_t pin, uint8_t val) ;
class stepper {
  public:
    int ena_pin;
    int pul_pin;
    int dir_pin;
    int motor_stage = 0;
    bool pulse_flag;



    /*con_acc*/
    bool dir_inv = false;
    float top_vel;
    float  c1 ;
    float c2 ;
    float t_acc ;
    float t_dacc ;
    float t_end;
    unsigned long acc_step;
    unsigned long  relative_goal_pos;
    unsigned long  relative_crr_pos;
    unsigned long  relative_crr_half_step;
    bool move_flag = false;

    /**/
    float crr_vel;//for g1
    long crr_pos;
    float goal_vel;
    long goal_pos;
    float max_acc;
    float max_vel;
    unsigned long timer = micros();
    unsigned long timer_next = micros();
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
      port_write(pul_pin, HIGH);
      pulse_flag = true;
    }
    void disable () {
      digitalWrite(ena_pin, !enable_logic);
    }

    void init_convel() {
      relative_goal_pos = abs(goal_pos - crr_pos);
      relative_crr_pos = 0;

      if (goal_pos > crr_pos) {
        port_write(dir_pin, 1 ^ dir_inv);
      }
      else {
        port_write(dir_pin, 0 ^ dir_inv);
      }


      port_write(pul_pin, HIGH);

      timer = micros();
      pulse_flag = true;
    }
    void update_convel() {
      if (relative_goal_pos != relative_crr_pos) {
        unsigned long pulse_period = 500000.0f / crr_vel;
        if ( micros() - timer >= pulse_period) {
          timer = micros();
          pulse_flag = !pulse_flag;
          port_write(pul_pin, pulse_flag);
          if (!pulse_flag) {
            relative_crr_pos++;
          }
        }

      }
      else {
        crr_pos = goal_pos;
      }
    }
    void init_conacc() {
      relative_goal_pos = abs(goal_pos - crr_pos);
      relative_crr_pos = 0;
      top_vel = max_vel;
      c1 = 0.5 * pow(top_vel, 2) / max_acc;
      if (relative_goal_pos < 2 * c1) {
        top_vel = sqrt(relative_goal_pos * max_acc);
      }
      c1 = 0.5 * pow(top_vel, 2) / max_acc;
      c2 = relative_goal_pos - 2 * c1;
      t_acc = top_vel / max_acc;
      t_dacc = c2 / top_vel + t_acc;
      t_end = c2 / top_vel + 2 * t_acc;
      acc_step = pow(top_vel, 2) / (2.0 * max_acc); //s =( v^2 - u^2) / 2a
      if (goal_pos > crr_pos) {
        port_write(dir_pin, 1 ^ dir_inv);
      }
      else {
        port_write(dir_pin, 0 ^ dir_inv);
      }
      move_flag = true;

      port_write(pul_pin, HIGH);
      pulse_flag = true;
      relative_crr_half_step = 0;
      timer = micros();
    }
    void update_conacc() {
      if (micros() >= timer) {

        if (relative_crr_half_step >= relative_goal_pos * 2) {
          crr_pos = goal_pos ;
          if (move_flag) {

            move_flag = false;
          }
        }
        else {
          if (relative_crr_half_step <= acc_step * 2) {
            unsigned long next_pulse = (sqrt(float((relative_crr_half_step + 1)) / max_acc) - sqrt(float((relative_crr_half_step)) / max_acc) ) * 1000000.0 ;
            timer = next_pulse + micros();

          }
          else if (relative_crr_half_step <= (relative_goal_pos - acc_step) * 2) {
            unsigned long next_pulse = 1000000.0 / (top_vel * 2.0);
            timer = micros() + next_pulse;

          }
          else {
            //t=(sqrt(u^2+2*a*s)-u)/a
            float dacc_crr_pos = relative_crr_half_step - (relative_goal_pos - acc_step) * 2 ;
            float  t2  = (sqrt(pow(top_vel, 2) + 2.0 * (-max_acc) * float(dacc_crr_pos + 1.0) / 2.0) - top_vel) / (-max_acc);
            float t1  =  (sqrt(pow(top_vel, 2) + 2.0 * (-max_acc) * float(dacc_crr_pos) / 2.0) - top_vel) / (-max_acc);
            unsigned long next_pulse = (t2 - t1) * 1000000.0;
            timer = micros() + next_pulse;
          }
          relative_crr_half_step ++;
          if (relative_crr_half_step % 2 == 0) {
            port_write(pul_pin, LOW);
            pulse_flag = false;

          }
          else {
            port_write(pul_pin, HIGH);
            pulse_flag = true;
          }
        }
      }
    }
};

class param_class {
  public:
    char ins;
    int int_val;
    float float_val;
    boolean ins_available = false;
    boolean val_available = false;
    boolean equal_param(char _ins, int _val) {
      return _ins == ins && int_val == _val;
    }
    boolean equal_ins(char _ins) {
      return _ins == ins;
    }

};
class cmd_class {

  private:
    String cmd_str;

    param_class cmd_param[CMD_MAX_PARAM];
    bool err_flag = false;
    int param_num = 0;

  public:
  cmd_class(){
     cmd_str.reserve(24);
  }
    int param_available();
    param_class param(uint8_t n);
    void set_cmd(String _cmd_str);
    String str() {

      return cmd_str;
    }
    void clear();
    bool isValid();
    bool ins_available(char _ins) {
      for (int i = 0; i < CMD_MAX_PARAM; i++) {
        if (cmd_param[i].ins == _ins)return true;
      }
      return false;
    }
    param_class find_param(char _ins) ;

    void print() {
      Serial.println(F("Print cmd"));
      Serial.print(param_num);
      Serial.println(F(" params"));
      for (int i = 0; i < param_num; i++) {
        Serial.print(cmd_param[i].ins);
        if (cmd_param[i].val_available) {
          Serial.print(F(" "));
          if(i == 1)Serial.println(cmd_param[i].int_val);
          else Serial.println(cmd_param[i].float_val);
        }
        else {
          Serial.println();
        }
      }
    }
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
  Serial.println(F("System Reset!"));
  Serial.println(F("RST"));
  pinMode(STATUS_LED_PIN, OUTPUT);
  motor_w.pulse_period = 4000;
  motor_z.pulse_period = 1000;

  motor_x.pulse_period = 2000;
  motor_x.dir_inv = true;
  motor_w.dir_inv = true;
  motor_y.pulse_period = 1500;

  motor_w.max_vel =   W_MAX_VEL * W_STEP_PER_DEG;
  motor_z.max_vel =   Z_MAX_VEL * Z_STEP_PER_MM;
  motor_x.max_vel =   X_MAX_VEL * X_STEP_PER_DEG;
  motor_y.max_vel =   Y_MAX_VEL * Y_STEP_PER_DEG;
  //
  motor_w.max_acc = float(  motor_w.max_vel) / 0.5;

  motor_x.max_acc = float(  motor_x.max_vel) / 1.0;
  motor_y.max_acc = float(  motor_y.max_vel) / 0.5;
  motor_z.max_acc = float(  motor_z.max_vel) / 2.0;
  motor_w.enable();
  motor_x.enable();
  motor_y.enable();
  motor_z.enable();
  pinMode(ENDSTOP_PIN, INPUT_PULLUP);
  pinMode(HOMING_LIMIT_PIN, INPUT_PULLUP);
  pinMode(SOLENOID_PIN, OUTPUT);

}


void put_cmd_buf(String cmd_str) {
  cmd_class crr_cmd;
  crr_cmd.set_cmd(cmd_str);
  if (crr_cmd.isValid()) {
    cmd_buf.insert(crr_cmd);
    if (cmd_buf.available() < CMD_BUF_SIZE - 1) {
      Serial.println(F("OK"));
    }
    else {
      buf_full_flag = true;
    }

  } else {
    Serial.println(F("ERR"));
    
  }
}

unsigned long Hearth_beat_timer = millis();

void loop() {
  serial_loop();
  job_loop();
  if(millis()-Hearth_beat_timer>=2000){
    Hearth_beat_timer = millis();
    Serial.println(F("Hearth beat!"));
  }
}


void job_loop() {



  port_write(STATUS_LED_PIN, LOW);
  if (cmd_buf.available() > 0) {
    //Serial.println(F("Read from buffer"));
    cmd_class crr_cmd = cmd_buf.read();

    if (crr_cmd.param(0).equal_param('G', 0))  {
      busy_flag = true;
      port_write(STATUS_LED_PIN, HIGH);

      G0(crr_cmd);
    }
    else if (crr_cmd.param(0).equal_param('G', 1))  {
      busy_flag = true;
      port_write(STATUS_LED_PIN, HIGH);

      G1(crr_cmd);
    }
    else if (crr_cmd.param(0).equal_param('M', 119))  {
      M119();
    }
    else if (crr_cmd.param(0).equal_param('G', 28))  {
      port_write(STATUS_LED_PIN, HIGH);
      G28_init(crr_cmd);


    }
    else if (crr_cmd.param(0).equal_param('M', 3))  {
      M3();
    }
    else if (crr_cmd.param(0).equal_param('M', 4))  {
      M4();
    }
    else if (crr_cmd.param(0).equal_param('M', 5))  { //Turn on solenoid
      M5();
    }
    else if (crr_cmd.param(0).equal_param('M', 114)) { //Turn on solenoid
      M114();
    }
    else{
      Serial.println(F("ERR,Unknow CMD"));
      crr_cmd.print();
    }
    Serial.println(F("READY"));

  }
  

}
