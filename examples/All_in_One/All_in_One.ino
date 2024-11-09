/*

    This example demonstrates all the features of the ExcelStepper library.

*/

#include <ExcelStepper.h>

ExcelStepper stepper(2, 5);  // Create a new stepper motor on pins 2 and 5.

void setup() {
    Serial.begin(9600);

    stepper.begin();                          // Set up the stepper motor.
    stepper.setDirection(COUNTER_CLOCKWISE);  // Set the direction of the motor.
}

void loop() {
    stepper.accelerate(800, 500);  // Accelerates from 0 to 800 steps per second in 500 steps.
    stepper.runToTarget();         // Waits for the motor to reach 500 steps.

    stepper.move(500);  // Moves 500 steps at 800 steps per second.

    while (!run()) {                           // Loops until the motor has reached the target position.
        Serial.println(stepper.currentSpeed);  // Large computations like serial prints may slow down the motor.
    }

    // Accelerate from 800 to 1600 steps per second in 500 steps.
    stepper.accelerate(1600, 500);
    stepper.runToTarget();

    // Jump from 1600 to 800 steps per second.
    stepper.jumpToSpeed(800, 500);
    stepper.move(500);
    stepper.runToTarget();

    // Decelerate to 0 steps per second in 1000 steps.
    stepper.decelerate(0, 1000);
    stepper.runToTarget();

    delay(1000);
}