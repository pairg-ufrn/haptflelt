/**
 * @file MPU6050.hpp
 * @brief MPU6050 class definition
 * @author Franklin
 */
#ifndef MPU6050_HPP
#define MPU6050_HPP

#include "extern.hpp"
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

namespace Movement {

    /**
     * @class MPU6050
     * @brief Based on Adafruit MPU6050 library.
     */
    class MPU6050 {

        private:
            Adafruit_MPU6050 mpu; ///< MPU6050.

            sensors_event_t accelerometer; ///< MPU6050 accelerometer sensor.
            sensors_event_t gyroscope; ///< MPU6050 gyroscope sensor.
            sensors_event_t temperature; ///< MPU6050 temperature sensor.

        public:

            /**
             * @brief Default constructor.
             */
            MPU6050() {}

            /**
             * @brief Initializes and configures the MPU6050.
             * @return `true` if MPU6050 was initialized, `false` otherwise.
             */
            bool begin();

            /**
             * @brief Reads the sensors values.
             */
            void update();

            /**
             * @brief Gets the accelerometer values.
             * @return Acceleration (m/s^2) on the 3 axes.
             */
            sensors_vec_t getAccelerometerValues();

            /**
             * @brief Gets the accelerometer X axis value.
             * @return Acceleration (m/s^2) on the X axis.
             */
            float getAccelerometerValuesX();

            /**
             * @brief Gets the accelerometer Y axis value.
             * @return Acceleration (m/s^2) on the Y axis.
             */
            float getAccelerometerValuesY();

            /**
             * @brief Gets the accelerometer Z axis value.
             * @return Acceleration (m/s^2) on the Z axis.
             */
            float getAccelerometerValuesZ();

            /**
             * @brief Gets the gyroscope values.
             * @return Rotation (rad/s) on the 3 axes.
             */
            sensors_vec_t getGyroscopeValues();

            /**
             * @brief Gets the gyroscope X axis value.
             * @return Rotation (rad/s) on the X axis.
             */
            float getGyroscopeValuesX();

            /**
             * @brief Gets the gyroscope Y axis value.
             * @return Rotation (rad/s) on the Y axis.
             */
            float getGyroscopeValuesY();

            /**
             * @brief Gets the gyroscope Z axis value.
             * @return Rotation (rad/s) on the Z axis.
             */
            float getGyroscopeValuesZ();

    };

}

#endif