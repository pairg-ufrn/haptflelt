/**
 * @file VibrationSystem.hpp
 * @brief VibrationSystem class definition
 * @author Franklin
 */
#ifndef VibrationSystem_HPP
#define VibrationSystem_HPP

#include "extern.hpp"
#include "VibracallSequence.hpp"

namespace Vibration {

    /**
     * @enum feedback_direction_t
     * @brief Represents the haptic feedback directions.
     */
    enum class feedback_direction_t {
        FRONT, BACK, LEFT, RIGHT,

        FRONT_LEFT, FRONT_RIGHT, BACK_LEFT, BACK_RIGHT,

        NONE
    };

    /**
     * @class VibrationSystem
     * @brief Manages the vibracalls.
     */
    class VibrationSystem {

        private:
            VibracallSequence* vibracalls; ///< Sequence of vibracalls.

            /**
             * @brief Identifies direction-related vibracalls.
             * @param _direction Haptic feedback direction.
             * @return Array with respective vibracalls indexes.
             */
            std::vector<unsigned short> directionToIndex(feedback_direction_t _direction);

        public:

            /**
             * @brief Constructor with parameters.
             * @param _PINS Array with vibracalls PINS.
             * @param _size Number of vibracalls.
             */
            VibrationSystem(unsigned short* _PINS, std::size_t _size) {
                this->vibracalls = new VibracallSequence(_PINS, _size);
            }

            /**
             * @brief Sends an haptic feedback.
             * @param _direction Direction to send the haptic feedback.
             */
            void hapticFeedback(feedback_direction_t _direction);

            /**
             * @brief Stops an active haptic feedback.
             * @param _direction Direction to stop the haptic feedback.
             */
            void stopHapticFeedback(feedback_direction_t _direction);

            /**
             * @brief Sends an haptic feedback for a certain period of time.
             * @param _direction Direction to send the haptic feedback.
             * @param milliseconds Vibration duration.
             */
            void hapticFeedback(feedback_direction_t _direction, uint32_t milliseconds);

            /**
             * @brief Stops all the active haptic feedbacks.
             */
            void stopAll();

    };

}

#endif