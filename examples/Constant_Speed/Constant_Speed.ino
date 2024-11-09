/*

    This example demonstrates how to use the ExcelStepper library to accelerate a stepper motor to a constant speed.


*/

#include <ExcelStepper.h>

ExcelStepper stepper(2, 5);  // Create a new stepper motor on pins 2 and 5.

void setup() {
    stepper.begin();                 // Set up the stepper motor.
    stepper.accelerate(1000, 1000);  // Accelerate to 1000 steps per second in 1000 steps.
}

void loop() {
    // Move the motor.
    //
    // One the motor has reached the target speed, it will continue to move at that speed.
    stepper.run();
}
