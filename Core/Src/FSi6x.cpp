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
    };
}