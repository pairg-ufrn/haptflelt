#include "Vibracall.hpp"

namespace Vibration {

    void Vibracall::turnOn() {
        digitalWrite(this->PIN, HIGH); 
    }

    void Vibracall::turnOff() {
        digitalWrite(this->PIN, LOW); 
    }

    bool Vibracall::isOn() {
        return this->on;
    }

    void Vibracall::vibrate(uint32_t milliseconds) {
        if(this->isOn()) this->turnOff();

        this->turnOn();
        delay(milliseconds);
        this->turnOff();
    }

}
