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

// Define the motor
M3508 m3508[4];


static int16_t current[4]; //Array of current for each motor. ID=1 motor corresponds to current[0], and so on

static PID rpm_PID(500,50,0); // PID controller for RPM

static float targetRPM[4]; // target rpm


// an example task
void mainTask(void *pvPara) {
  // Initialize the motor
  initMotor(m3508);
  while (1) {
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
