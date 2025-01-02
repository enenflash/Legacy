Robot bot;
bool idle = true;
int commaIndex1, commaIndex2, commaIndex3;
int otherDir, otherStr, otherX, otherY;

/* :::::::: SETUP :::::::: */
void setup() {
  Serial.begin(9600);
  bluetooth.begin(38400);
  Serial.println("\nRobot Initialized");
  
  Serial6.begin(9600);

  pinMode(IDLE_PIN, INPUT);

  /* :::::::: MOTOR PINS :::::::: */
  pinMode(TL_PWM, OUTPUT);
  pinMode(TR_PWM, OUTPUT);
  pinMode(BL_PWM, OUTPUT);
  pinMode(BR_PWM, OUTPUT);

  analogWriteFrequency(TL_PWM, 20000);
  analogWriteFrequency(TR_PWM, 20000);
  analogWriteFrequency(BL_PWM, 20000);
  analogWriteFrequency(BR_PWM, 20000);

  pinMode(TL_DIR, OUTPUT);
  pinMode(TR_DIR, OUTPUT);
  pinMode(BL_DIR, OUTPUT);
  pinMode(BR_DIR, OUTPUT);

  /* :::::::: ULTR PINS :::::::: */

  pinMode(USL_TRIG, OUTPUT);
  pinMode(USR_TRIG, OUTPUT);
  pinMode(USB_TRIG, OUTPUT);
  pinMode(USL_ECHO, INPUT);
  pinMode(USR_ECHO, INPUT);
  pinMode(USB_ECHO, INPUT);

  pinMode(IDLE_PIN, INPUT_PULLDOWN);

  bot.mc.stopMotors();
  
  // IMU
  if (!bot.bno.begin()) { Serial.print("\nNo BNO055 detected"); }
  else { Serial.println("\nBNO055 Initialized"); }
  bot.bno.setExtCrystalUse(true);

  // OTOS
  if (myOtos.begin() == false) { Serial.print("\nNo OTOs detected"); }
  else { Serial.println("\nOTOS Initialized"); }
  bot.otos.SetUp();
  bot.otos.SetPos(91.0, -121.5, 0); // start at the center of the field

  // IR
  Wire.begin();
}

/* :::::::: DEBUGGING :::::::: */

void printIR() {
  Serial.print("Direction: ");
  Serial.print(bot.direction);
  Serial.print(" Strength: ");
  Serial.println(bot.strength);
};

void printMovement() {
  Serial.print("MoveX: ");
  Serial.print(bot.x);
  Serial.print(" MoveY: ");
  Serial.print(bot.y);
  Serial.print(" Speed: ");
  Serial.println(bot.mc.getSpeed());
};

void printLine() {
  Serial.print("Line: ");
  Serial.println(bot.lineValue);
};

void printBluetooth(int otherDir, int otherStr, int otherX, int otherY) {
  Serial.print("OtherDir:");
  Serial.println(otherDir);
  Serial.print("OtherStr:");
  Serial.println(otherStr);
  Serial.print("OtherX:");
  Serial.println(otherX);
  Serial.print("OtherY:");
  Serial.println(otherY);

}

void printPosition() {
  Serial.print("Position X: ");
  Serial.println(bot.ps.x);
  Serial.print("Position Y: ");
  Serial.println(bot.ps.y);
}

/* :::::::: MAIN :::::::: */
void loop() {
  // check idle switch
  if (idle) {
    if (digitalRead(IDLE_PIN) == HIGH) {
      idle = false;
    }
    else {  
      return;
    }
  }

  bot.update();

  // seperate bluetooth data
  if (bot.otherData != "NONE") {// && commaIndex1 != -1 && commaIndex2 != -1 && commaIndex3 != -1) {
    commaIndex1 = bot.otherData.indexOf(",");
    commaIndex2 = bot.otherData.indexOf(",", commaIndex1 + 1);
    commaIndex3 = bot.otherData.indexOf(",", commaIndex2 + 1);

    otherDir = bot.otherData.substring(0, commaIndex1).toInt();
    otherStr = bot.otherData.substring(commaIndex1 + 1, commaIndex2).toInt();
    otherX = bot.otherData.substring(commaIndex2 + 1, commaIndex3).toInt();
    otherY = bot.otherData.substring(commaIndex3 + 1).toInt();
  }

  else {
    otherDir = 0;
    otherStr = 0;
    otherX = 0;
    otherY = 0;
  }

  // if (!(bot.direction < 9 && bot.direction > 3) && (otherDir > bot.direction)) {
  //   bot.x = 0;
  //   bot.y = 0;
  //   bot.offset = 0;
  // }
  
  if (true) {
    // target goal if ball infront and closer than other robot target goal
    if (bot.direction == 12) {// && !((otherDir == 12 || otherDir == 11 || otherDir == 1) && otherStr > bot.strength - 10)) {
      bot.targetGoal();
      bot.getOffset();
    }

    // if ball closer to robot or behind robot get behind ball
    else { //if (otherStr - 10 < bot.strength || (bot.direction >= 4 && bot.direction <= 8)){ 
      bot.getBehindBall();
    }

    // // otherwise defend goal
    // else {
    //   bot.mc.stopMotors(); //change to go to goal defence mode
    // }
  }

  //bot.offset = 0;

  

  bot.stopAtLine();

  //Serial.print("Direction: ");
  //Serial.print(bot.direction);
  //Serial.print(" Strength: ");
  //Serial.println(bot.strength);

  // Serial.print("X: ");
  // Serial.print(bot.ps.x);
  // Serial.print(" Y: ");
  // Serial.println(bot.ps.y);

  //bot.ps.serialPrint();
  
  //printLine();
  //printBluetooth(otherDir, otherStr, otherX, otherY);

  bot.adjustSpeed();
  bot.logY();
  // if (bot.direction > 3 && bot.direction < 9) {
  //   bot.getBehindBall();
  // }
  // else{
  // bot.moveTo(90, 120, 3); // this line currently overrides everything else
  // }
  bot.defendGoal();
  printPosition();
  bot.run();
  delay(0);
}