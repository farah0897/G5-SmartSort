#include <Servo.h>

// Define number of joints/servos
#define NUM_JOINTS 5

// Servo objects and configurations
Servo servos[NUM_JOINTS];
const int servo_pins[NUM_JOINTS] = {8, 9, 10, 11, 12}; // Adjust pins as needed
float current_positions[NUM_JOINTS] = {90, 90, 90, 90, 90}; // Starting positions

// Servo constraints
const float MIN_ANGLE = 0.0;
const float MAX_ANGLE = 180.0;

void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    Serial.setTimeout(1);
    
    // Initialize all servos
    for(int i = 0; i < NUM_JOINTS; i++) {
        servos[i].attach(servo_pins[i]);
        servos[i].write(current_positions[i]);
    }
}

// Validate angle is within constraints
bool isValidAngle(float angle) {
    return angle >= MIN_ANGLE && angle <= MAX_ANGLE;
}

void loop() {
    if(Serial.available()) {
        String command = Serial.readStringUntil('\n');
        
        // Process command based on first character
        char cmd = command.charAt(0);
        
        switch(cmd) {
            case 'r': // Read current positions
                // Send current positions as comma-separated values
                for(int i = 0; i < NUM_JOINTS; i++) {
                    Serial.print(current_positions[i]);
                    if(i < NUM_JOINTS - 1) {
                        Serial.print(",");
                    }
                }
                Serial.println();
                break;
                
            case 'w': // Write new positions
                // Expected format: "w90,90,90,90,90"
                command = command.substring(1); // Remove the 'w'
                int index = 0;
                int lastIndex = 0;
                
                // Parse comma-separated values
                while(lastIndex < command.length() && index < NUM_JOINTS) {
                    int nextComma = command.indexOf(',', lastIndex);
                    if(nextComma == -1) {
                        nextComma = command.length();
                    }
                    
                    String valueStr = command.substring(lastIndex, nextComma);
                    float newPosition = valueStr.toFloat();
                    
                    // Validate and update servo position
                    if (isValidAngle(newPosition)) {
                        servos[index].write(newPosition);
                        current_positions[index] = newPosition;
                    } else {
                        Serial.print("Invalid angle for joint ");
                        Serial.print(index);
                        Serial.println(". Skipping.");
                    }
                    
                    lastIndex = nextComma + 1;
                    index++;
                }
                break;
                
            default:
                Serial.println("Unknown command");
                break;
        }
    }
    delay(1); // Small delay to prevent overwhelming the serial port
} 