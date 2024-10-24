#include "main.h"
#include "FSi6x.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"

namespace FSi6x
{

    static RcData rcData;

    const RcData *getRcData()
    {
        return &rcData;
    }

    void resetData()
    {
        rcData.channel1 = 1024;
        rcData.channel2 = 1024;
        rcData.channel3 = 1024;
        rcData.channel4 = 1024;
        rcData.channel5 = 240;
        rcData.channel6 = 240;
        rcData.channel7 = 240;
        rcData.channel8 = 240;
        rcData.channel9 = 240;
        rcData.channel10 = 240;
        rcData.channel11 = 1024;
        rcData.channel12 = 1024;
        rcData.channel13 = 1024;
        rcData.channel14 = 1024;
        rcData.channel15 = 1024;
        rcData.channel16 = 1024;
        rcData.channel17 = false;
        rcData.channel18 = false;
        rcData.isFrameLost = false;
        rcData.isFailSafeActivated = false;   
    }

    // Define the buffer of the remote controller
    static uint8_t rcBuff[25];

    bool checkRange()
    {

        // Check if the remote controller data is within the range

        if(rcData.channel1 < 240 || rcData.channel1 > 1807)
        {
            return true;
        }

        if(rcData.channel2 < 240 || rcData.channel2 > 1807)
        {
            return true;
        }

        if(rcData.channel3 < 240 || rcData.channel3 > 1807)
        {
            return true;
        }

        if(rcData.channel4 < 240 || rcData.channel4 > 1807)
        {
            return true;
        }

        if(rcData.channel5 < 240 || rcData.channel5 > 1807)
        {
            return true;
        }

        if(rcData.channel6 < 240 || rcData.channel6 > 1807)
        {
            return true;
        }

        if(rcData.channel7 < 240 || rcData.channel7 > 1807)
        {
            return true;
        }

        if(rcData.channel8 < 240 || rcData.channel8 > 1807)
        {
            return true;
        }

        if(rcData.channel9 < 240 || rcData.channel9 > 1807)
        {
            return true;
        }

        if(rcData.channel10 < 240 || rcData.channel10 > 1807)
        {
            return true;
        }

        if(rcData.channel11 < 240 || rcData.channel11 > 1807)
        {
            return true;
        }

        if(rcData.channel12 < 240 || rcData.channel12 > 1807)
        {
            return true;
        }

        if(rcData.channel13 < 240 || rcData.channel13 > 1807)
        {
            return true;
        }

        if(rcData.channel14 < 240 || rcData.channel14 > 1807)
        {
            return true;
        }

        if(rcData.channel15 < 240 || rcData.channel15 > 1807)
        {
            return true;
        }

        if(rcData.channel16 < 240 || rcData.channel16 > 1807)
        {
            return true;
        }

        // Return false if all the channels are within the range
        return false;
    }

    uint32_t counter = 0;
    StackType_t uxCounterTaskStack[256];
    StaticTask_t xCounterTaskTCB;

    void counterTask(void *pvPara)
    {
        // Let the counter task run forever to check the performance of the system
        while (true)
        {

            if(counter > 200)
            {
                // Reset the data of the remote controller
                resetData();

                // Set the remote controller is not connected
                rcData.isConnected = false;

                // Set the error flag of the remote controller
                rcData.isError = true;
            }else 
            {
                counter ++;
            }

            vTaskDelay(1);
        }
    }

    void rcCallback(UART_HandleTypeDef *huart, uint16_t size)
    {
        // Reset the counter
        counter = 0;

        // Initialize the remote controller status
        rcData.isConnected = true;
        rcData.isError = false;

        // Check the header of the remote controller data
        if(rcBuff[0] != 0x0F)
        {
            // Reset the data of the remote controller
            resetData();

            // Set the remote controller is not connected
            rcData.isConnected = false;

            // Set the error flag of the remote controller
            rcData.isError = true;

            // Receive the next round of the UART data reception
            HAL_UARTEx_ReceiveToIdle_IT(&huart3, rcBuff, 25);

            // Avoid the following code execution
            return;
        }

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

        // Check if the remote controller is in failsafe mode or frame lost
        if(rcData.isFailSafeActivated || rcData.isFrameLost)
        {
            // Reset the data of the remote controller
            resetData();

            // Set the remote controller is not connected
            rcData.isConnected = false;

            // Set the error flag of the remote controller
            rcData.isError = true;

            // Receive the next round of the UART data reception
            HAL_UARTEx_ReceiveToIdle_IT(&huart3, rcBuff, 25);

            // Avoid the following code execution
            return;
        }


        // Check if the remote controller data is within the range
        if(checkRange())
        {
            // Reset the data of the remote controller
            resetData();

            // Set the remote controller is not connected
            rcData.isConnected = false;

            // Set the error flag of the remote controller
            rcData.isError = true;

            // Receive the next round of the UART data reception
            HAL_UARTEx_ReceiveToIdle_IT(&huart3, rcBuff, 25);

            // Avoid the following code execution
            return;
        }

        // Receive the next round of the UART data reception
        HAL_UARTEx_ReceiveToIdle_IT(&huart3, rcBuff, 25);
    }

    void erCallback(UART_HandleTypeDef *huart)
    {
        // Reset the data of the remote controller
        resetData();

        // Set the remote controller is not connected
        rcData.isConnected = false;

        // Set the error flag of the remote controller
        rcData.isError = true;

        // Receive the next round of the UART data reception
        HAL_UARTEx_ReceiveToIdle_IT(&huart3, rcBuff, 25);
    }

    void init()
    {
        // Register the callback function of the UART data reception
        HAL_UART_RegisterRxEventCallback(&huart3, rcCallback);

        // Register the callback function of the UART error
        HAL_UART_RegisterCallback(&huart3, HAL_UART_ERROR_CB_ID, erCallback);

        // Start the first round of the UART data reception
        HAL_UARTEx_ReceiveToIdle_IT(&huart3, rcBuff, 25);

        // Create the counter task
        xTaskCreateStatic(counterTask, "counterTask", 256, NULL, 0, uxCounterTaskStack, &xCounterTaskTCB);

        // Reset the data of the remote controller
        resetData();

        // Set the remote controller is connected
        rcData.isConnected = true;

        // Set the remote controller is not error
        rcData.isError = false;
    }
}