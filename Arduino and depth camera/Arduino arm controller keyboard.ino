#include <Servo.h>

// Define servo motor objects
Servo baseMotor;    // Continuous rotating base motor (Motor 1)
Servo joint1;       // Joint 1 (Motor 2)
Servo joint1Support; // Support servo for Joint 1 (Motor 5)
Servo joint2;       // Joint 2 (Motor 3)

Servo joint3;       // Joint 3 (Motor 4)

// Define initial speeds/positions
int baseSpeed = 85;    // Neutral speed for continuous servo (stationary)
int joint1Angle = 90;  // Starting angle for joint 1
int joint1SupportAngle = 90; // Starting angle for joint 1 support
int joint2Angle = 90;  // Starting angle for joint 2
int joint3Angle = 90;  // Starting angle for joint 3

void setup() {
  // Attach the servo motors to the appropriate pins
  baseMotor.attach(9);  // Base motor to pin 9 (continuous servo)
  joint1.attach(5);    // Joint 1 to pin 10
  joint1Support.attach(6); // Joint 1 Support to pin 13
  joint2.attach(11);    // Joint 2 to pin 11
  joint3.attach(12);    // Joint 3 to pin 12
  
  // Start serial communication
  Serial.begin(9600);
  Serial.println("Mechanical Arm Control Initialized");
  Serial.println("Use the following keys to control the arm:");
  Serial.println("'w' - Rotate Base Clockwise");
  Serial.println("'s' - Rotate Base Counterclockwise");
  Serial.println("'a' - Rotate Joint 1 Up");
  Serial.println("'d' - Rotate Joint 1 Down");
  Serial.println("'r' - Rotate Joint 2 Up");
  Serial.println("'f' - Rotate Joint 2 Down");
  Serial.println("'t' - Rotate Joint 3 Up");
  Serial.println("'g' - Rotate Joint 3 Down");
  Serial.println("'e' - Stop Base Rotation");
}

void loop() {
  // Check if data is available from the serial monitor
  if (Serial.available()) {
    char command = Serial.read();  // Read the input command
    
    // Perform the corresponding action based on the command
    switch(command) {
      case 'w':  // Rotate base clockwise (Increase speed)
        baseSpeed = 180;  // Full speed clockwise
        baseMotor.write(baseSpeed);
        Serial.println("Base rotating clockwise");
        break;
        
      case 's':  // Rotate base counterclockwise (Decrease speed)
        baseSpeed = 0;    // Full speed counterclockwise
        baseMotor.write(baseSpeed);
        Serial.println("Base rotating counterclockwise");
        break;
        
      case 'e':  // Stop base rotation (Neutral position)
        baseSpeed = 90;   // Neutral position (stop)
        baseMotor.write(baseSpeed);
        Serial.println("Base stopped");
        break;
        
      case 'a':  // Rotate joint 1 upwards
        joint1Angle += 5;
        if (joint1Angle > 180) joint1Angle = 180;
        joint1.write(joint1Angle);
        
        // Adjust the supporting servo in the opposite direction
        joint1SupportAngle = 180 - joint1Angle;
        joint1Support.write(joint1SupportAngle);
        
        Serial.print("Joint 1 rotated to: ");
        Serial.println(joint1Angle);
        Serial.print("Joint 1 support rotated to: ");
        Serial.println(joint1SupportAngle);
        break;
        
      case 'd':  // Rotate joint 1 downwards
        joint1Angle -= 5;
        if (joint1Angle < 0) joint1Angle = 0;
        joint1.write(joint1Angle);
        
        // Adjust the supporting servo in the opposite direction
        joint1SupportAngle = 180 - joint1Angle;
        joint1Support.write(joint1SupportAngle);
        
        Serial.print("Joint 1 rotated to: ");
        Serial.println(joint1Angle);
        Serial.print("Joint 1 support rotated to: ");
        Serial.println(joint1SupportAngle);
        break;
        
      case 'r':  // Rotate joint 2 upwards
        joint2Angle += 5;
        if (joint2Angle > 180) joint2Angle = 180;
        joint2.write(joint2Angle);
        Serial.print("Joint 2 rotated to: ");
        Serial.println(joint2Angle);
        break;
        
      case 'f':  // Rotate joint 2 downwards
        joint2Angle -= 5;
        if (joint2Angle < 0) joint2Angle = 0;
        joint2.write(joint2Angle);
        Serial.print("Joint 2 rotated to: ");
        Serial.println(joint2Angle);
        break;
        
      case 't':  // Rotate joint 3 upwards
        joint3Angle += 5;
        if (joint3Angle > 180) joint3Angle = 180;
        joint3.write(joint3Angle);
        Serial.print("Joint 3 rotated to: ");
        Serial.println(joint3Angle);
        break;
        
      case 'g':  // Rotate joint 3 downwards
        joint3Angle -= 5;
        if (joint3Angle < 0) joint3Angle = 0;
        joint3.write(joint3Angle);
        Serial.print("Joint 3 rotated to: ");
        Serial.println(joint3Angle);
        break;
        
      default:
        Serial.println("Invalid input. Use 'w', 's', 'a', 'd', 'r', 'f', 't', 'g', or 'e'.");
    }
  }
}




