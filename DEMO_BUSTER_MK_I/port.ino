void port_write( uint8_t pin, uint8_t val) {
  /*
    B (digital pin 8 to 13)
    C (analog input pins) 14 - 21
    D (digital pins 0 to 7)
  */


  volatile uint8_t *port = NULL;
  if (pin >= 0 && pin <= 7) {
    if (val == 0)PORTD &= ~(1 << pin);
    else PORTD |= (1 << pin);
  }
  else if (pin >= 8 && pin <= 13) {
    pin -= 8;
    if (val == 0)PORTB &= ~(1 << pin);
    else PORTB |= (1 << pin);
  }
  else if (pin >= 14 && pin <= 21) {
    pin -= 14;
    if (val == 0)PORTC &= ~(1 << pin);
    else PORTC |= (1 << pin);
  }

}
