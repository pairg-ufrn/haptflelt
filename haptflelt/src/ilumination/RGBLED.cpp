#include "RGBLED.hpp"

namespace Ilumination {

    void RGBLED::turnOnRed() {
        this->RED = 255;
        this->GREEN = 0;
        this->BLUE = 0;

        this->writeRGBValues();
    }

    void RGBLED::turnOnGreen() {
        this->RED = 0;
        this->GREEN = 255;
        this->BLUE = 0;

        this->writeRGBValues();
    }

    void RGBLED::turnOnBlue() {
        this->RED = 0;
        this->GREEN = 0;
        this->BLUE = 255;

        this->writeRGBValues();
    }

    void RGBLED::turnOnCustom(uint8_t _RED, uint8_t _GREEN, uint8_t _BLUE) {
        this->RED = _RED;
        this->GREEN = _GREEN;
        this->BLUE = _BLUE;

        this->writeRGBValues();
    }

    void RGBLED::turnOff() {
        this->RED = 0;
        this->GREEN = 0;
        this->BLUE = 0;

        this->writeRGBValues();
    }

    bool RGBLED::isOn() {
        return this->RED > 0 || this->GREEN > 0 || this->BLUE > 0;
    }

    void RGBLED::writeRGBValues() {
        analogWrite(this->RED_PIN, this->RED);
        analogWrite(this->GREEN_PIN, this->GREEN);
        analogWrite(this->BLUE_PIN, this->BLUE);
    }
}

