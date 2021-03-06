#include "RGB.h"

RGB::RGB(uint8_t pinr, uint8_t ping, uint8_t pinb) {
  pin[0] = pinr;
  pin[1] = ping;
  pin[2] = pinb;
}
void RGB::begin() {
  for (int i = 0; i < 3; i++) pinMode(pin[i], OUTPUT);
}
void RGB::set(uint8_t red, uint8_t green, uint8_t blue) {
  pwmWrite(pin[0], red);
  pwmWrite(pin[1], green);
  pwmWrite(pin[2], blue);
}

void RGB::refresh() {
  set(actual[0], actual[1], actual[2]);
}
