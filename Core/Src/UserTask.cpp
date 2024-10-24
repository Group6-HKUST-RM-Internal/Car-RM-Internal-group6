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


M3508 m3508[4];

static bool motorOn = false;
static int16_t current[4];

static PID rpm_PID(500,50,0);
static PID rmp_PID_small(100,10,0);

static float targetRPM = 0; // target rpm


// an example task
void mainTask(void *pvPara) {
  initMotor(m3508);
  while (1) {
    // Press & Release detection (flip motorOn when button is from pressed to
    // released)
    // TODO: implement the PID controller to control the rpm or position
      // Start spinning when motorOn == 1, set a current to some value
    if(readButton0()){
    current[0] = rmp_PID_small.update(targetRPM, m3508[0].rpm);
    current[1] = rmp_PID_small.update(targetRPM, m3508[1].rpm);
    transmit(current);
    vTaskDelay(1);
    }
    current[0] = 0;
    current[1] = 0;
    current[2] = 0;
    current[3] = 0;
    transmit(current);
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
