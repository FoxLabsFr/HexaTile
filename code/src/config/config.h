#ifndef CONFIG_H
#define CONFIG_H

#define SERVOGROUP_DISABLE_JSON
#define MATRIX_DISABLE_JSON
#define MATRIX_DISABLE_BLENDING 

// LED Matrix configuration
#define LED_PIN 6
#define LED_COUNT 144

// Servo configuration
#define S1_PIN 7
#define S2_PIN 5
#define S3_PIN 3

// Servo parameters
#define SERVO_MIN_PULSE 500
#define SERVO_MAX_PULSE 2500
#define SERVO_MIN_ANGLE 1000
#define SERVO_MAX_ANGLE 1750

// Delta Maximum roll/pitch angle in degrees
#define DELTA_RANGE_ANGLE 30.0 

#endif // CONFIG_H 