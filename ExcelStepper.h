#ifndef ExcelStepper_h
#define ExcelStepper_h

#include <Arduino.h>

/// @brief A class to accelerate stepper motors to desired speeds within a given number of steps.
///
/// @attention This class can only be used with stepper drivers with 2 control pins (step and direction).
class ExcelStepper {
   public:
    /// @brief An enum representing the possible directions of a stepper motor.
    enum Direction { CLOCKWISE, COUNTERCLOCKWISE };

    /// @brief The minimum speed the motor can reach before it stops in steps per second.
    ///
    /// @attention This must not be 0.
    static const byte DEFAULT_MIN_SPEED = 100;

    /// @brief The constructor.
    ///
    /// @attention minSpeed must not be 0.
    ///
    /// @param stepPin: The pin number of the step pin.
    /// @param dirPin: The pin number of the direction pin.
    /// @param minSpeed: The minimum speed the motor can reach before it stops in steps per second. Defaults to
    /// DEFAULT_MIN_SPEED.
    ExcelStepper(byte stepPin, byte dirPin, byte minSpeed = DEFAULT_MIN_SPEED);

    /// @brief Sets up the stepper motor.
    ///
    /// @attention Only call this once before any other functions.
    void begin();

    /// @brief Sets the direction of the stepper motor.
    ///
    /// @param direction: The direction of the stepper motor.
    void setDirection(Direction direction);

    /// @brief Accelerates the stepper motor to the target speed within the given number of steps.
    ///
    /// run() will return true when the motor has reached the target position.
    ///
    /// @attention This only sets the correct parameters. Call run() as often as possible to actually move the motor.
    /// @attention Calling run() after the target position has been reached will continue to move the motor at the
    /// target speed.
    /// @attention Calling this before a previous target was reached will cancel that previous target and accelerate the
    /// motor from the speed that was reached.
    ///
    /// @param targetSpeed: The speed to accelerate to in steps per second.
    /// @param steps: The number of steps to accelerate to the target speed within.
    void accelerate(uint16_t targetSpeed, uint32_t steps);

    /// @brief Decelerates the stepper motor to the target speed within the given number of steps.
    ///
    /// The exactly same as accelerate() to improve the readability of the user's code.
    ///
    /// run() will return true when the motor has reached the target position.
    ///
    /// @attention This only sets the correct parameters. Call run() as often as possible to actually move the motor.
    /// @attention Calling run() after the target position has been reached will continue to move the motor at the
    /// target speed.
    /// @attention Calling this before a previous target was reached will cancel that previous target and decelerate the
    /// motor from the speed that was reached.
    ///
    /// @param targetSpeed: The speed to decelerate to in steps per second.
    /// @param steps: The number of steps to decelerate to the target speed within.
    void decelerate(uint16_t targetSpeed, uint32_t steps);

    /// @brief Moves the motor at the current speed for the given number of steps.
    ///
    /// run() will return true when the motor has reached the target position.
    ///
    /// @attention This only sets the correct parameters. Call run() as often as possible to actually move the motor.
    /// @attention Calling run() after the target position has been reached will continue to move the motor.
    /// @attention Calling this before a previous target was reached will cancel that previous target and move the
    /// motor at the speed that was reached.
    ///
    /// @param steps: The number of steps to move the motor.
    void move(uint32_t steps);

    /// @brief Sets the speed of the motor to the given speed.
    ///
    /// run() will always return true.
    ///
    /// @attention This only sets the correct parameters. Call run() as often as possible to actually move the motor.
    /// @attention Calling this before a previous target was reached will cancel that previous target and move the
    /// motor at the given speed.
    ///
    /// @param speed: The speed to set the motor to in steps per second.
    void jumpToSpeed(uint16_t speed);

    /// @brief Returns true if the motor has reached the target position defined by move(), accelerate() or
    /// decelerate().
    bool hasReachedTarget();

    /// @brief Drives the stepper motor.
    ///
    /// Returns true if the motor has reached the target position defined by move(), accelerate() or decelerate().
    ///
    /// @attention Call this function as often as possible to actually move the motor.
    bool run();

    /// @brief Drives the stepper motor until the target position defined by move(), accelerate() or decelerate() is
    /// reached.
    ///
    /// @attention This function will block the program until the target position is reached.
    void runToTarget();

    /// @brief The current speed of the motor in steps per second.
    uint16_t currentSpeed;

   private:
    /// An enum representing the state of a pulse.
    enum _EXCEL_STEPPER_PULSE_STATE { STEP, DELAY };

    /// The pin number of the step pin.
    byte _stepPin;

    /// The pin number of the direction pin.
    byte _dirPin;

    /// The minimum speed the motor can reach before it stops in steps per second.
    byte _minSpeed;

    /// The current state of the pulse.
    _EXCEL_STEPPER_PULSE_STATE _pulseState;

    /// The time of the last step in microseconds.
    uint32_t _lastStepTime;

    /// The current delay between steps in microseconds.
    uint16_t _stepDuration;

    /// The maximum step duration in microseconds.
    ///
    /// This determines the minimum speed the motor can reach before it stops.
    ///
    /// This is just a precalculated value of 1000000 / _minSpeed.
    uint16_t _maxStepDuration;

    /// The duration of the step pulse in microseconds.
    ///
    /// This is just a precalculated value of _stepDuration / 2.
    uint16_t _stepPulseDuration;

    /// The target speed of the motor in steps per second.
    uint16_t _targetSpeed;

    /// The number of steps remaining to reach the target position.
    uint32_t _stepsRemaining;

    /// The amount to increase the speed by each step in steps per second.
    int32_t _acceleration;

    /// Sets _currentSpeed and _stepDuration to the given speed.
    void _setSpeed(uint16_t speed);

    /// Writes a digital value to a pin as fast as possible.
    ///
    /// This no longer checks if PWM is enabled on the pin.
    void _fastDigitalWrite(byte pin, bool value);
};

#endif