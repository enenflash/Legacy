/* ##### ROBOT ##### */

// Main Robot Class
class Robot {
public:
  // Object Initilisations
  MotorController mc = MotorController(false);
  Adafruit_BNO055 bno = Adafruit_BNO055(55);
  IRSensor ir;
  Bluetooth bt;
  Otos otos;
  Pid linearPid;
  PositionSystem ps;

  // Variable Initilisations
  float tilt = 0;
  float offset = 0;
  float x = 0, y = 0;
  float previousY;
  float x_from_center, y_from_goal;
  float pastY = 999;
  float totalY = 0;
  float pastX = 999;
  float totalX = 0;
  array<float, 3> xyh;

  int direction = 0, strength = 0;
  int lineValue;
  int commaIndex1, commaIndex2, commaIndex3, commaIndex4;
  int inPosition = false;
  int defending = false;
  int otherDir, otherStr, otherX, otherY;

  bool otherDef;
  bool OnLeftLine, OnRightLine, OnFrontLine, OnBackLine;
  bool useOTOS = true;
  bool useBNO = true;

  String otherData;
  String startingPosition = "NONE";

private:
  // Main Robot Functions
  void getSensorData(bool notOTOS=false) {
    // Getting IR Data
    ir.readData();
    direction = ir.getFilteredDirection();
    strength = ir.getFilteredStrength();

    // Getting IMU Data
    if (useBNO) {
      sensors_event_t event;
      bno.getEvent(&event);
      tilt = event.orientation.x;
    }
    
    // Getting Data from OTOs and Accounting for Drift
    if (useOTOS && !notOTOS) {
      xyh = otos.getXYH();
      ps.x = xyh[0] - (1.0 / 250.0) * totalX;     
      ps.y = xyh[1] - (1.0 / 55.0) * totalY;    

      // Using OTOs Heading if BNO Cannot Connect
      if (!useBNO) {
        tilt = xyh[2];
      }
      
      if (startingPosition != "NONE") { 
        if (pastY != 999) totalY += abs(ps.y - pastY);
        if (pastX != 999) totalX += abs(ps.x - pastX);
        pastY = ps.y;
        pastX = ps.x; 
      }
    }
    

    // Converting Tilt to Unit Circle
    tilt = 360 - tilt;

    // Getting Data from Line Sensor
    if (Serial6.available()) {
      lineValue = Serial6.read() - 1;
    }
  };

  // Getting Vector Directly to Ball
  void getBallXY(float& ballX, float& ballY) {
    ballX = cos(irAngles[direction]);
    ballY = sin(irAngles[direction]);
  };

  // Functions to Simplify if Statements
  bool inRange(float variable, int target, int range) {
    return (variable < target + range && variable > target - range);
  };

  bool ballBehind(int direction) {
    return direction > 3 && direction < 9;
  };

  bool strengthDifferance(bool self = true) {  // Note: Checks That Robot Itself is Closer by Default
    // Checking if Teammate is Closer
    if (!self) {
      return strength < otherStr && abs(strength - otherStr) >= MINIMUM_STRENGTH_DIFFERANCE;
    }

    // Checking if Robot is Closer
    return strength > otherStr && abs(strength - otherStr) >= MINIMUM_STRENGTH_DIFFERANCE;
  };

  float getHypotenuse(float a, float b) {
    return sqrt(pow(a, 2) + pow(b, 2));
  }

  float getHypotenuse(int a, int b) {
    return sqrt(pow(a, 2) + pow(b, 2));
  }

  float strengthToDistance() {
    return -(31.0/12.0) * strength + 117.25;
  };

  // Checking Defence Conditions
  bool checkDefenceConditions() {

    // Conditions Not To Defend
    if (otherDir == 0 || otherDef || ballBehind(direction) || (direction == 0 && otherDir == 0) || ballBehind(otherDir)) {
      return false;
    }

    // Defend if Teammate has Control of Ball
    if ((otherDir >= 11 || otherDir == 1) && (otherStr > MIN_CLOSE_STRENGTH) && strengthDifferance(false)) {
      return true;
    }

    // When Robot Can't See the Ball but Teammate Can
    if (!ballBehind(otherDir) && direction == 0) {
      return true;
    }
    else if (ballBehind(otherDir) && direction == 0) {
      return false;
    }

    // Stop Robots Colliding
    if (getHypotenuse(abs(x - otherX), abs(y - otherY)) < MINIMUM_TEAMMATE_DISTANCE){
      if (y < otherY) {
        return true;
      }
    }


    // Conditions When Defending Often
    if (DEFEND_OFTEN) {
      // Defend if Ball Closer to and Infront of Teammate
      if (strengthDifferance(false) && !ballBehind(otherDir)) {
        return true;
      }

      // Ball Behind Temmate and Infront of Robot and Robot Closer to Goal than Ball
      // if (strengthDifferance(false) && ballBehind(otherDir) && (DEFEND_POS_Y - ps.x < strengthToDistance())) {
      //   return true;
      // }
    }

    // Not Defending by Default
    return false;
  };

public:
  // Updating Robot Information
  void update(bool idle=false) {
    // Get All Sensor Data
    getSensorData(idle);

    if (!USING_OTOS) ps.update(tilt * PI / 180);

    // Updating Bluetooth
    bt.sendData(direction, strength, ps.x, ps.y, inPosition);
    otherData = bt.getData();

    if (otherData != "NONE") {
      commaIndex1 = otherData.indexOf(",");
      commaIndex2 = otherData.indexOf(",", commaIndex1 + 1);
      commaIndex3 = otherData.indexOf(",", commaIndex2 + 1);
      commaIndex4 = otherData.indexOf(",", commaIndex3 + 1);

      otherDir = otherData.substring(0, commaIndex1).toInt();
      otherStr = otherData.substring(commaIndex1 + 1, commaIndex2).toInt();
      otherX = otherData.substring(commaIndex2 + 1, commaIndex3).toInt();
      otherY = otherData.substring(commaIndex3 + 1, commaIndex4).toInt();
      otherDef = otherData.substring(commaIndex4 + 1).toInt();
    }

    else {
      otherDir = 0;
      otherStr = 0;
      otherX = 0;
      otherY = 0;
      otherDef = false;
    }
  };

  // Getting Vector to Behind the Ball
  void getBehindBall() {
    if (direction == 0) {
      x = 0, y = 0;
      return;
    }

    if (strength < 30) {
      getBallXY(x, y);
      return;
    }

    if (direction == 12) {
      x = 0, y = 1;
      return;
    }

    if (direction == 6 && lineValue != 3) {
      x = 1, y = 0;
      return;
    }

    else if (direction == 6) {
      x = -1, y = 0;
      return;
    }

    float movementAngle;

    float ballX = 0, ballY = 0;
    getBallXY(ballX, ballY);

    if (ballX < 0) {
      movementAngle = irAngles[direction] + PI / 2;
    } 
    else {
      movementAngle = irAngles[direction] - PI / 2;
    }

    x = cos(movementAngle);
    y = sin(movementAngle);
  };

  // Moving to Specific Coordiants Using PID
  bool moveTo(float TARGET_X, float TARGET_Y, float maxSpeed, double dt) {
    float dx = TARGET_X - ps.x;
    float dy = TARGET_Y - ps.y;
    float distance = sqrt(pow(dx, 2) + pow(dy, 2));

    if (distance > 0.3) {
      x = dx / distance;
      y = -(dy / distance);
    }

    else {
      x = 0;
      y = 0;
      linearPid.integral = 0;
      return true;
    }

    float speed = linearPid.compute(distance, dt);
    speed = max(0, min(speed, maxSpeed));
    if (speed > 0 && speed < MINIMUM_PID_SPEED) {
      speed = MINIMUM_PID_SPEED;
    }

    mc.setSpeed(speed);
    return false;
  };

  // Defending the Goal
  void defendGoal(double dt) {
    defending = checkDefenceConditions();
    if (!defending) {
      inPosition = false;
      return;
    }
    
    offset = 0;

    if (!inPosition) {
      if ((otherY < ps.y) && (otherX > ps.x - ROBOT_DIAMETER && otherX < ps.x + ROBOT_DIAMETER)) {
        
        if (ps.x <= FIELD_WIDTH/2.0) {
          moveTo(30, ps.y, MAX_SPEED, dt);
        }
        else {
          moveTo(FIELD_WIDTH-30, ps.y, MAX_SPEED, dt);
        } 
      }

      else {
        inPosition = moveTo(DEFEND_POS_X, DEFEND_POS_Y, MAX_SPEED, dt);
      }
      
    }

    else {
      // getBehindBall();
      // if ((ps.x > DEFEND_POS_X + 26 && x > 0) || (ps.x < DEFEND_POS_X - 26 && x < 0)) {
      //   x = 0;
      // }
      x = 0;
      y = 0;
    }
  }

  // Adjusting Speed According To Situation
  void adjustSpeed() {
    
    // don't set speed if defending
    if (defending) {
      return;
    }
    // Speed On The Line
    if (lineValue != 0 || OnLeftLine || OnRightLine || OnBackLine || OnFrontLine) {

      // If Over the Line Use Max Speed to Return
      if ((lineValue == 3 && OnLeftLine) || (lineValue == 2 && OnRightLine) || (lineValue == 1 && OnBackLine) || (lineValue == 4 && OnFrontLine)) {
        mc.setSpeed(MAX_SPEED);
      }

      else if ((ps.x < 20 && ps.x > 0) || (ps.x > FIELD_WIDTH / 2.0 - 20) || (ps.y < 20 && ps.y > 0) || (ps.y > FIELD_LENGTH - 20)) {
        mc.setSpeed(MAX_SPEED);
      }

      else {
        mc.setSpeed(LINE_SPEED);
      }

      offset = 0;
      return;
    }

    // Speed When Ball Behind

    if (ballBehind(direction)) {
      if (strength > MIN_CLOSE_STRENGTH) {
        mc.setSpeed(BALL_BEHIND_CLOSE_SPEED);
      } else {
        mc.setSpeed(BALL_BEHIND_SPEED);
      }
    } 
    else {
      mc.setSpeed(MAX_SPEED);
    }
  };

  // Tilting Towards the Goal With Trig
  void getOffset() {
    if (lineValue != 0 || OnLeftLine || OnRightLine || OnFrontLine || OnBackLine || !useOTOS) {
      offset = 0;
      return;
    }

    float x_from_center = ps.x - GOAL_POS_X;
    float y_from_goal = ps.y - GOAL_POS_Y;


    offset = atan(x_from_center / y_from_goal) * 180 / PI;
  };

  // Getting Goal XY Positon With Ultrasonics
  void getGoalXY(float& x, float& y) {
    x = -(ps.x - GOAL_POS_X);
    y = ps.y - GOAL_POS_Y;
  };

  // Setting Start Position Based on Idle Switch
  void setStartPos(String startPos) {
    startingPosition = startPos;
    if (startPos == "FORWARDS") {
      otos.setPos(91.5, 122.0, 0);
    } 
    else if (startPos == "BACK") {
      otos.setPos(91.5, 204, 0);
    } 
    else if (startPos == "DOWN") {
      otos.setPos(91.5, 179, 0);
    } 
    else if (startPos == "LEFT") {
      otos.setPos(57, 192, 0);
    } 
    else if (startPos == "RIGHT") {
      otos.setPos(126, 192, 0);
    }
  }

  // Staying Within the Lines (lineValue 1 is front, 2 is left, 3 is right, 4 is back)
  void stopAtLine() {

    // Stopping at Line Using OTOs and Colour Sensor
    if (useOTOS) {
      // On left line
      if (((lineValue == 2 || lineValue == 3) && ps.x < FIELD_WIDTH / 2.0 && ps.x > 0) || (ps.x < LINE_LIMIT && ps.x > 0)) {
        x = 1;
        OnLeftLine = true;
        OnRightLine = false;
      }

      // On right line
      else if (((lineValue == 2 || lineValue == 3) && ps.x > FIELD_WIDTH / 2.0) || (ps.x > FIELD_WIDTH - LINE_LIMIT)) {
        x = -1;
        OnLeftLine = false;
        OnRightLine = true;
      }

      // On neither side line
      else {
        OnLeftLine = false;
        OnRightLine = false;
      }

      if (((lineValue == 1 || lineValue == 4) && ps.y > FIELD_LENGTH / 2.0) || ps.y > FIELD_LENGTH - LINE_LIMIT) {
        y = 1;
        OnBackLine = true;
        OnFrontLine = false;
      }

      else if ((((lineValue == 1 || lineValue == 4) && ps.y < FIELD_LENGTH / 2.0 && ps.y > 0) || (ps.y < LINE_LIMIT && ps.y > 0)) && (ps.x < GOAL_POS_X - 22.5 || ps.x > GOAL_POS_X + 22.5) ) {
        y = -1;
        OnBackLine = false;
        OnFrontLine = true;
      }

      else if (OnLeftLine || OnFrontLine) {
        y = 0;
        OnBackLine = false;
        OnFrontLine = false;
      }

      else {
        OnBackLine = false;
        OnFrontLine = false;
      }

    }

    // Stopping at Line Using Just Colour Sensor
    else {
      if (lineValue == 2) {
        x = 1;
        OnLeftLine = true;
        OnRightLine = false;
      }

      else if (lineValue == 3) {
        x = -1;
        OnLeftLine = false;
        OnRightLine = true;
      }

      else {
        OnLeftLine = false;
        OnRightLine = false;
      }

      if (lineValue == 1) {
        y = 1;
        OnBackLine = true;
        OnFrontLine = false;
      }

      else if (lineValue == 4) {
        y = -1;
        OnBackLine = false;
        OnFrontLine = true;
      }

      else if (OnLeftLine || OnFrontLine) {
        y = 0;
        OnBackLine = false;
        OnFrontLine = false;
      }

      else {
        OnBackLine = false;
        OnFrontLine = false;
      }
    }

    if ((OnFrontLine || OnBackLine) && !(OnRightLine || OnLeftLine)) {
      x = 0;
    }
  };

  // Final Running of Robot
  void run() {
    // Running Motors Using Motor Controller
    mc.runMotors(x, y, tilt, offset);
  };
};
