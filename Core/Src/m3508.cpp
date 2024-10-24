#include "m3508.hpp"

#include "fdcan.h"
#include "gpio.h"
#include "math.h"
#include "pid.hpp"

#define M3508_REDUCTION_RATIO (3591.0f / 187.0f)

extern "C" {

static inline int16_t clamp(int16_t value, int16_t min, int16_t max) {
  return value < min ? min : value > max ? max : value;
}

M3508 *motor;

static bool motorRegistered = false;

static uint8_t can_tx_buf[8];
static uint8_t can_rx_buf[8];

FDCAN_TxHeaderTypeDef txHeader = {.Identifier = 0x200,
                                  .IdType = FDCAN_STANDARD_ID,
                                  .TxFrameType = FDCAN_DATA_FRAME,
                                  .DataLength = FDCAN_DLC_BYTES_8,
                                  .ErrorStateIndicator = FDCAN_ESI_PASSIVE,
                                  .BitRateSwitch = FDCAN_BRS_OFF,
                                  .FDFormat = FDCAN_CLASSIC_CAN,
                                  .TxEventFifoControl = FDCAN_NO_TX_EVENTS,
                                  .MessageMarker = 0};

FDCAN_FilterTypeDef filter = {.IdType = FDCAN_STANDARD_ID,
                              .FilterIndex = 0,
                              .FilterType = FDCAN_FILTER_MASK,
                              .FilterConfig = FDCAN_FILTER_TO_RXFIFO0,
                              .FilterID1 = 0x200,
                              .FilterID2 = 0x7F0};

void CAN_RxCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
  // some data treatment
  if (RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) {
    FDCAN_RxHeaderTypeDef rxHeader;
    HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &rxHeader, can_rx_buf);

    // decode
    static int32_t rotoryCnt = 0;
    static uint16_t lastRawEncoder = 0;

    motor->id = rxHeader.Identifier;
    motor->rawEncoder = (can_rx_buf[0] << 8) | can_rx_buf[1];
    motor->rotorRPM = (can_rx_buf[2] << 8) | can_rx_buf[3];
    motor->torque_current = (can_rx_buf[4] << 8) | can_rx_buf[5];
    motor->temperature = can_rx_buf[6];

    if (motor->rawEncoder - lastRawEncoder > 4096) {
      rotoryCnt--;
    } else if (motor->rawEncoder - lastRawEncoder < -4096) {
      rotoryCnt++;
    }

    motor->accumulatedPosition = (rotoryCnt * 8192 + motor->rawEncoder) /
                                 8192.0f * 2 * 3.1415926f /
                                 M3508_REDUCTION_RATIO;
    motor->rpm = (float)motor->rotorRPM / M3508_REDUCTION_RATIO;

    lastRawEncoder = motor->rawEncoder;
  }
}

void transmit(int16_t current) {

  int16_t val = (clamp(current, -8000, 8000));

  can_tx_buf[0] = (val >> 8) & 0xFF;
  can_tx_buf[1] = (val & 0xFF);
  can_tx_buf[2] = (val >> 8) & 0xFF;
  can_tx_buf[3] = (val & 0xFF);
  can_tx_buf[4] = (val >> 8) & 0xFF;
  can_tx_buf[5] = (val & 0xFF);
  can_tx_buf[6] = (val >> 8) & 0xFF;
  can_tx_buf[7] = (val & 0xFF);

  HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &txHeader, can_tx_buf);
}

void initMotor(M3508 *motor_) {
  if (motor_ == nullptr || motorRegistered)
    return;
  motor = motor_;

  HAL_FDCAN_RegisterRxFifo0Callback(&hfdcan1, CAN_RxCallback);
  HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT,
                               FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE);
  HAL_FDCAN_ConfigFilter(&hfdcan1, &filter);
  HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE,
                                 0xFFFFFFFF);

  HAL_FDCAN_Start(&hfdcan1);

  motorRegistered = true;
}
}

#undef M3508_REDUCTION_RATIO