/**
 * @file HaptFlelt.hpp
 * @brief HaptFlelt class definition
 * @author Franklin
 */
#ifndef HAPTFLELT_HPP
#define HAPTFLELT_HPP

#include "extern.hpp"
#include "intern.hpp"

#define VAR_PIN_RED    5
#define VAR_PIN_GREEN  18
#define VAR_PIN_BLUE   19

#define VAR_TOUCH_RIGHT 15
#define VAR_TOUCH_LEFT 14

#define VAR_TOUCH_FRONT_RIGHT 13
#define VAR_TOUCH_FRONT_LEFT 12

#define VAR_PIN_BUZZER 4

#define VAR_THRESHOLD_ACCEL_ENTER 3.0 ///< Threshold (m/s^2) to activate a movement.
#define VAR_THRESHOLD_ACCEL_EXIT  1.5 ///< Threshold (m/s^2) to reset a movement.
#define VAR_THRESHOLD_ACCEL_DOMINANCE 1.5 ///< How much (m/s^2) the dominant axis must exceed the others.
#define VAR_MAX_MOVEMENT_DURATION_MS 4000UL ///< Maximum duration (ms) of a movement.
#define VAR_MIN_MOVEMENT_DURATION_MS 200UL ///< Minimum duration (ms) of a movement.
#define VAR_OPPOSITE_COOLDOWN_MS 350UL ///< Time (ms) the opposite of an ended movement stays suppressed.

#define VAR_THRESHOLD_TOUCH 15

#define VAR_N_VIBRACALLS 4

#define VAR_HAPTIC_FEEDBACK_DURATION_MS 120UL ///< Haptic feedback duration (ms).

using namespace Touch;
using namespace Bluetooth;
using namespace Movement;

namespace Core {

    /**
     * @class HaptFlelt
     * @brief Manages the belt subsystems.
     */
    class HaptFlelt {

        private:
            unsigned short vibracalls_pins[4] = {25, 26, 27, 32}; ///< GPIO ports connected to vibracalls.

            Ilumination::RGBLED led; ///< RGB LED.
            Sound::Buzzer buzzer; ///< Buzzer.
            Bluetooth::BluetoothManager bluetooth; ///< Bluetooth communication.
            Vibration::VibrationSystem vibrationSystem; ///< Vibracalls.
            Movement::MovementSystem movementSystem; ///< Movement detection.
            Touch::TouchSystem touchSystem; ///< Touch buttons.
            Core::SecretCommandSystem secretCommandSystem; ///< Secret commands.

            movement_t last_move = movement_t::NONE; ///< Last movement sent.
            touch_t last_touch = touch_t::NONE; ///< Last touch sent.

            /**
             * @brief Executes a secret command.
             * @param command Secret command to be executed.
             */
            void handleSecretCommand(secret_command_t command);

            /**
             * @brief Detects the current touch and sends it to the PC.
             */
            void process_touch();

            /**
             * @brief Detects the current movement and sends it to the PC.
             */
            void process_movement();

        public:

            /**
             * @brief Default constructor.
             */
            HaptFlelt() :
                led(VAR_PIN_RED, VAR_PIN_GREEN, VAR_PIN_BLUE),
                buzzer(VAR_PIN_BUZZER),
                bluetooth("HaptFlelt Device"),
                vibrationSystem(vibracalls_pins, VAR_N_VIBRACALLS),
                movementSystem(VAR_THRESHOLD_ACCEL_ENTER, VAR_THRESHOLD_ACCEL_EXIT, VAR_THRESHOLD_ACCEL_DOMINANCE, VAR_MAX_MOVEMENT_DURATION_MS, VAR_MIN_MOVEMENT_DURATION_MS, VAR_OPPOSITE_COOLDOWN_MS),
                touchSystem(VAR_TOUCH_RIGHT, VAR_TOUCH_LEFT, VAR_TOUCH_FRONT_RIGHT, VAR_TOUCH_FRONT_LEFT, VAR_THRESHOLD_TOUCH)
            {}

            /**
             * @brief Initializes all subsystems.
             */
            void initSystem();

            /**
             * @brief Runs one cycle of the main loop.
             */
            void process();
    };

}

#endif
