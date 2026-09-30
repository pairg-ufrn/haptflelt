/**
 * @file Vibracall.hpp
 * @brief Vibracall class definition
 * @author Franklin
 */
#ifndef VIBRACALL_HPP
#define VIBRACALL_HPP

#include "extern.hpp"

namespace Vibration {

    /**
     * @class Vibracall
     * @brief Represents a Vibracall and controls it.
     */
    class Vibracall {

        private:
            bool on; ///< `true` if vibracall is active, `false` otherwise
            unsigned short PIN; ///< GPIO port connected to vibracall.

        public:

            /**
             * @brief Default constructor.
             */
            Vibracall() : on(false), PIN(0) {}

            /**
             * @brief Constructor with parameters.
             * @param _PIN GPIO port connected to vibracall.
             */
            Vibracall(unsigned short _PIN) : PIN(_PIN), on(false) {
                pinMode(this->PIN, OUTPUT);
            }

            /**
             * @brief Turns on the vibracall.
             */
            void turnOn();

            /**
             * @brief Turns off the vibracall.
             */
            void turnOff();

            /**
             * @brief Checks if the vibracall is on.
             * @return `true` if vibracall is active, `false` otherwise.
             */
            bool isOn();

            /**
             * @brief Turns on the vibracall for a certain period of time.
             * @param milliseconds Vibration duration.
             */
            void vibrate(uint32_t milliseconds);
    };

}

#endif