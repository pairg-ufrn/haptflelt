#include "MPU6050.hpp"

namespace Movement {

    bool MPU6050::begin() {
        bool conn = this->mpu.begin();

        if (!conn) return false;
        
        mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
        mpu.setGyroRange(MPU6050_RANGE_250_DEG);
        // Noise is already filtered by MovementSystem, so a faster filter keeps detection responsive.
        mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

        return true;
    }

    void MPU6050::update() {
        this->mpu.getEvent(&this->accelerometer, &this->gyroscope, &this->temperature);
    }

    sensors_vec_t MPU6050::getAccelerometerValues() {
        return this->accelerometer.acceleration;
    }

    float MPU6050::getAccelerometerValuesX() {
        return this->getAccelerometerValues().x;
    }

    float MPU6050::getAccelerometerValuesY() {
        return this->getAccelerometerValues().y;
    }

    float MPU6050::getAccelerometerValuesZ() {
        return this->getAccelerometerValues().z;
    }

    sensors_vec_t MPU6050::getGyroscopeValues() {
        return this->gyroscope.gyro;
    }

    float MPU6050::getGyroscopeValuesX() {
        return this->getGyroscopeValues().x;
    }

    float MPU6050::getGyroscopeValuesY() {
        return this->getGyroscopeValues().y;
    }

    float MPU6050::getGyroscopeValuesZ() {
        return this->getGyroscopeValues().z;
    }

}