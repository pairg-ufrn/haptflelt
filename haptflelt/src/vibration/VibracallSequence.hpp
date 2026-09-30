/**
 * @file VibracallSequence.hpp
 * @brief VibracallSequence class definition
 * @author Franklin
 */
#ifndef VIBRACALLSEQUENCE_HPP
#define VIBRACALLSEQUENCE_HPP

#include "extern.hpp"
#include "Vibracall.hpp"

namespace Vibration {

    /**
     * @class VibracallSequence
     * @brief Represents a sequence of vibracalls.
     */
    class VibracallSequence {

        private:
            std::vector<Vibracall> sequence; ///< Sequence of vibracalls.

        public:
            
            /**
             * @brief Default constructor.
             */
            VibracallSequence() : sequence(0) {}

            /**
             * @brief Constructor with parameters.
             * @param PINS Array with vibracalls PINS.
             * @param _size Number of vibracalls.
             */
            VibracallSequence(const unsigned short* PINS, std::size_t _size) : sequence(_size) {
                for(int i = 0; i < _size; i++) {
                    Vibracall temp(PINS[i]);
                    this->sequence[i] = temp;

                    pinMode(PINS[i], OUTPUT);
                }
            }

            /**
             * @brief Turns on the vibracall.
             * @param _index Vibracall index in the sequence.
             */
            void turnOn(unsigned short _index);

            /**
             * @brief Turns on multiple vibracalls.
             * @param _indexes Vibracalls indexes in the sequence.
             * @param _size Number of vibracalls.
             */
            void turnOn(const unsigned short* _indexes, std::size_t _size);

            /**
             * @brief Turns off the vibracall.
             * @param _index Vibracall index in the sequence.
             */
            void turnOff(unsigned short _index);

            /**
             * @brief Turns off multiple vibracalls.
             * @param _indexes Vibracalls indexes in the sequence.
             * @param _size Number of vibracalls.
             */
            void turnOff(const unsigned short* _indexes, std::size_t _size);

            /**
             * @brief Checks if a vibracall is on.
             * @param _index Vibracall index to check if it is on.
             * @return `true` if the respective vibracall is active, `false` otherwise.
             */
            bool isOn(unsigned short _index);

            /**
             * @brief Turns on the specific vibracall for a certain period of time.
             * @param _index Vibracall index to turn on.
             * @param milliseconds Vibration duration.
             */
            void vibrate(unsigned short _index, uint32_t milliseconds);

            /**
             * @brief Turns on multiple vibracalls for a certain period of time.
             * @param _indexes Vibracalls indexes to turn on.
             * @param _size Number of vibracalls.
             * @param milliseconds Vibration duration.
             */
            void vibrate(unsigned short* _indexes, std::size_t _size, uint32_t milliseconds);

    };

}

#endif