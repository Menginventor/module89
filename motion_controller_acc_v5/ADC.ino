
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
float precision_read_ADC(byte MUX) {
  float val = 0;
  for (int i = 0; i < 100; i++) {
    val += read_ADC(MUX);
  }
  val = val / 100;
  return val;
}
float read_POT_X(){
  
}
