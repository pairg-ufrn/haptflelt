/**
 * @file TouchSystem.hpp
 * @brief TouchSystem class definition
 * @author Franklin
 */
#ifndef TOUCHSYSTEM_HPP
#define TOUCHSYSTEM_HPP

#include "extern.hpp"
#include "TouchButton.hpp"

namespace Touch {

    /**
     * @enum touch_t
     * @brief Represents types of touch, both single and combined.
     */
    enum class touch_t {
        LEFT, RIGHT, FRONT_RIGHT, FRONT_LEFT, // Single touches.

        L_R, L_FR, R_FL, FR_FL, // Combined touches.

        ALL, // All 4 buttons at the same time (used for secret commands).

        NONE // No touch.
    };

    /**
     * @enum atomic_touch_t
     * @brief Represents the single (atomic) types of touch.
     */
    enum class atomic_touch_t {
        LEFT, RIGHT, FRONT_RIGHT, FRONT_LEFT
    };

    /**
     * @class TouchSystem
     * @brief Manages the touch buttons operations.
     */
    class TouchSystem {

        private:
            float threshold; ///< Touch threshold.

            TouchButton rightButton; ///< Represents the right touch button.
            TouchButton leftButton; ///< Represents the left touch button.
            TouchButton frontRightButton; ///< Represents the front right touch button.
            TouchButton frontLeftButton; ///< Represents the front left touch button.

        public:

            /**
             * @brief Default constructor.
             */
            TouchSystem() {}

            /**
             * @brief Constructor with parameters.
             * @param _pinRight Right button pin.
             * @param _pinLeft Left button pin.
             * @param _pinFrontRight Front right button pin.
             * @param _pinFrontLeft Front left button pin.
             * @param _threshold Touch threshold.
             */
            TouchSystem(unsigned short _pinRight, unsigned short _pinLeft, 
                unsigned short _pinFrontRight, unsigned short _pinFrontLeft, 
                float _threshold) : 
                    rightButton(_pinRight, _threshold),
                    leftButton(_pinLeft, _threshold),
                    frontRightButton(_pinFrontRight, _threshold),
                    frontLeftButton(_pinFrontLeft, _threshold)
                {}

            /**
             * @brief Detects if some button was touched.
             * @return Type of touch.
             */
            touch_t detectTouch();

    };

}

#endif