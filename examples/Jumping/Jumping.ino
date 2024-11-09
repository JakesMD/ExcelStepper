/*

    This example demonstrates how to use the ExcelStepper library to jump between motor speeds.

*/

#include <ExcelStepper.h>

ExcelStepper stepper(2, 5);  // Create a new stepper motor on pins 2 and 5.

void setup() {
    stepper.begin();  // Set up the stepper motor.
}

void loop() {
    stepper.jumpToSpeed(800);  // Set the speed to 800 steps per second.
    stepper.move(500);         // Move 500 steps at 800 steps per second.
    stepper.runToTarget();     // Wait for the motor to reach its target position.

    stepper.jumpToSpeed(400);  // Set the speed to 400 steps per second.
    stepper.move(500);         // Move 500 steps at 400 steps per second.
    stepper.runToTarget();     // Wait for the motor to reach its target position.
}
