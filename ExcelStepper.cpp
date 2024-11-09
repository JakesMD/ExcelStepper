#include "ExcelStepper.h"

ExcelStepper::ExcelStepper(byte stepPin, byte dirPin, byte minSpeed) {
    _stepPin = stepPin;
    _dirPin = dirPin;
    _minSpeed = minSpeed;
    _maxStepDuration = 1000000 / _minSpeed;
}

void ExcelStepper::begin() {
    pinMode(_stepPin, OUTPUT);
    pinMode(_dirPin, OUTPUT);
    setDirection(CLOCKWISE);

    _pulseState = STEP;
    _lastStepTime = micros();
    _setSpeed(0);
    _stepsRemaining = 0;
    _acceleration = 0;
}

void ExcelStepper::setDirection(Direction direction) {
    if (direction == CLOCKWISE) {
        digitalWrite(_dirPin, HIGH);
    } else {
        digitalWrite(_dirPin, LOW);
    }
}

void ExcelStepper::accelerate(uint16_t targetSpeed, uint32_t steps) {
    _targetSpeed = targetSpeed;
    _stepsRemaining = steps;
    _acceleration = (static_cast<int32_t>(_targetSpeed - currentSpeed) * 100) / _stepsRemaining;
}

void ExcelStepper::decelerate(uint16_t targetSpeed, uint32_t steps) { accelerate(targetSpeed, steps); }

void ExcelStepper::move(uint32_t steps) {
    _stepsRemaining = steps;
    _acceleration = 0;
    _targetSpeed = currentSpeed;
}

void ExcelStepper::jumpToSpeed(uint16_t speed) {
    _stepsRemaining = 0;
    _acceleration = 0;
    _targetSpeed = speed;
    _setSpeed(speed);
}

bool ExcelStepper::hasReachedTarget() { return _stepsRemaining == 0; }

bool ExcelStepper::run() {
    if (currentSpeed == 0 && _targetSpeed == 0) {
        _stepsRemaining = 0;
        return true;
    }

    uint32_t currentTime = micros();
    uint32_t timeElapsed = currentTime - _lastStepTime;

    if (_pulseState == STEP && timeElapsed >= _stepPulseDuration) {
        _fastDigitalWrite(_stepPin, LOW);
        _pulseState = DELAY;
    }

    else if (_pulseState == DELAY && timeElapsed >= _stepDuration) {
        _fastDigitalWrite(_stepPin, HIGH);
        _pulseState = STEP;
        _lastStepTime = currentTime;

        if (!hasReachedTarget()) {
            _stepsRemaining--;
            _setSpeed(_targetSpeed - (static_cast<int32_t>(_acceleration) * _stepsRemaining) / 100);
        }
    }

    return hasReachedTarget();
}

void ExcelStepper::runToTarget() {
    while (!run()) {
    }
}

void ExcelStepper::_setSpeed(uint16_t speed) {
    if (speed == currentSpeed) return;

    currentSpeed = speed;
    _stepDuration = (speed > _minSpeed) ? 1000000 / speed : _maxStepDuration;
    _stepPulseDuration = _stepDuration / 2;
}

void ExcelStepper::_fastDigitalWrite(byte pin, bool val) {
    byte bit = digitalPinToBitMask(pin);
    byte port = digitalPinToPort(pin);
    volatile byte *out;

    if (port == NOT_A_PIN) return;

    out = portOutputRegister(port);

    byte oldSREG = SREG;
    cli();

    if (val == LOW) {
        *out &= ~bit;
    } else {
        *out |= bit;
    }

    SREG = oldSREG;
}
