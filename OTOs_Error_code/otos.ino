QwiicOTOS myOtos;
class Otos {
  public:

  void SetUp() {
    myOtos.calibrateImu();
    myOtos.setLinearUnit(0);
    sfe_otos_pose2d_t offset = {0.04,0.04, 0};
    myOtos.setOffset(offset);
    myOtos.resetTracking();
  }

  array<float, 2> GetXY() {
    sfe_otos_pose2d_t myPosition;
    myOtos.getPosition(myPosition);
    return {myPosition.x * 100, myPosition.y * 100};
  }

  void SetPos(float x, float y, float tilt) {
    sfe_otos_pose2d_t pos = {x/100, y/100, tilt};
    myOtos.setPosition(pos);
  }

};