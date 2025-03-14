/* ##### MAIN LOGIC ##### */

// Variable Initilisations
Robot bot;
int loops = 0;
int loopFlickers = 200;
double dt = 0.01;
double startTime, endTime;
bool positionSet = false;
bool otosError = false;
bool bnoError = false;
bool switchError = false;
bool overallError = false;
bool fieldWorthy = true;
String startPos = "NONE";

/* ### DEBUGGING ### */
void printIR() {
  Serial.print("\nDirection: ");
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

void printLine(bool justLineValue=true) {
  Serial.print("\nLine: ");
  Serial.println(bot.lineValue);

  if (justLineValue) return;

  if (bot.OnLeftLine) {
    Serial.println("ON LEFT LINE");
  }

  else if (bot.OnRightLine) {
    Serial.println("ON RIGHT LINE");
  }

  if (bot.OnBackLine) {
    Serial.println("ON BACK LINE");
  }

  else if (bot.OnFrontLine) {
    Serial.println("ON FRONT LINE");
  }
};

void printBluetooth() {
  Serial.print("\nOtherDir:");
  Serial.println(bot.otherDir);
  Serial.print("OtherStr:");
  Serial.println(bot.otherStr);
  Serial.print("OtherX:");
  Serial.println(bot.otherX);
  Serial.print("OtherY:");
  Serial.println(bot.otherY);
  Serial.print("OtherDef:");
  Serial.println(bot.otherDef);

}

void printPosition() {
  Serial.print("\nPosition X: ");
  Serial.println(bot.ps.x);
  Serial.print("Position Y: ");
  Serial.println(bot.ps.y);
  Serial.print("Offset: ");
  Serial.println(bot.offset);
}

/* ### SETUP ### */
void setup() {
  Serial.begin(9600);
  bluetooth.begin(38400);
  Serial6.begin(9600);

  Serial.println("\nSerials Initialized");
  // Pin Setup

  // Motor Pins
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

  // Ultrasonic Pins

  pinMode(USL_TRIG, OUTPUT);
  pinMode(USR_TRIG, OUTPUT);
  pinMode(USB_TRIG, OUTPUT);
  pinMode(USL_ECHO, INPUT);
  pinMode(USR_ECHO, INPUT);
  pinMode(USB_ECHO, INPUT);

  // Idle Pin
  pinMode(IDLE_PIN, INPUT_PULLDOWN);
  pinMode(13, OUTPUT);

  Serial.println("Pins Initialized");
  // Stopping All Motors
  bot.mc.stopMotors();
  
  // IMU
  if (bot.bno.begin()) { Serial.println("\nBNO055 Initialized"); }
  else { 
    Serial.println("\nNo BNO055 detected"); 
    bnoError = true;
    bot.useBNO = false;
  }
  bot.bno.setExtCrystalUse(true);

  // OTOS
  if (myOtos.begin()) { Serial.println("OTOS Initialized"); }
  else { 
    Serial.println("No OTOs detected");
    otosError = true;
    bot.useOTOS = false;
  }
  bot.otos.setUp();
  
  // IDLE SWITCH
  if (mswitch->begin()) { Serial.println("IDLE SWITCH Initialized");}
  else {
    Serial.println("No IDLE SWITCH detected");
    switchError = true;
    loopFlickers = 50;
    bot.useOTOS = false;
  }
  mswitch->setEventMode(true);

  // IR
  Wire.begin();

  Serial.println("\nRobot Initialized");

  // Checking for Errors
  overallError = bnoError || switchError || otosError;
  fieldWorthy = !(bnoError && switchError);
  digitalWrite(13, HIGH);
}

/* ### MAIN LOOP ### */
void loop() {
  // Checking for Errors and Flashing LED to Indicate Error
  loops++;
  if (overallError && loops % loopFlickers == 0 && fieldWorthy) {
    digitalWrite(13, HIGH);
  }
  else if (overallError) {
    digitalWrite(13, LOW);
  }

  // Checking Idle Status and Setting Initial Position
  if (startPos == "NONE" || !fieldWorthy) {
    if (!otosError && !switchError) {
      startPos = checkIdle();
    }
    else {
      startPos = checkIdle(true);
    }
    // Updating Robot's Sensors
    bot.update(true);
    return;
  }

  else if (!positionSet) {
    bot.setStartPos(startPos);
    positionSet = true;
    if (!overallError) {
      digitalWrite(13, LOW);
    }
  }

  // Updating Robot's Sensors
  bot.update();

  // Starting Loop Timer
  startTime = micros();

  // Targeting Goal if Ball Infront and Close
  if (bot.direction == 12) {
    bot.x = 0;
    bot.y = 1;
    bot.getOffset();
  }

  else if (bot.direction == 11 && bot.ps.x <=FIELD_WIDTH/2.0 && bot.strength > MIN_CLOSE_STRENGTH) {
    bot.getBehindBall();
    bot.getOffset();
  }

  else if (bot.direction == 1 && bot.ps.x >=FIELD_WIDTH/2.0 && bot.strength > MIN_CLOSE_STRENGTH) {
    bot.getBehindBall();
    bot.getOffset();
  }

  // Otherwise Get Behind Ball
  else {
    bot.offset = 0;
    bot.getBehindBall();
  }
  
  // Defending Goal (if needed)
  bot.defendGoal(dt);

  // Staying Within the Lines
  bot.stopAtLine();

  // Adjusting Speed
  bot.adjustSpeed();
  
  

  // Running the Robot
  bot.run();

  // Updating Loop Timer
  endTime = micros();
  dt = (endTime - startTime)/1000.0;
  //printPosition();
  printLine();
}


