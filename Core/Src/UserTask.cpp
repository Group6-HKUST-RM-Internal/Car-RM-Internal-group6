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
#include "FSi6x.hpp"


StackType_t uxMainTaskStack[configMINIMAL_STACK_SIZE];
StaticTask_t xMainTaskTCB;

// an example task
void mainTask(void *pvPara) {

  // Initialize the FSi6X module
  FSi6x::init();

  // Define the remote controller data
  static FSi6x::RcData rcData;

  while (true) {

    // Get the remote controller data
    rcData = *FSi6x::getRcData();
  
    vTaskDelay(1);
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
