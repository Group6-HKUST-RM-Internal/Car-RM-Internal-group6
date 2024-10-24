/**
 * @file UserTask.cpp
 * @author JIANG Yicheng  RM2023 (EthenJ@outlook.sg)
 * @brief Create user tasks with cpp support
 * @version 0.1
 * @date 2022-08-20
 *
 * @copyright Copyright (c) 2022
 */
#include "AppConfig.h" // Include our customized configuration
#include "FreeRTOS.h"
#include "gpio.h"
#include "main.h"
#include "task.h"
#include "pid.hpp"
#include "m3508.hpp"

StackType_t uxMainTaskStack[configMINIMAL_STACK_SIZE];
StaticTask_t xMainTaskTCB;

uint8_t readButton0(void) {
  return !HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
}

uint8_t readButton1(void) {
  return !HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14);
}


M3508 m3508;

static uint8_t lastButtonState0 = 0;
static uint8_t lastButtonState1 = 0;
static bool motorOn = false;
static int16_t current = 0;

static PID rpm_PID(0,0,0);
static PID pos_PID(0,0,0);

static float targetRPM = 0; // target rpm
static float targetPosition = 100; // target position


// an example task
void mainTask(void *pvPara) {
  initMotor(&m3508);
  while (1) {
    // Press & Release detection (flip motorOn when button is from pressed to
    // released)
    if (readButton0() != lastButtonState0 && readButton0() == 0) {
      motorOn = !motorOn;
    }
    if (readButton1() != lastButtonState1 && readButton1() == 0) {
      targetPosition = 3.1415926f / 2.0f + m3508.accumulatedPosition;
      targetRPM = 100.0f - targetRPM;
    }

    // TODO: implement the PID controller to control the rpm or position
    if (motorOn) {
      // Start spinning when motorOn == 1, set a current to some value
      current = rpm_PID.update(targetRPM, m3508.rpm);
    } else {
      // Stop spinning when motorOn == 0, set current to 0
      current = 0;
      // TODO: remember to reset PID controller when motor is off
      pos_PID.reset();
    }

    transmit(800);
    lastButtonState0 = readButton0();
    lastButtonState1 = readButton1();
    vTaskDelay(1);
    // Delay the whole program for 1ms after executing a round of loop
    // This is important because we do not want our program to refresh too fast.
  }
  
    
}

/**
 * @brief Intialize all the drivers and add task to the scheduler
 * @todo  Add your own task in this file
 */
void startUserTasks() {
  xTaskCreateStatic(mainTask, "testTask", configMINIMAL_STACK_SIZE, NULL, 0,
                    uxMainTaskStack, &xMainTaskTCB);
  /**
   * @todo Add your own task here
   */
}
