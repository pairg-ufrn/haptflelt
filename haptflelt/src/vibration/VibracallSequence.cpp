#include "VibracallSequence.hpp"

namespace Vibration {

    void VibracallSequence::turnOn(unsigned short _index) {
        this->sequence[_index].turnOn();
    }

    void VibracallSequence::turnOn(const unsigned short* _indexes, std::size_t _size) {
        if (_size > this->sequence.size()) return;
        for(auto i(0); i < _size; i++) this->turnOn(_indexes[i]);
    }

    void VibracallSequence::turnOff(unsigned short _index) {
        this->sequence[_index].turnOff();
    }

    void VibracallSequence::turnOff(const unsigned short* _indexes, std::size_t _size) {
        if (_size > this->sequence.size()) return;
        for(auto i(0); i < _size; i++) this->turnOff(_indexes[i]);
    }

    bool VibracallSequence::isOn(unsigned short _index) {
        return this->sequence[_index].isOn();
    }

    void VibracallSequence::vibrate(unsigned short _index, uint32_t milliseconds) {
        this->sequence[_index].vibrate(milliseconds);
    }

    void VibracallSequence::vibrate(unsigned short* _indexes, std::size_t _size, uint32_t milliseconds) {
        if (_size > this->sequence.size()) return;
        for(auto i(0); i < _size; i++) this->vibrate(_indexes[i], milliseconds);
    }

}