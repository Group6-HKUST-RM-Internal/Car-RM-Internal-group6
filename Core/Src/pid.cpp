#include "pid.hpp"
float PID::update(float target, float measurement, float dt)
{

  // --------------- PID Controller --------------- //
  lastError = error;
  error = target - measurement;

  pOut = Kp * error;
  iOut = iOut + Ki * error * dt;
  dOut = Kd * (error - lastError) / dt;
  output = pOut + iOut + dOut;
  if (output > 16384)
  {
    output = 16384;
  }
  if (output < -16384)
  {
    output = -16384;
  }
  return output;
}

void PID::reset()
{
  pOut = 0;
  iOut = 0;
  dOut = 0;
  error = 0;
  lastError = 0;
}