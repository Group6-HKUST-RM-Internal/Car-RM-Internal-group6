#include "main.h"

#include "string.h"

#pragma once

namespace FSi6x
{
    // Define the structure of the remote controller data
    typedef struct RcData;
    
    // Return the constant pointer of the current decoded data
    const RcData *getRcData();

    // Reset the data of the remote controller
    void resetData();

    // Define the callback function of the UART data reception
    void rcCallback(UART_HandleTypeDef *huart, uint16_t size);

    // Define the callback function of the UART error
    void erCallback(UART_HandleTypeDef *huart);

    // Define the initialization function of the FSi6X module
    void init();
}