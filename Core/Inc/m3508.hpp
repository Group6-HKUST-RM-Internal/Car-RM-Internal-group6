#include "main.h"

extern "C" {

// struct M3508 {
//   uint16_t id;
//   uint16_t angle;
//   int16_t speed;
//   int16_t current;
//   uint8_t temperature;
// };

struct M3508 {
  uint16_t id;
  uint16_t rawEncoder;
  int16_t rotorRPM;
  int16_t torque_current;
  uint8_t temperature;

  float accumulatedPosition;
  float rpm;
};

void initMotor(M3508 *motor_);

void transmit(int16_t current);

}