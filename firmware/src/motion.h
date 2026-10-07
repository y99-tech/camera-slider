#pragma once
#include <Arduino.h>

enum class Mode : uint8_t { Idle, Homing, Move, RunAB, Sequence, Timelapse };

void   motionInit();
void   motionLoop();

// all positions in mm, 0 = motor end (after homing)
String motionHome();
String motionZero();                         // "here is 0" (no endstops)
String motionGoto(float mm, float speed = 0); // speed 0 = max speed
String motionJog(float deltaMm);
String motionTimedMove(float mm, float secs, float easePct);
String motionRunAB(float secs, float easePct, int legs);   // legs: 1 = A->B, 2 = A->B->A, 0 = forever
String motionSequence(int loops);                          // keyframes, ping-pong, 0 = forever
String motionTimelapse(int shots, float intervalS, int settleMs, int shutterMs);
String motionSnap(int dir);                  // stop-motion: step + shutter
void   motionStop();                         // decelerate and cancel any job
void   motionEStop();                        // immediate stop
void   motionSetMotor(bool on);              // off = hand mode (free carriage / crank)
void   motionApplySettings();
void   shutterPulse(int ms);

float  motionPos();
bool   motionIsHomed();
bool   motionIsBusy();
bool   motionMotorOn();
Mode   motionMode();
String motionStatusJson();
