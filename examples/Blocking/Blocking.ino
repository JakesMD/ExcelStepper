/*

    This example demonstrates how to use the ExcelStepper library to move a stepper motor in a blocking way.

    This program is blocking because it waits for the motor to reach its target position before continuing to the next
    step. This means that the program will not be able to do anything else while the motor is moving.

*/

#include <ExcelStepper.h>

ExcelStepper stepper(2, 5);  // Create a new stepper motor on pins 2 and 5.

void setup() {
    stepper.begin();  // Set up the stepper motor.
}

void loop() {
    // Accelerate to 800 steps per second in 500 steps.
    stepper.accelerate(800, 500);
    stepper.runToTarget();

    // Move 500 steps at 800 steps per second.
    stepper.move(500);
    stepper.runToTarget();

    // Accelerate to 1600 steps per second in 500 steps.
    stepper.accelerate(1600, 500);
    stepper.runToTarget();

    // Move 500 steps at 800 steps per second.
    stepper.move(500);
    stepper.runToTarget();

    // Decelerate to 0 steps per second in 1000 steps.
    stepper.decelerate(0, 1000);
    stepper.runToTarget();

    delay(1000);
}
