#include "config/config.h"
#include "Delta/Delta.h"

Delta delta;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting HexaTilt...");
  delta.init();
}

const unsigned long cycleDuration = 5000; // 5 seconds per cycle

void loop() {
  unsigned long currentTime = millis();
  static unsigned long startTime = 0;  
  if (startTime == 0) startTime = currentTime;
  
  // Only move if not in sleep mode
  if (delta.currentState != Delta::State::Sleep) {
    unsigned long elapsedTime = currentTime - startTime;
    float cycleProgress = (float)(elapsedTime % cycleDuration) / cycleDuration;
    
    // Calculate motion parameters
    float heightPerc = 0.3 + 0.4 * sin(cycleProgress * 2 * PI);
    float rollAngle = 20 * sin(cycleProgress * 4 * PI);
    float pitchAngle = 20 * cos(cycleProgress * 3 * PI);
    
    // Update pose every 100ms
    static unsigned long lastPoseUpdate = 0;
    if (currentTime - lastPoseUpdate > 100) { 
      lastPoseUpdate = currentTime;
      delta.setPose(heightPerc, rollAngle, pitchAngle, 100);
    }
    
    // Print status every 2 seconds
    static unsigned long lastPrint = 0;
    if (currentTime - lastPrint > 2000) {
      lastPrint = currentTime;
      Serial.print("Height: "); Serial.print(heightPerc * 100, 1); Serial.print("% ");
      Serial.print("Roll: "); Serial.print(rollAngle, 1); Serial.print("° ");
      Serial.print("Pitch: "); Serial.print(pitchAngle, 1); Serial.println("°");
    }
  }
  
  delta.update();
}