#pragma once
#include <Windows.h>
#include <math.h>
#include "Helpers.h"

class Camera
{
public:
  static void AddPosition(float x, float y, float z);
	static void AddLookAtPosition(float x, float y, float z);
	static void LookAt();
	static void LookAt(float eyeX, float eyeY, float eyeZ, float lookAtX, float lookAtY, float lookAtZ);

  static float GetAngle() {
    return ANGLE;
  }

  static float GetNear() {
    return Z_NEAR;
  }

  static float GetFar() {
    return Z_FAR;
  }

private:
	Camera();
	~Camera();

  static float EYE[3];
  static float SIGHT[3];
  static float UP[3];
  static float ANGLE;
  static float Z_NEAR;
  static float Z_FAR;

};