/* ##### IR SENSOR ##### */

// IR Sensor Class
class IRSensor {
private:
  // Variable Initilisations
  int strength;
  int direction;
  int dirHistory[DIR_HIST_LENGTH] = {12, 12, 12, 12, 12};
  int strHistory[STR_HIST_LENGTH] = {40, 40, 40, 40, 40};

  // Histories For Greater Accuracy
  void updateStrHistory() {
    for (int i = 0; i < STR_HIST_LENGTH; i++) {
      if (i != STR_HIST_LENGTH-1) {
        strHistory[i] = strHistory[i+1];
      }
      else {
        strHistory[i] = strength;
      }
    }
  };

  void updateDirHistory() {
    for (int i = 0; i < DIR_HIST_LENGTH; i++) {
      if (i != DIR_HIST_LENGTH-1) {
        dirHistory[i] = dirHistory[i+1];
      }
      else {
        dirHistory[i] = direction;
      }
    }
  };

public:
  // Retrieving IR Sensor Data
  void readData() {
    Wire.requestFrom(0x08, 2);
    if (Wire.available()) {
      direction = Wire.read();
      strength = Wire.read();
    }

    // Updating Histories
    updateDirHistory();
    updateStrHistory();
  };

  // Retreving Raw Data For Debugging
  int getRawDirection() {
    return direction;
  };

  int getRawStrength() {
    return strength;
  };

  // Finding Most Common Direction In History
  int getFilteredDirection() {
    int directionCount[13] = {0};
    
    for (int i = 0; i < DIR_HIST_LENGTH; i++) {
      directionCount[dirHistory[i]]++;
    }
    
    int count = directionCount[dirHistory[DIR_HIST_LENGTH-1]];
    int direction = dirHistory[DIR_HIST_LENGTH-1];
    
    for (int i = 0; i < 13; i++) {
      if (directionCount[i] > count) {
        count = directionCount[i];
        direction = i;
      }
    }
    return direction;
  };
  
  // Getting Average Strength From History
  int getFilteredStrength() {
    int totalStrength = 0;
    for (int i = 0; i < STR_HIST_LENGTH; i++) {
      totalStrength += strHistory[i];
    }
    return totalStrength / (STR_HIST_LENGTH + 0.0);
  };
};