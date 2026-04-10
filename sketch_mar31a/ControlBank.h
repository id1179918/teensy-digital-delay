#pragma once

#include "Smoothed.h"

template<size_t N>
class ControlBank {
public:
    // Takes ownership of raw pointers
    explicit ControlBank(std::array<AnalogInput*, N> inputs)
        : _inputs(inputs) {
    }

    ~ControlBank() {
        for (auto* p : _inputs) delete p;
    }

    void update() {
        for (auto* p : _inputs) p->update();
    }

    AnalogInput& operator[](size_t i) { return *_inputs[i]; }

private:
    std::array<AnalogInput*, N> _inputs;
};