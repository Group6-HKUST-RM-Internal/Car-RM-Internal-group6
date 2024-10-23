#include "main.h"

#include "stdint.h"

#pragma once

namespace FSi6X
{
    typedef struct RcData;

    // Get the decoded remote controller data
    const RcData *getRcData();

    // Reset the data
    void resetData();

    // Receive callback
    void rxCallback(UART_HandleTypeDef *huart, uint16_t size);

    // Error callback
    void erCallback(UART_HandleTypeDef *huart);

    // Initialize the FSi6X module
    void init();
}