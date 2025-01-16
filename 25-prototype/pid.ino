/* ##### PID Controller ##### */

// PID Class
class Pid {
  public:
    // Variable Initilisations
    double kp = PROPORTIONAL_CONSTANT;
    double ki = INTEGRAL_CONSTANT;
    double kd = DERIVETIVE_CONSTANT;
    double previousError, integral, derivitive;

    // Calculating Speed
    double compute(double error, double dt) {
      integral += error * dt;
      derivitive = (error - previousError) / dt;
      previousError = error;
      return kp * error + ki * integral + kd * derivitive; 
    };
};

