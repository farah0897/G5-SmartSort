#include <Servo.h>

// Define servo motor objects
Servo baseMotor;    // Rotating the base (Motor 1)
Servo joint1;       // Joint 1 (Motor 2)
Servo joint2;       // Joint 2 (Motor 3)
Servo joint3;       // Joint 3 (Motor 4)

// Define initial angles
int baseAngle = 90;    // Starting angle for base motor
int joint1Angle = 90;  // Starting angle for joint 1
int joint2Angle = 90;  // Starting angle for joint 2
int joint3Angle = 90;  // Starting angle for joint 3

// Joystick input pins
const int joystickXPin = A0;  // X-axis for base motor
const int joystickYPin = A1;  // Y-axis for joint motors

// Dead zone threshold
const int deadZone = 50;  // Adjust this value to set the dead zone size

// Step size for incremental movement
const int stepSize = 1;  // Angle step size for smoother control

void setup() {
  // Attach the servo motors to the appropriate pins
  baseMotor.attach(9);   // Base motor to pin 9
  joint1.attach(10);     // Joint 1 to pin 10
  joint2.attach(11);     // Joint 2 to pin 11
  joint3.attach(12);     // Joint 3 to pin 12
  
  // Start serial communication for debugging
  Serial.begin(9600);
  Serial.println("Joystick-controlled Mechanical Arm with Dead Zone and Incremental Control Initialized");
}

void loop() {
  // Read joystick positions (0-1023)
  int xVal = analogRead(joystickXPin);
  int yVal = analogRead(joystickYPin);

  // Center value for joystick (approx. 512)
  int centerX = 512;
  int centerY = 512;

  // Apply dead zone logic and map movement incrementally for X-axis (base motor)
  if (abs(xVal - centerX) > deadZone) {
    if (xVal > centerX) {
      // Move clockwise if joystick is pushed to the right
      baseAngle += stepSize;
      if (baseAngle > 180) baseAngle = 180;  // Constrain to max angle
    } else {
      // Move counterclockwise if joystick is pushed to the left
      baseAngle -= stepSize;
      if (baseAngle < 0) baseAngle = 0;  // Constrain to min angle
    }
    baseMotor.write(baseAngle);
    Serial.print("Base Motor Angle: ");
    Serial.println(baseAngle);
  }

  // Apply dead zone logic and map movement incrementally for Y-axis (joint motors)
  if (abs(yVal - centerY) > deadZone) {
    if (yVal > centerY) {
      // Move joints up if joystick is pushed forward
      joint1Angle += stepSize;
      joint2Angle += stepSize;
      joint3Angle += stepSize;
      if (joint1Angle > 180) joint1Angle = 180;  // Constrain to max angle
      if (joint2Angle > 180) joint2Angle = 180;
      if (joint3Angle > 180) joint3Angle = 180;
    } else {
      // Move joints down if joystick is pulled backward
      joint1Angle -= stepSize;
      joint2Angle -= stepSize;
      joint3Angle -= stepSize;
      if (joint1Angle < 0) joint1Angle = 0;  // Constrain to min angle
      if (joint2Angle < 0) joint2Angle = 0;
      if (joint3Angle < 0) joint3Angle = 0;
    }
    joint1.write(joint1Angle);
    joint2.write(joint2Angle);
    joint3.write(joint3Angle);

    Serial.print("Joint 1, 2, 3 Angle: ");
    Serial.println(joint1Angle);
  }

  // Add a short delay for stability and smoother control
  delay(50);
}




