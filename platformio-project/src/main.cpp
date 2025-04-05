#include <iostream>
#include <Arduino.h>

using namespace std;

#include "vector.hpp"
#include "motor_controller.hpp"
#include "position_system.hpp"

// declarations here
void blinkLED();

PositionSystem pos_sys;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pos_sys.setup();
}

void loop() {
  // for testing bno055
  pos_sys.update();
  Serial.println(pos_sys.get_tilt());
}

// function definitions here
void blinkLED() {
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
}