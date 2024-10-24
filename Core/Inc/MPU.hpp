#include "main.h"

namespace MPU
{
    // Define the structure of the IMU data
    struct ImuData
    {
        // Define the decoded data group of the IMU

        // Accelerometer data
        float accX;
        float accY;
        float accZ;

        // Gyroscope data
        float gyroX;
        float gyroY;
        float gyroZ;

        // Magnetometer data
        float magX;
        float magY;
        float magZ;

        // Temperature data
        float temp;
    };

    // Return the constant pointer of the current decoded data
    const ImuData *getImuData();

    // Reset the data of the IMU
    void resetData();

    // Define the callback function of the I2C data reception
    void imuCallback(I2C_HandleTypeDef *hi2c, uint16_t size);

    // Define the callback function of the I2C error
    void erCallback(I2C_HandleTypeDef *hi2c);
}