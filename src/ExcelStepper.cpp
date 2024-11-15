#include "ExcelStepper.h"

ExcelStepper::ExcelStepper(byte stepPin, byte dirPin, uint16_t minSpeed) : _stepPin(stepPin), _dirPin(dirPin) {
    _minSpeed = minSpeed;
    _maxStepDuration = 1000000 / _minSpeed;
}

void ExcelStepper::begin() {
    _stepPin.begin();
    _dirPin.begin();
    setDirection(CLOCKWISE);

    _pulseState = STEP;
    _lastStepTime = micros();
    _setSpeed(_minSpeed);
    _stepsRemaining = 0;
    _acceleration = 0;
    _isTargetFullStop = true;
}

void ExcelStepper::setDirection(Direction direction) {
    if (direction == CLOCKWISE) {
        _dirPin.write(HIGH);
    } else {
        _dirPin.write(LOW);
    }
}

void ExcelStepper::accelerate(uint16_t targetSpeed, uint32_t steps) {
    _setTargetSpeed(targetSpeed);
    _stepsRemaining = steps;
    _acceleration = (static_cast<float>((static_cast<int32_t>(_targetSpeed) - static_cast<int32_t>(_currentSpeed))) /
                     static_cast<float>(_stepsRemaining)) *
                    100.0;
}

void ExcelStepper::decelerate(uint16_t targetSpeed, uint32_t steps) { accelerate(targetSpeed, steps); }

void ExcelStepper::move(uint32_t steps) {
    _stepsRemaining = steps;
    _acceleration = 0;
}

void ExcelStepper::jumpToSpeed(uint16_t speed) {
    _stepsRemaining = 0;
    _acceleration = 0;
    _setTargetSpeed(speed);
    _setSpeed(speed);
}

bool ExcelStepper::hasReachedTarget() { return _stepsRemaining == 0; }

bool ExcelStepper::run() {
    if (_currentSpeed == _minSpeed && _isTargetFullStop) return true;

    unsigned long currentTime = micros();
    unsigned long timeElapsed = currentTime - _lastStepTime;

    if (_pulseState == STEP && timeElapsed >= _stepPulseDuration) {
        _stepPin.write(LOW);
        _pulseState = DELAY;
    }

    else if (_pulseState == DELAY && timeElapsed >= _stepDuration) {
        _stepPin.write(HIGH);
        _pulseState = STEP;
        _lastStepTime = currentTime;

        if (!hasReachedTarget()) {
            _stepsRemaining--;
            if (_acceleration != 0) {
                _setSpeed(_targetSpeed - (_acceleration * static_cast<int32_t>(_stepsRemaining)) / 100);
            }
        }
    }

    return hasReachedTarget();
}

void ExcelStepper::runToTarget() {
    while (!run()) {
    }
}

uint16_t ExcelStepper::currentSpeed() { return _currentSpeed; }

void ExcelStepper::_setSpeed(uint16_t speed) {
    if (speed == _currentSpeed) return;

    if (speed <= _minSpeed) {
        _currentSpeed = _minSpeed;
        _stepDuration = _maxStepDuration;
    } else {
        _currentSpeed = speed;
        _stepDuration = 1000000 / speed;
    }

    _stepPulseDuration = _stepDuration / 2;
}

void ExcelStepper::_setTargetSpeed(uint16_t targetSpeed) {
    if (targetSpeed < _minSpeed) {
        _targetSpeed = _minSpeed;
        _isTargetFullStop = true;
    } else {
        _targetSpeed = targetSpeed;
        _isTargetFullStop = false;
    }
}
