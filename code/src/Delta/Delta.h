#ifndef DELTA_H
#define DELTA_H

#include <Arduino.h>
#include <Wire.h>
#include "config/config.h"
#include "MatrixController.h"
#include "ServoGroup.h"

class Delta {
 public:
  enum class State { Idle, Moving, Sleep, Initializing }; 
  State currentState;

  Delta();
  void init();
  void update();
  void setAngles(float angle1, float angle2, float angle3, int duration);
  void setPose(float heightPerc, float rollAngle, float pitchAngle, int duration);

  Matrix matrix;
  ServoGroup servoGroup;

  struct Position {
    float heightPerc;
    float rollPerc;
    float pitchPerc; 
  };
  
 private:
  void renderPoseVisual(float heightPerc, float rollPerc, float pitchPerc);
  Position currentPos;
  
  // Timer for auto-detach functionality
  unsigned long startTime;
  bool servosDetached;
  static const unsigned long DETACH_DELAY = 20000; // 20 seconds
};

#endif