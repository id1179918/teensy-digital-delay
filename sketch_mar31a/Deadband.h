#pragma once

#include "VRFilter.h"

// Deadband is used as a threshold filter, letting the actual control level value change only if there is a significant change in voltage readings.
// This avoids low changing but high frequency noise from constantly triggering audio updates.

struct Deadband : VoltageReadingFilter {
    float threshold;
    float last{ -1.f };   // -1 forces first update through

    explicit Deadband(float threshold = 0.005f) : threshold(threshold) {}

    float process(float in) override {
        if (fabsf(in - last) >= threshold) last = in;
        return last;
    }
    void reset(float val) override { last = val; }
};