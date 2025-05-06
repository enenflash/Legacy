#include <iostream>
using namespace std;

#define DF_PWM 33
#define DF_DIR1 13
const int DRIBBLER_SPEED = 100;

class DribblerMotor {
public:

	void run() {
    int pwmSpeed = DRIBBLER_SPEED / 100 * 255;
    digitalWrite(DF_DIR1, LOW);
    analogWrite(DF_PWM, pwmSpeed);
	};

	void stop() {
    digitalWrite(DF_DIR1, LOW);
    analogWrite(DF_PWM, 0);
	};
};

DribblerMotor DF = DribblerMotor();

void setup() {
  Serial.begin(9600);
  pinMode(DF_PWM, OUTPUT);
  pinMode(DF_DIR1, OUTPUT);
}

void loop() {
  DF.run();
  // delay(2000);
  // DF.stop();

  
  // delay(2000);

}
