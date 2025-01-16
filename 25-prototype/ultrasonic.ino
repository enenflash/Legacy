/* ##### ULTRASONICS ##### */

// Ultrasonic Class
class Ultrasonic {
public:
  // Variable Initilisations
  int trigger_pin;
  int echo_pin;

  Ultrasonic(int TRIGGER_PIN, int ECHO_PIN) {
    trigger_pin = TRIGGER_PIN;
    echo_pin = ECHO_PIN;
  }

  // Getting Distances from Ultrasonics
  int getDistance() {
    long duration, ult_distance;

    // Clearing Trigger Pin
    digitalWrite(trigger_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger_pin, LOW);

    // Getting Duration in Miliseconds
    duration = pulseIn(echo_pin, HIGH);//, 20000);
    
    // Converting to Centimeters
    ult_distance = (duration / 29) / 2;  // Calculating the ult_distance

    return ult_distance;
  }
};

// Postion System Class for Field Grid System
class PositionSystem {
private:
  // Variable Initilisations
  Ultrasonic ultLeft = Ultrasonic(USL_TRIG, USL_ECHO);
  Ultrasonic ultRight = Ultrasonic(USR_TRIG, USR_ECHO);
  Ultrasonic ultBack = Ultrasonic(USB_TRIG, USB_ECHO);

  // LEFT, RIGHT, BACK
  int rawDistances[3] = {0};
  float trueDistances[3] = {0};
  float previousDistances[3] = {0};

  float deltaDistances[3] = {0};

  // Getting and Refining Distances
  void getRawDistances() {
    rawDistances[0] = ultLeft.getDistance() + ULTRASONIC_TO_ROBOT;
    rawDistances[1] = ultRight.getDistance() + ULTRASONIC_TO_ROBOT;
    rawDistances[2] = ultBack.getDistance() + ULTRASONIC_TO_ROBOT;
  };

  void getTrueDistances(float tilt) { // tilt in radians
    for (int i = 0; i < 3; i++) {
      trueDistances[i] = cos(tilt)*rawDistances[i];
    }
  };

  // Comparing Ultrasonics
  bool totalWidthCompare() {
    return abs((trueDistances[0]+trueDistances[1])-FIELD_WIDTH) < 20;
  };

  void getDeltaDistances() {
    for (int i = 0; i < 3; i++) {
      deltaDistances[i] = abs(trueDistances[i]-previousDistances[i]);
    }
  };

  // Logging Distances
  void logPreviousDistances() {
    for (int i = 0; i < 3; i++) {
      previousDistances[i] = trueDistances[i];
    }
  };

public:
  // Variable Initilisations
  bool reliable = true;
  int usingIndex = 0; 
  float x, y;

  // Deciding Which Ultrasonic to Trust (Left/Right)
  void chooseULT() {
    getDeltaDistances();

    if (deltaDistances[0] > 20 && deltaDistances[1] > 20) {
      usingIndex = 0;
      reliable = false;
    }
    else if (deltaDistances[0] > 20) {
      usingIndex = 1;
      reliable = true;
    }
    else if (deltaDistances[1] > 20) {
      usingIndex = 0;
      reliable = true;
    }
    else {
      usingIndex = 0;
      reliable = true;
    }
  };

  // Update Ultrasonics and Position System
  void update(float tilt) {
    getRawDistances();
    getTrueDistances(tilt);

    if (!totalWidthCompare()) {
      Serial.println("Choose ULT");
      chooseULT();
    }
    
    if (usingIndex == 0) {
      x = trueDistances[0];
    }
    else if (usingIndex == 1) {
      x = FIELD_WIDTH - trueDistances[1];
    }

    y = FIELD_LENGTH - trueDistances[2];

    if (totalWidthCompare()) {
      logPreviousDistances();
    }

  };

  // Debugging
  void serialPrint() {
    Serial.print("TrueLeft: ");
    Serial.print(trueDistances[0]);
    Serial.print(" TrueRight: ");
    Serial.println(trueDistances[1]);

    Serial.print("RawLeft: ");
    Serial.print(rawDistances[0]);
    Serial.print(" RawRight: ");
    Serial.println(rawDistances[1]);

    Serial.print("Reliable: ");
    Serial.print(reliable);
    Serial.print(" usingIndex: ");
    Serial.println(usingIndex);
  }
};