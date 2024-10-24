struct PID {
  PID(float Kp_, float Ki_, float Kd_)
      : error(0), lastError(0), Kp(Kp_), Ki(Ki_), Kd(Kd_), pOut(0), iOut(0), dOut(0), output(0) {}

  float update(float target, float measurement, float dt = 0.001f);

  void reset();

private:
  float error;
  float lastError;

  float Kp;
  float Ki;
  float Kd;

  float pOut;
  float iOut;
  float dOut;

  float output;
};