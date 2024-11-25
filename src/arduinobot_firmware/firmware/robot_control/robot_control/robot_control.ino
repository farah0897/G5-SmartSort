#include <Servo.h>

#define SERVO_BASE_PLATE_PIN 8
#define SERVO_ARM_1_LEFT_PIN 9
#define SERVO_ARM_1_RIGHT_PIN 10
#define SERVO_ARM_2_PIN 11
#define SERVO_GRIPPER_INNER_CIRCLE_PIN 12

#define BASE_PLATE_START 90
#define ARM_1_START 90
#define ARM_2_START 90
#define GRIPPER_START 90

#define STEP_ANGLE 15  // Smaller step angle for finer control
#define MIN_ANGLE 0
#define MAX_ANGLE 180

Servo base_plate;
Servo arm_1_left;
Servo arm_1_right;
Servo arm_2;
Servo inner_circle; // Controls the gripper

void reach_goal(Servo &motor, int goal) {
    if (goal >= motor.read()) {
        for (int pos = motor.read(); pos <= goal; pos++) {
            motor.write(pos);
            delay(5);
        }
    } else {
        for (int pos = motor.read(); pos >= goal; pos--) {
            motor.write(pos);
            delay(5);
        }
    }
}

void moveServo(Servo &motor, int direction) {
    int current = motor.read();
    int target = current + (direction * STEP_ANGLE);
    
    if (target > MAX_ANGLE) target = MAX_ANGLE;
    if (target < MIN_ANGLE) target = MIN_ANGLE;
    
    reach_goal(motor, target);
}

// Move two motors synchronously (e.g., arm_1_left and arm_1_right)
void moveDualServo(Servo &motor1, Servo &motor2, int direction) {
    int current1 = motor1.read();
    int current2 = motor2.read();
    int target1 = current1 + (direction * STEP_ANGLE);
    int target2 = current2 + (direction * STEP_ANGLE);
    
    if (target1 > MAX_ANGLE) target1 = MAX_ANGLE;
    if (target1 < MIN_ANGLE) target1 = MIN_ANGLE;
    if (target2 > MAX_ANGLE) target2 = MAX_ANGLE;
    if (target2 < MIN_ANGLE) target2 = MIN_ANGLE;
    
    reach_goal(motor1, target1);
    reach_goal(motor2, target2);
}

void setup() {
    base_plate.attach(SERVO_BASE_PLATE_PIN);
    arm_1_left.attach(SERVO_ARM_1_LEFT_PIN);
    arm_1_right.attach(SERVO_ARM_1_RIGHT_PIN);
    arm_2.attach(SERVO_ARM_2_PIN);
    inner_circle.attach(SERVO_GRIPPER_INNER_CIRCLE_PIN);

    base_plate.write(BASE_PLATE_START);
    arm_1_left.write(ARM_1_START);
    arm_1_right.write(ARM_1_START);
    arm_2.write(ARM_2_START);
    inner_circle.write(GRIPPER_START);

    Serial.begin(115200);
    Serial.setTimeout(1);
}

void loop() {
    if (Serial.available()) {
        char chr = Serial.read();

        // Base plate rotation
        if (chr == 'B') {
            moveServo(base_plate, 1); // Rotate base plate clockwise
        } else if (chr == 'b') {
            moveServo(base_plate, -1); // Rotate base plate counterclockwise
        }

        // Arm_1 controls (dual motors)
        else if (chr == 'U') {
            moveDualServo(arm_1_left, arm_1_right, 1); // Move arm_1 up
        } else if (chr == 'D') {
            moveDualServo(arm_1_left, arm_1_right, -1); // Move arm_1 down
        }

        // Arm_2 controls (single motor)
        else if (chr == 'u') {
            moveServo(arm_2, 1); // Move arm_2 up
        } else if (chr == 'd') {
            moveServo(arm_2, -1); // Move arm_2 down
        }

        // Gripper controls
        else if (chr == 'O') {
            moveServo(inner_circle, 1); // Open gripper (clockwise)
        } else if (chr == 'C') {
            moveServo(inner_circle, -1); // Close gripper (counterclockwise)
        }

        // Reset to home position
        else if (chr == 'H') {
            base_plate.write(BASE_PLATE_START);
            arm_1_left.write(ARM_1_START);
            arm_1_right.write(ARM_1_START);
            arm_2.write(ARM_2_START);
            inner_circle.write(GRIPPER_START);
        }
    }
}
