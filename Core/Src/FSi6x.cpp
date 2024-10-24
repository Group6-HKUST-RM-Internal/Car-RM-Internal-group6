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

        // Is Frame lost of the remote controller
        bool isFrameLost;

        // Is fail safe activated of the remote controller
        bool isFailSafeActivated;

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

    // Define the buffer of the remote controller
    static uint8_t rcBuff[25];

    void rcCallback(UART_HandleTypeDef *huart, uint16_t size)
    {
        // TODO: Implement the callback function of the UART data reception

        // Decode the remote controller data

        /*
            FSi6X Remote Controller Data Format

            Total Data Length: 25 Bytes

            Byte[0]: header, 0x0F
            Byte[1 -22]: 1 - 16 channels, 11 bits each
            Byte[23]
            Bit 0: channel 17 0x01)
            Bit 1: channel 18 0x02)
            Bit 2: frame lost 0x04)
            Bit 3: failsafe activated 0x08)
            Byte[24]: footer

        */

        rcData.channel1 = (rcBuff[1] | (rcBuff[2] << 8)) & 0x7FF;
        rcData.channel2 = ((rcBuff[2] >> 3) | (rcBuff[3] << 5)) & 0x7FF;
        rcData.channel3 = ((rcBuff[3] >> 6) | (rcBuff[4] << 2) | (rcBuff[5] << 10)) & 0x7FF;
        rcData.channel4 = ((rcBuff[5] >> 1) | (rcBuff[6] << 7)) & 0x7FF;
        rcData.channel5 = ((rcBuff[6] >> 4) | (rcBuff[7] << 4)) & 0x7FF;
        rcData.channel6 = ((rcBuff[7] >> 7) | (rcBuff[8] << 1) | (rcBuff[9] << 9)) & 0x7FF;
        rcData.channel7 = ((rcBuff[9] >> 2) | (rcBuff[10] << 6)) & 0x7FF;
        rcData.channel8 = ((rcBuff[10] >> 5) | (rcBuff[11] << 3)) & 0x7FF;
        rcData.channel9 = (rcBuff[12] | rcBuff[13] << 8) & 0x7FF;
        rcData.channel10 = ((rcBuff[13] >> 3) | (rcBuff[14] << 5)) & 0x7FF;
        rcData.channel11 = ((rcBuff[14] >> 6) | (rcBuff[15] << 2) | (rcBuff[16] << 10)) & 0x7FF;
        rcData.channel12 = ((rcBuff[16] >> 1) | (rcBuff[17] << 7)) & 0x7FF;
        rcData.channel13 = ((rcBuff[17] >> 4) | (rcBuff[18] << 4)) & 0x7FF;
        rcData.channel14 = ((rcBuff[18] >> 7) | (rcBuff[19] << 1) | (rcBuff[20] << 9)) & 0x7FF;
        rcData.channel15 = ((rcBuff[20] >> 2) | (rcBuff[21] << 6)) & 0x7FF;
        rcData.channel16 = ((rcBuff[21] >> 5) | (rcBuff[22] << 3)) & 0x7FF;
        rcData.channel17 = rcBuff[23] & 0x01;
        rcData.channel18 = rcBuff[23] & 0x02;
        rcData.isFrameLost = rcBuff[23] & 0x04;
        rcData.isFailSafeActivated = rcBuff[23] & 0x08;

        // Receive the next round of the UART data reception
        HAL_UARTEx_ReceiveToIdle_IT(&huart3, rxBuff, 25);
    }

    void erCallback(UART_HandleTypeDef *huart)
    {
        // Reset the data of the remote controller
        resetData();

        // Set the error flag of the remote controller
        rcData.isError = true;

        // Receive the next round of the UART data reception
        HAL_UARTEx_ReceiveToIdle_IT(&huart3, rxBuff, 25);
    }

    void init()
    {
        // Register the callback function of the UART data reception
        HAL_UART_RegisterRxEventCallback(&huart3, rcCallback);

        // Register the callback function of the UART error
        HAL_UART_RegisterCallback(&huart3, HAL_UART_ERROR_CB_ID, erCallback);

        // Start the first round of the UART data reception
        HAL_UARTEx_ReceiveToIdle_IT(&huart3, rxBuff, 25);

        // Set the remote controller is connected
        rcData.isConnected = true;

        // Set the remote controller is not error
        rcData.isError = false;
    }
}