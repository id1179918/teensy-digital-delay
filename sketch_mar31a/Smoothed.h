#pragma once

#include "Deadband.h"

using Callback = std::function<void(float)>;

class FilteredInput {
public:

    FilteredInput(uint8_t pin,
        Filter* filter,
        Callback cb,
        int resolution = 1023)
        : _pin(pin)
        , _filter(filter)
        , _cb(std::move(cb))
        , _resolution(static_cast<float>(resolution))
    {
    }

    ~FilteredInput() {
        delete _filter;
    }

    void update() {
        float level = analogRead(_pin) / _resolution;

        level = _filter->process(level);

        if (level != _lastEmitted) {
            _lastEmitted = level;
            if (_cb) _cb(level);
        }
    }

    float value()  const { return _lastEmitted; }
    uint8_t pin()  const { return _pin; }

    void reset() {
        float val = analogRead(_pin) / _resolution;
        for (auto* f : _filters) { f->reset(val); }
        _lastEmitted = val;
    }

private:
    uint8_t                  _pin;
    std::array<Filter*, N>   _filters;
    Callback                 _cb;
    float                    _resolution;
    float                    _lastEmitted{ -1.f };
};