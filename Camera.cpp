#include "Camera.h"

float Camera::ANGLE = 90.0;
float Camera::Z_NEAR = 5.0;
float Camera::Z_FAR = 205.0;
float Camera::EYE[3] = { 50.f, 30.f, 40.f};
float Camera::SIGHT[3] = { 0.f, 20.f, 0.f};
float Camera::UP[3] = { 0.f, 1.f, 0.f };

//**********************************************************************************
// Camera::Camera
//**********************************************************************************
Camera::Camera()
{
}

//**********************************************************************************
// Camera::~Camera
//**********************************************************************************
Camera::~Camera()
{
}

//**********************************************************************************
// Camera::AddPosition
//**********************************************************************************
void Camera::AddPosition(float x, float y, float z) {
  Camera::EYE[0] += x;
  Camera::EYE[1] += y;
  Camera::EYE[2] += z;
}

//**********************************************************************************
// Camera::AddLookAtPosition
//**********************************************************************************
void Camera::AddLookAtPosition(float x, float y, float z) {
  Camera::SIGHT[0] += x;
  Camera::SIGHT[1] += y;
  Camera::SIGHT[2] += z;
}

//**********************************************************************************
// Camera::LookAt
//**********************************************************************************
void Camera::LookAt() {
  // Look at with current camera parameters
  gluLookAt(
    Camera::EYE[0], Camera::EYE[1], Camera::EYE[2],
    Camera::SIGHT[0], Camera::SIGHT[1], Camera::SIGHT[2],
    Camera::UP[0], Camera::UP[1], Camera::UP[2]
  );
}

//**********************************************************************************
// Camera::LookAt
//**********************************************************************************
void Camera::LookAt(float eyeX, float eyeY, float eyeZ, float lookAtX, float lookAtY, float lookAtZ) {

  // Look a with user given parameters
  // Used in elephant view
  gluLookAt(
    eyeX, eyeY, eyeZ,
    lookAtX, lookAtY, lookAtZ,
    Camera::UP[0], Camera::UP[1], Camera::UP[2]
  );
}