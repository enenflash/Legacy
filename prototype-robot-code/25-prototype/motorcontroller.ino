/* ##### MOTOR CONTROLLER (To Be Updated to New Version) ##### */

// Motor Class to Control Individual Motors on the Hardware Level
class Motor {
public:
  // Variable Initilisations
	int PWM_PIN;
	int DIR_PIN;
	Motor(int pwm_pin, int dir_pin) {
		PWM_PIN = pwm_pin;
		DIR_PIN = dir_pin;
	};

  // Running The Motor
	void run(float speedPercent) {
    int pwmSpeed = (100 - abs(speedPercent)) / 100 * 255;
    
    // Deriving Pwm Speed and Direction
    if (speedPercent > 0) {
      digitalWrite(DIR_PIN, LOW);
    }
    else {
      digitalWrite(DIR_PIN, HIGH);
    }

    analogWrite(PWM_PIN, pwmSpeed);
	};

  // Stopping the Motor
	void stop() {
    analogWrite(PWM_PIN, 255);
	};
};

// Motor Controller Class to Interact with all Four Motors
class MotorController {
private:
  // Variable Initilisations
  bool headless;
  float speed = 100.0;
  array<float, 4> motorSpeeds;

  // Motor Object Initilisations
  Motor TL = Motor(TL_PWM, TL_DIR);
  Motor TR = Motor(TR_PWM, TR_DIR);
  Motor BL = Motor(BL_PWM, BL_DIR);
  Motor BR = Motor(BR_PWM, BR_DIR);

public:
  MotorController(bool headlessMode) {
    headless = headlessMode;
  };

private:
  array<float, 4> getMotorSpeeds(float x, float y, float speed, float tilt, float offset) {
    // Adjusting for the Robot's Tilt

    // Convert Tilt to Bearings
    tilt = 360 - tilt;
    if (tilt > 180) {
      tilt = -(360 - tilt);
    }

    float motorRatio[4] = { -x - y, -x + y, x - y, x + y };
    // Rounding Motor Ratio to Prevent Errors
    for (int i = 0; i < 4; i++) {
      motorRatio[i] = round(motorRatio[i] * 1000) / 1000;
    }

    float highestVal = *max_element(motorRatio, motorRatio + 4);
    float lowestVal = *min_element(motorRatio, motorRatio + 4);

    float largest;
    if (highestVal > lowestVal * -1) {
      largest = highestVal;
    }
    else {
      largest = lowestVal * -1;
    }

    array<float, 4> motorSpeeds;

    // Avoiding Division By 0
    if (largest == 0) {
      motorSpeeds = { tilt + offset, tilt + offset, tilt + offset, tilt + offset };
      return motorSpeeds;
    }

    for (int i = 0; i < 4; i++) {
      // Note: +0.0 Converts motorRatio to a Double
      motorSpeeds[i] = ((motorRatio[i] + 0.0) / largest * speed) + tilt + offset;
    }

    // TL, TR, BL, BR Motors Respectively
    return motorSpeeds;
  };

public:
  // Outputting Speeds
  array<float, 4> getMotorSpeeds() {
    return motorSpeeds;
  };
  float getSpeed() {
    return speed;
  };

  // Setting Speed (in Percentage)
  void setSpeed(float newSpeed) {
    speed = newSpeed;
  };

  // Running Motors Using X/Y Coordinates (and Tilt for Gyro Correction)
  void runMotors(float initial_x = 0, float initial_y = 0, float tilt = 0, float offset = 0) {
    float x = 0, y = 0;
    if (headless) {
      x = initial_x*cos(-tilt) - initial_y*sin(-tilt);
      y = initial_x*sin(-tilt) + initial_y*cos(-tilt);
    }
    else {
      x = initial_x, y = initial_y;
    }
      
    motorSpeeds = getMotorSpeeds(x, y, speed, tilt, offset);

    for (int i = 0; i < 4; i++) {
      if (motorSpeeds[i] > 100) {
        motorSpeeds[i] = 100;
      }
      else if (motorSpeeds[i] < -100) {
        motorSpeeds[i] = -100;
      }
    }

    // Running All Motors
    TL.run(motorSpeeds[0]);
    TR.run(motorSpeeds[1]);
    BL.run(motorSpeeds[2]);
    BR.run(motorSpeeds[3]);
  };

  // Running Using Raw Percentage Speeds
  void runRaw(float tlSpeed, float trSpeed, float blSpeed, float brSpeed) {
    TL.run(tlSpeed);
    TR.run(trSpeed);
    BL.run(blSpeed);
    BR.run(brSpeed);
  };

  // Stopping All Motors
  void stopMotors() {
    motorSpeeds = { 0, 0, 0, 0 };
    TL.stop();
    TR.stop();
    BL.stop();
    BR.stop();
  };
};