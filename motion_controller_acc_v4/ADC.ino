
void ADC_init() {
  Wire.begin();


  while ( ADS.begin()  ) {
    Serial.print("Error in initializing ADS1015, error#: ");

    Serial.println("Cannot continue");
    delay(500);
  }
}
int read_ADC(byte MUX) {
  ADS.startConversion(MUX);
  while (!(ADS.available()));
  return (ADS.getRaw());
}
/*

OK
param0 : M119
Z-ENDSTOP : 0
MOTOR-X : 662.28
MOTOR-Y : 672.97
MOTOR-W : 688.43
GRIPPER-SENSOR : -37.99
OK
param0 : G0
param1 : X90
param2 : Y90
param3 : W90
OK
param0 : M119
Z-ENDSTOP : 0
MOTOR-X : 143.14
MOTOR-Y : 1286.89
MOTOR-W : 199.53
GRIPPER-SENSOR : -37.08
OK
param0 : G0
param1 : X-90
param2 : Y-90
param3 : W-90
OK
param0 : M119
Z-ENDSTOP : 0
MOTOR-X : 1199.57
MOTOR-Y : 77.30
MOTOR-W : 1236.54
GRIPPER-SENSOR : -36.83


*/
