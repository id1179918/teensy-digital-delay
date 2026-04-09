#pragma once

#include <Arduino.h>
#include <array>
#include <functional>
#include <cmath>

struct VoltageReadingFilter {
    virtual float process(float in) = 0;
    virtual void  reset(float val) = 0;
    virtual ~VoltageReadingFilter() = default;
};