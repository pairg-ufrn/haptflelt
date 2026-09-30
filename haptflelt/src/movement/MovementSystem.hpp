/**
 * @file MovementSystem.hpp
 * @brief MovementSystem class definition
 * @author Franklin
 */
#ifndef MOVEMENTSYSTEM_HPP
#define MOVEMENTSYSTEM_HPP

#include "extern.hpp"
#include "MPU6050.hpp"

namespace Movement {

    /**
     * @enum movement_t
     * @brief Represents types of movement.
     */
    enum class movement_t {
        UP, DOWN, LEFT, RIGHT, FRONT, BACK,  // Movement types.

        NONE // No movement.
    };

    /**
     * @class MovementSystem
     * @brief Detects movements based on the MPU6050 events.
     */
    class MovementSystem {

        private:
            MPU6050 mpu; ///< MPU6050.

            float threshold_enter; ///< Threshold (m/s^2) to activate a movement.
            float threshold_exit; ///< Threshold (m/s^2) to reset a movement.
            float dominance_margin; ///< How much (m/s^2) the dominant axis must exceed the others.
            unsigned long max_movement_duration_ms; ///< Maximum duration (ms) of a movement.
            unsigned long min_movement_duration_ms; ///< Minimum duration (ms) of a movement.
            unsigned long opposite_cooldown_ms; ///< Time (ms) the opposite of an ended movement stays suppressed.

            float baseline_x = 0.0f; ///< Resting value of the X axis.
            float baseline_y = 0.0f; ///< Resting value of the Y axis.
            float baseline_z = 0.0f; ///< Resting value of the Z axis.

            movement_t current_movement = movement_t::NONE; ///< Current movement.
            unsigned long movement_start_time = 0; ///< Time (ms) when the current movement started.

            movement_t last_ended_movement = movement_t::NONE; ///< Last movement that ended.
            unsigned long cooldown_until = 0; ///< Time (ms) until the opposite of the last ended movement stays suppressed.

            /**
             * @brief Gets the opposite direction of a movement.
             * @param movement Movement.
             * @return Opposite movement, or `movement_t::NONE` if there is none.
             */
            static movement_t oppositeOf(movement_t movement);

            /**
             * @brief Ends the current movement.
             * @param now Current time (ms).
             */
            void endCurrentMovement(unsigned long now);

            /**
             * @brief Calibrates the resting position.
             * @param samples Number of readings used to compute the average.
             */
            void calibrate(uint16_t samples = 200);

            /**
             * @brief Gets the X axis deviation from the resting position.
             * @return Acceleration (m/s^2) deviation on the X axis.
             */
            float deltaX();

            /**
             * @brief Gets the Y axis deviation from the resting position.
             * @return Acceleration (m/s^2) deviation on the Y axis.
             */
            float deltaY();

            /**
             * @brief Gets the Z axis deviation from the resting position.
             * @return Acceleration (m/s^2) deviation on the Z axis.
             */
            float deltaZ();

        public:

            /**
             * @brief Constructor with parameters.
             * @param _threshold_enter Threshold (m/s^2) to activate a movement.
             * @param _threshold_exit Threshold (m/s^2) to reset a movement.
             * @param _dominance_margin How much (m/s^2) the dominant axis must exceed the others.
             * @param _max_movement_duration_ms Maximum duration (ms) of a movement.
             * @param _min_movement_duration_ms Minimum duration (ms) of a movement.
             * @param _opposite_cooldown_ms Time (ms) the opposite of an ended movement stays suppressed.
             */
            MovementSystem(float _threshold_enter, float _threshold_exit, float _dominance_margin,
                unsigned long _max_movement_duration_ms, unsigned long _min_movement_duration_ms,
                unsigned long _opposite_cooldown_ms) :
                threshold_enter(_threshold_enter),
                threshold_exit(_threshold_exit),
                dominance_margin(_dominance_margin),
                max_movement_duration_ms(_max_movement_duration_ms),
                min_movement_duration_ms(_min_movement_duration_ms),
                opposite_cooldown_ms(_opposite_cooldown_ms) {
            }

            /**
             * @brief Initializes the MPU6050 and calibrates the resting position.
             * @return `true` if the system was initialized, `false` otherwise.
             */
            bool initSystem();

            /**
             * @brief Detects the current movement.
             * @return Type of movement.
             */
            movement_t detectMovement();

            /**
             * @brief Recalibrates the resting position and resets the current movement.
             * @param samples Number of readings used to compute the average.
             */
            void recalibrate(uint16_t samples = 100);

            /**
             * @brief Prints the axes deviations and thresholds to Serial.
             */
            void debugPrint();

    };

}

#endif
