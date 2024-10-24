#include "MPU.hpp"

namespace MPU
{
    // Define the structure of the IMU data
    ImuData imuData;

    // Get the constant pointer of the current decoded data
    const ImuData *getImuData()
    {
        return &imuData;
    }

    // Reset the data of the IMU
    void resetData()
    {
        imuData.accX = 0.0f;
        imuData.accY = 0.0f;
        imuData.accZ = 0.0f;
        imuData.gyroX = 0.0f;
        imuData.gyroY = 0.0f;
        imuData.gyroZ = 0.0f;
        imuData.magX = 0.0f;
        imuData.magY = 0.0f;
        imuData.magZ = 0.0f;
        imuData.temp = 0.0f;
    }

    
}