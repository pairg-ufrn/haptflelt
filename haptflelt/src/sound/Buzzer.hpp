/**
 * @file Buzzer.hpp
 * @brief Buzzer class definition
 * @author Franklin
 */
#ifndef BUZZER_HPP
#define BUZZER_HPP

#include "extern.hpp"

namespace Sound {

    /**
     * @class Buzzer
     * @brief Represents a buzzer and controls it.
     */
    class Buzzer {

        private:
            bool on; ///< `true` if buzzer is active, `false` otherwise
            unsigned short PIN; ///< GPIO port connected to buzzer.

            /**
             * @brief Write buzzer value, i.e., if its on or off.
             */
            void writeBuzzerValue();

        public:

            /**
             * @brief Constructor with parameters.
             * @param _PIN GPIO port connected to buzzer's pin.
             */
            Buzzer(unsigned short _PIN) : PIN(_PIN), on(false) {
                pinMode(this->PIN, OUTPUT);
            }
            
            /**
             * @brief Turns on the buzzer.
             */
            void turnOn();

            /**
             * @brief Turns off the buzzer.
             */
            void turnOff();

            /**
             * @brief Checks if the buzzer is on.
             * @return `true` if buzzer is active, `false` otherwise.
             */
            bool isOn();

            /**
             * @brief The buzzer beeps for a certain period of time.
             * @param milliseconds Whistle duration.
             */
            void whistle(uint32_t milliseconds);
    };

}

#endif