/**
 * @file RGBLED.hpp
 * @brief RGBLED class definition
 * @author Franklin
 */
#ifndef RGBLED_HPP
#define RGBLED_HPP

#include "extern.hpp"

namespace Ilumination {
    
    /**
     * @class RGBLED
     * @brief Represents a RGB LED and controls it.
     */
    class RGBLED {

        private:
            uint8_t RED; ///< RED value from 0 to 255
            uint8_t GREEN; ///< GREEN value from 0 to 255
            uint8_t BLUE; ///< BLUE value from 0 to 255

            unsigned short RED_PIN; ///< GPIO port connected to LED's RED pin.
            unsigned short GREEN_PIN; ///< GPIO port connected to LED's GREEN pin.
            unsigned short BLUE_PIN; ///< GPIO port connected to LED's BLUE pin.

            /**
             * @brief Writes RGB values.
             */
            void writeRGBValues();

        public:
            
            /**
             * @brief Constructor with parameters.
             * @param _RED_PIN GPIO port connected to LED's RED pin.
             * @param _GREEN_PIN GPIO port connected to LED's GREEN pin.
             * @param _BLUE_PIN GPIO port connected to LED's BLUE pin.
             */
            RGBLED(unsigned short _RED_PIN, unsigned short _GREEN_PIN, unsigned short _BLUE_PIN) {
                this->RED = 0;
                this->GREEN = 0;
                this->BLUE = 0;

                this->RED_PIN = _RED_PIN;
                this->GREEN_PIN = _GREEN_PIN;
                this->BLUE_PIN = _BLUE_PIN;

                pinMode(this->RED_PIN, OUTPUT);
                pinMode(this->GREEN_PIN, OUTPUT);
                pinMode(this->BLUE_PIN, OUTPUT);
            }
            
            /**
             * @brief Turns on the LED in red.
             */
            void turnOnRed();

            /**
             * @brief Turns on the LED in blue.
             */
            void turnOnBlue();

            /**
             * @brief Turns on the LED in green.
             */
            void turnOnGreen();

            /**
             * @brief Turns on the LED in a custom color.
             * @param _RED RED value from 0 to 255.
             * @param _GREEN GREEN value from 0 to 255.
             * @param _BLUE BLUE value from 0 to 255.
             */
            void turnOnCustom(uint8_t _RED, uint8_t _GREEN, uint8_t _BLUE);
            
            /**
             * @brief Turns off the LED.
             */
            void turnOff();
            
            /**
             * @brief Checks if the LED is on.
             * @return `true` if LED is on, `false` otherwise.
             */
            bool isOn();
    };

}

#endif