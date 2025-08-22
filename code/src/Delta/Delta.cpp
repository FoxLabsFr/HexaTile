#include "Delta.h"

Delta::Delta()
    : currentState(State::Idle),
      matrix(),
      servoGroup(),
      startTime(0),
      servosDetached(false) {}

void Delta::init() {
  currentState = State::Initializing;
  Serial.println("Delta init...");
   
  // Configure ServoGroup
  servoGroup.setIds("pwm",{S1_PIN, S2_PIN, S3_PIN});
  servoGroup.setDefaultPosition({SERVO_MAX_ANGLE, SERVO_MAX_ANGLE, SERVO_MAX_ANGLE}); 
  servoGroup.setMinPulse({SERVO_MIN_PULSE, SERVO_MIN_PULSE, SERVO_MIN_PULSE});
  servoGroup.setMaxPulse({SERVO_MAX_PULSE, SERVO_MAX_PULSE, SERVO_MAX_PULSE});
  servoGroup.setOffsets({0, 0, 0});
  servoGroup.setInverts({1, 1, 1});
  servoGroup.init();
  
  // Initialize Matrix
  matrix.init(MatrixTypes::HEXAGONAL, LED_PIN);

  // set initial pose and color
  matrix.setColor(0, 0, 5);
  currentPos = {0.5f, 0.0f, 0.0f};
  Serial.println("Delta robot initialized");  

  delay(5000);
  currentState = State::Moving;
}


void Delta::update() {
  // Handle automatic detach timer
  if (currentState != State::Sleep) {
    unsigned long currentTime = millis();
    
    if (startTime == 0) {
      startTime = currentTime;
    }
    
    if (currentTime - startTime >= DETACH_DELAY) {
      currentState = State::Sleep;
      servoGroup.detachAll();
      Serial.println("20 seconds elapsed - entering sleep mode");
    }
  }
  
  // update motor positions
  servoGroup.update();
  // update LED matrix
  renderPoseVisual(currentPos.heightPerc, currentPos.rollPerc, currentPos.pitchPerc);
}


void Delta::setAngles(float angle1, float angle2, float angle3, int duration) {
  // Angles in degrees * 10 format, for servoGroup format
  int16_t angles[] = {(int16_t)angle1, (int16_t)angle2, (int16_t)angle3};
  servoGroup.setPositions(angles, duration);
}

void Delta::setPose(float heightPerc, float rollAngle, float pitchAngle, int duration) {
  short minHeight = SERVO_MIN_ANGLE;
  short maxHeight = SERVO_MAX_ANGLE; 
  
  float baseAngle = minHeight + (maxHeight - minHeight) * heightPerc;
  float rollRad = rollAngle * PI / 180.0;
  float pitchRad = pitchAngle * PI / 180.0;
  
  // Calculate servo offsets: S1 (front), S2 (right), S3 (left)
  float rollOffset = sin(rollRad) * 400.0;
  float pitchOffset = sin(pitchRad) * 400.0;
  
  float angle1 = baseAngle + pitchOffset;
  float angle2 = baseAngle - (pitchOffset * 0.5) + rollOffset;
  float angle3 = baseAngle - (pitchOffset * 0.5) - rollOffset;
  
  angle1 = constrain(angle1, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
  angle2 = constrain(angle2, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
  angle3 = constrain(angle3, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
  
  setAngles(angle1, angle2, angle3, duration);
  
  // Update LED matrix state
  float rollPerc = (rollAngle / DELTA_RANGE_ANGLE) * 100.0;
  float pitchPerc = (pitchAngle / DELTA_RANGE_ANGLE) * 100.0;
  rollPerc = constrain(rollPerc, -100.0, 100.0);
  pitchPerc = constrain(pitchPerc, -100.0, 100.0);
  
  currentPos.heightPerc = heightPerc;
  currentPos.rollPerc = -rollPerc;
  currentPos.pitchPerc = pitchPerc;
}

// Renders pose visualization on LED matrix (circle for height + square for orientation)
void Delta::renderPoseVisual(float heightPerc, float rollPerc, float pitchPerc) {
  float currentHeightPerc = heightPerc;
  float currentRollPerc = rollPerc;
  float currentPitchPerc = pitchPerc;

  // Clear all pixels first
  matrix.clear();

  const int W = matrix.getWidth();
  const int H = matrix.getHeight();
  float centerX = (float(W) - 1.0f) / 2.0f;
  float centerY = (float(H) - 1.0f) / 2.0f;

  float circleRadius = 1.0f + (currentHeightPerc * 4.5f);

  // Only set the pixels that should be lit (circle)
  for (int x = 0; x < W; x++) {
    for (int y = 0; y < H; y++) {
      float dx = float(x) - centerX;
      float dy = centerY - float(y);
      float distance = sqrtf(dx * dx + dy * dy);
      if (distance <= circleRadius && distance >= circleRadius - 1.0f) {
        uint8_t intensity = uint8_t(currentHeightPerc * 20.0f + 5.0f);
        matrix.setPixelXY(x, y, 0, 0, intensity); // Blue circle
      }
    }
  }

  // Draw the 2x2 red square (this will overwrite circle pixels if they overlap)
  float squareX = centerX + (currentPitchPerc * 0.03f) + 0.5f;
  float squareY = centerY - (currentRollPerc * 0.03f) + 0.5f;
  squareX = constrain(squareX, 0.5f, 13.5f);
  squareY = constrain(squareY, 0.5f, 13.5f);

  for (int sx = 0; sx < 2; sx++) {
    for (int sy = 0; sy < 2; sy++) {
      int px = int(squareX - 0.5f + sx);
      int py = int(squareY - 0.5f + sy);
      matrix.setPixelXY(px, py, 15, 0, 0); // Red square
    }
  }
  
  // Now show all pixels at once
  matrix.show();
}