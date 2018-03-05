#include "Robot.h";

void Robot::setup() {
  setAddresses();
}

void Robot::begin() {
  for (int i = 0 ; i < 3 ; i++) laser[i].begin();
  color.begin();
  mov.begin();
}

bool Robot::check() {
  bool ok = true;
  for (int i = 0 ; i < 3 ; i++) ok&=laser[i].check();
  return ok && color.check() && mov.check();
}

void Robot::update() {
  for (int i = 0; i < 3; i++) data.dist[i] = laser[i].read();
  data.color = color.read();
  data.tempL = tempL.read();
  data.tempR = tempR.read();
}

void Robot::go() {
  laser[0].start();
  mov.go();
  uint16_t end = endDist(laser[0].read());
  while ( (laser[0].read() > end) && (color.read() == 2) );
  mov.stop();
  laser[0].stop();
}

void Robot::back() {
  laser[0].start();
  mov.go(true);
  uint16_t end = endDist(laser[0].read()) + 300;
  while (laser[0].read() < end);
  mov.stop();
  laser[0].stop();
}

void Robot::rotate(bool dir) {
  mov.rotate(dir);
}

void Robot::victim() {
  //cagamattoni.caga();
  led.set(0, 0, 0);
}

void Robot::setAddresses() {
  pinMode(LX_LEFT, OUTPUT);
  pinMode(LX_FRONT, OUTPUT);
  laser[1].setAddress(L_RIGHT);
  pinMode(LX_FRONT, INPUT);
  delay(10);
  laser[0].setAddress(L_FRONT);
  pinMode(LX_LEFT, INPUT);
  delay(10);
  laser[2].setAddress(L_LEFT);
}

uint16_t Robot::endDist(uint16_t distance) {
  return distance - (distance % 300) + CENTRED;
}