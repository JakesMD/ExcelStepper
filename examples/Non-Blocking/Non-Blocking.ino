/*

    This example demonstrates how to use the ExcelStepper library to move a stepper motor in a non-blocking way.

    This program is non-blocking because it does not wait for the motor to reach. This means that the program can do
    other things while the motor is moving.

*/

#include <ExcelStepper.h>

ExcelStepper stepper(2, 5);  // Create a new stepper motor on pins 2 and 5.

void setup() {
    stepper.begin();  // Set up the stepper motor.
}

void loop() {
    // Accelerate to 800 steps per second in 500 steps.
    stepper.accelerate(800, 500);
    while (!stepper.run()) {
        // Do whatever you want while the motor is moving.
    }

    // Move 500 steps at 800 steps per second.
    stepper.move(500);
    while (!stepper.run()) {
        // Do whatever you want while the motor is moving.
    }

    // Decelerate to 0 steps per second in 500 steps.
    stepper.decelerate(0, 500);
    while (!stepper.run()) {
        // Do whatever you want while the motor is moving.
    }
}
