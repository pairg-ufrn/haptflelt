#include "Buzzer.hpp"

namespace Sound {

    void Buzzer::turnOn() {
        this->on = true;
        this->writeBuzzerValue();
    }

    void Buzzer::turnOff() {
        this->on = false;
        this->writeBuzzerValue();
    }

    bool Buzzer::isOn() {
        return this->on;
    }

    void Buzzer::whistle(uint32_t milliseconds) {
        if (this->isOn()) this->turnOff();

        this->turnOn();
        delay(milliseconds);
        this->turnOff();
    }

    void Buzzer::writeBuzzerValue() {
        if (this->on == true) digitalWrite(this->PIN, HIGH);
        else digitalWrite(this->PIN, LOW);
    }

}