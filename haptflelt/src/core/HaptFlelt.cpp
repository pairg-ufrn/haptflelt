#include "HaptFlelt.hpp"

namespace Core {

    void HaptFlelt::handleSecretCommand(secret_command_t command) {
        switch (command) {
            case secret_command_t::CALIBRATE_ZERO: {
                for (auto i = 0; i < 2; i++) {
                    buzzer.whistle(80);
                    led.turnOnBlue();
                    delay(80);
                    led.turnOff();
                    delay(80);
                }

                movementSystem.recalibrate();
                break;
            }
            default: break;
        }
    }

    void HaptFlelt::process_touch() {
        auto touch = touchSystem.detectTouch();

        auto secret = secretCommandSystem.detect(touch);

        if (secret != secret_command_t::NONE) {
            // Only fires on the transition, not while the buttons stay pressed.
            if (this->last_touch != touch) {
                this->handleSecretCommand(secret);
                bluetooth.send(sent_command_t::NONE);
            }

            this->last_touch = touch;
            return;
        }

        auto touch_command = CommandMapper::toCommand(touch);

        // NONE is sent only once, on the transition. Active states are sent
        // continuously, so the PC doesn't miss them if a message is lost.
        if (touch == touch_t::NONE) {
            if (this->last_touch != touch_t::NONE) bluetooth.send(sent_command_t::NONE);
        } else {
            bluetooth.send(touch_command);
        }

        this->last_touch = touch;
    }

    void HaptFlelt::process_movement() {
        auto move = movementSystem.detectMovement();

        auto movement_command = CommandMapper::toCommand(move);

        // NONE is sent only once, on the transition. Active states are sent
        // continuously, so the PC doesn't miss them if a message is lost.
        if (move == movement_t::NONE) {
            if (this->last_move != movement_t::NONE) bluetooth.send(sent_command_t::NONE);
        } else {
            bluetooth.send(movement_command);
        }

        this->last_move = move;
    }

    void HaptFlelt::initSystem() {
        delay(200);
        
        Serial.begin(115200);
        
        bool error = false;

        bluetooth.initSystem();
        if (!bluetooth.isConnected()) error = true;
        
        if (!movementSystem.initSystem()) error = true;
        
        if (error) {
            
            for (auto i(0); i < 2; i++) {
                buzzer.whistle(100);
                delay(100);
            }

            for (auto i(0); i < 5; i++) {
                led.turnOnRed();
                delay(800);
                led.turnOff();
                delay(200);
            }
            
            ESP.restart();
        } else {
            led.turnOnBlue();
            buzzer.whistle(100);
            delay(100);
            led.turnOff();
        }
    }

    void HaptFlelt::process() {

        if (!bluetooth.isConnected()) {
            delay(1000);
            Serial.println("Closed");
            bluetooth.initSystem();
        } else {

            feedback_direction_t feedback = bluetooth.receive();
            
            if (feedback != feedback_direction_t::NONE) 
                vibrationSystem.hapticFeedback(feedback, VAR_HAPTIC_FEEDBACK_DURATION_MS);
            
            this->process_touch();
            this->process_movement();
        }
    
        delay(50);
    }

}
