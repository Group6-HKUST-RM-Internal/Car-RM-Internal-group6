#include "main.h"

#include "string.h"

#pragma once

namespace FSi6x
{
    // Define the structure of the remote controller data
    struct RcData
    {
        // Define the decoded data group of the remote controller

        // Channel 1 to 16
        uint16_t channel1;
        uint16_t channel2;
        uint16_t channel3;
        uint16_t channel4;
        uint16_t channel5;
        uint16_t channel6;
        uint16_t channel7;
        uint16_t channel8;
        uint16_t channel9;
        uint16_t channel10;
        uint16_t channel11;
        uint16_t channel12;
        uint16_t channel13;
        uint16_t channel14;
        uint16_t channel15;
        uint16_t channel16;

        // Channel 17 and 18
        bool channel17;
        bool channel18;

        // Is Frame lost of the remote controller
        bool isFrameLost;

        // Is fail safe activated of the remote controller
        bool isFailSafeActivated;

        // Is connected to the remote controller
        bool isConnected;

        // Is error of the remote controller
        bool isError;
    };
    
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