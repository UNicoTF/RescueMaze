#include "Moviment.h"

Moviment::Moviment (uint16_t speed) {
  this->speed = speed;
}

void Moviment::begin(){
  motorFR.begin();
  motorFL.begin();
  motorRR.begin();
  motorRL.begin();
  if (!orientation.check()) {
    pinMode(PC13,OUTPUT);
    gpio_write_bit(GPIOC, 13, 0);
    while(1) delay(1000);
  }
  orientation.begin();
  delay(100);
  orientation.calibrate();
  delay(100);
  orientation.start();
}
void Moviment::go() {
  go(false);
}

void Moviment::go(bool invert) {
  motorFR.start(bound(speed + kR, 65535), !invert);
  motorFL.start(bound(speed + kL, 65535), !invert);
  motorRR.start(bound(speed + kR, 65535), !invert);
  motorRL.start(bound(speed + kL, 65535), !invert);
  
}

void Moviment::rotate() {
  rotate(false);
}

void Moviment::rotate(bool invert) {
  orientation.start();
  float end = endAngle(orientation.yaw(), invert);
  if (invert) {
    while(orientation.yaw()<end) {
      rotationSpeed(invert,end);
    }
    stop();
    while(orientation.yaw()>end) {
      rotationSpeed(!invert,end);
    }
  }
  else {
    while(orientation.yaw()>end){
      rotationSpeed(invert,end);
    }
    stop();
    while(orientation.yaw()<end) {
      rotationSpeed(!invert,end);
    }
  }
  setK(0,0);
  stop();
}

void Moviment::stop() {
  motorFR.stop();
  motorFL.stop();
  motorRR.stop();
  motorRL.stop();
}

void Moviment::setSpeed(uint16_t speed) {
  this->speed = speed;
  motorFR.setSpeed(speed);
  motorFL.setSpeed(speed);
  motorRR.setSpeed(speed);
  motorRL.setSpeed(speed);
}

void Moviment::setK(int rightK, int leftK) {
  kR = rightK;
  kL = leftK;
  motorFR.setSpeed(bound((speed + kR) , 65535));
  motorFL.setSpeed(bound((speed + kL) , 65535));
  motorRR.setSpeed(bound((speed + kR) , 65535));
  motorRL.setSpeed(bound((speed + kL) , 65535));
}

float Moviment::endAngle(float angle, bool invert) {
  if (invert) {
    angle += 90;
    return (angle > 360) ? angle - 360 : angle;
  }
  else {
    angle -= 90;
    return (angle < 0) ? angle + 360 : angle;
  }
}

void Moviment::rotationSpeed(bool invert , float endRotation) {
  direzione = orientation.yaw();
  if (endRotation-direzione>0) setK(FIRST_K+((endRotation - direzione)*300), SECOND_K+((endRotation - direzione)*300));
  else setK(SECOND_K+((direzione - endRotation)*300), FIRST_K+((direzione - endRotation)*300));
  motorFR.start(bound((speed + kR) , 65535), !invert);
  motorFL.start(bound((speed + kL) , 65535), invert);
  motorRR.start(bound((speed + kR) , 65535), !invert);
  motorRL.start(bound((speed + kL) , 65535), invert);
  Serial.print(endRotation);
  Serial.print("  ");
  Serial.print(direzione);
  Serial.print(" R: ");
  Serial.print(bound((kR) , 65535));
  Serial.print(" L: ");
  Serial.println(bound((kL) , 65535));
}

uint16_t Moviment::bound(uint32_t n, uint16_t max) {
  return (n > max) ? max : n;
}
