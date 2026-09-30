/**
 * @file TouchButton.hpp
 * @brief TouchButton class definition
 * @author Franklin
 */
#ifndef TOUCHBUTTON_HPP
#define TOUCHBUTTON_HPP

#include "extern.hpp"

namespace Touch {

    /**
     * @class TouchButton
     * @brief Based on Adafruit TouchButton library.
     */
    class TouchButton {

        private:
            unsigned short PIN; ///< GPIO port connected to button.
            float threshold; ///< Touch threshold.

        public:

            /**
             * @brief Default constructor.
             */
            TouchButton() {}

            /**
             * @brief Constructor with parameters.
             * @param _PIN Button pin.
             * @param _threshold Touch threshold.
             */
            TouchButton(unsigned short _PIN, float _threshold) : PIN(_PIN), threshold(_threshold) {}

            /**
             * @brief Checks if the button was touched.
             * @return `true` if the button was touched, `false` otherwise.
             */
            bool touched();

            /**
             * @brief Sets the button GPIO port (pin).
             * @param _PIN Button pin.
             */
            void setPin(unsigned short _PIN);

            /**
             * @brief Sets the touch threshold.
             * @param _threshold Touch threshold.
             */
            void setThreshold(float _threshold);

    };

}

#endif