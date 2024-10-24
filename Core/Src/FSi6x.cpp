#include "FSi6x.hpp"

#include "usart.h"

namespace FSi6x
{
    typedef struct RcData
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
        uint16_t channel17;
        uint16_t channel18;

        // Frame lost of the remote controller
        bool frameLost;

        // Is fail safe activated of the remote controller
        bool failSafeActivated;

        // Is connected to the remote controller
        bool isConnected;

        // Is error of the remote controller
        bool isError;
    };

    RcData rcData;

    const RcData *getRcData()
    {
        return &rcData;
    }

    void resetData()
    {
        // TODO: Implement the reset data of the FSi6X module
    }

    void rcCallback(UART_HandleTypeDef *huart, uint16_t size)
    {
        // TODO: Implement the callback function of the UART data reception
    }

    void erCallback(UART_HandleTypeDef *huart)
    {
        // TODO: Implement the callback function of the UART error
    }

    void init()
    {
        // TODO: Implement the initialization of the FSi6X module
        HAL_UART_RegisterRxEventCallback(&huart3, rcCallback);
        HAL_UART_RegisterCallback(&huart3, HAL_UART_ERROR_CB_ID, erCallback);
    }
}