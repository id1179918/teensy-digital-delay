#pragma once

#include "VRFilter.h"
#include "Deadband.h"

using Callback = std::function<void(float)>;

class AnalogInput {
public:

    AnalogInput(uint8_t pin,
        VoltageReadingFilter* filter,
        Callback cb,
        int resolution = 1023)
        : _pin(pin)
        , _filter(filter)
        , _callback(std::move(cb))
        , _resolution(static_cast<float>(resolution))
    {
    }

    ~AnalogInput() {
        delete _filter;
    }

    void update() {
        float level = analogRead(_pin) / _resolution;

        level = _filter->process(level);

        if (level != _lastEmitted) {
            _lastEmitted = level;
            if (_callback) _callback(level);
        }
    }

    float getLastEmittedValue()  const { return _lastEmitted; }
    uint8_t getPin()  const { return _pin; }

    void reset() {
        float val = analogRead(_pin) / _resolution;
        _filter->reset(val);
        _lastEmitted = val;
    }

private:
    uint8_t                  _pin;
    VoltageReadingFilter*   _filter;
    Callback                 _callback;
    float                    _resolution;
    float                    _lastEmitted{ -1.f };
};

inline AnalogInput* makePot(uint8_t pin,
    std::function<void(float)> callback,
    float deadbandThreshold = 0.005f,
    int   resolution = 1023)
{
    return new AnalogInput(
        pin,
        new Deadband(deadbandThreshold),
        std::move(callback),
        resolution
    );
}