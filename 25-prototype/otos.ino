/* ##### Optical Tracking Odometry Sensor ##### */

// OTOs Initilisation
QwiicOTOS myOtos;

// OTOs Class
class Otos {
  public:
  
  // OTOs Initial Setup
  void setUp() {
    myOtos.calibrateImu();
    myOtos.setLinearUnit(0);
    myOtos.setLinearScalar(1.08);
    myOtos.setAngularScalar(0.995);
    sfe_otos_pose2d_t offset = {0, 0.05, 0};
    myOtos.setOffset(offset);
    myOtos.resetTracking();
  };

  // Returning X/Y Positions (and Converting from Meters to Centimeters)
  array<float, 3> getXYH() {
    float heading = 0.0;
    sfe_otos_pose2d_t myPosition;
    myOtos.getPosition(myPosition);

    heading = myPosition.h;
    if (heading < 0) {
      heading = abs(heading);
    }
    else {
      heading = 360 - heading;
    }
    
    return {myPosition.x * 100.0, myPosition.y * -100.0, heading};
  };
  
  // Setting the Start Position
  void setPos(float x, float y, float tilt) {
    sfe_otos_pose2d_t pos = {x/100, y/-100, tilt};
    myOtos.setPosition(pos);
  };

  void printOTOS() {
    sfe_otos_pose2d_t myPosition;
    myOtos.getPosition(myPosition);

    Serial.println("\nNote: This is Without Drift Equasions")
    Serial.print("X: ");
    Serial.println(myPosition.x * 100);
    Serial.print("Y: ");
    Serial.println(myPosition.y * -100);
    Serial.print("OTOs TILT: ");
    Serial.println(myPosition.h);
  }
};