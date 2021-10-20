#pragma once
#include <Windows.h>
#include "GL/glut.h"
#include "GL/GL.h"
#include "GL/GLU.h"
#include <string>

using namespace std;

const float PI = 3.1415926535897932384626433832795f;
const GLfloat g_ColorBlack[] = { 0.f, 0.f, 0.f };
const GLfloat g_ColorWhite[] = { 1.f, 1.f, 1.f };
const GLfloat g_ColorGlass[] = { 0.93f, 0.93f, 0.93f, 0.2f};
const GLfloat g_ColorYellow[] = { 1.f, 1.f, 0.f };
const GLfloat g_ColorPurple[] = { 0.5, 0, 0.5 };
const GLfloat g_ColorDarkBlue[] = { 0.06f, 0.11f, 0.21f };

const GLfloat g_ColorLeg[] = { 0.2f, 0.2f, 0.2f, 1.f };
const GLfloat g_ColorTail[] = { 0.15f, 0.15f, 0.15f , 1.f };
const GLfloat g_ColorTorso[] = { 0.4f, 0.4f, 0.4f, 1.f };
const GLfloat g_ColorNeck[] = { 0.3f, 0.3f, 0.3f, 1.f };
const GLfloat g_ColorHead[] = { 0.25f, 0.25f, 0.25f, 1.f };
const GLfloat g_ColorEar[] = { 0.35f, 0.35f, 0.35f, 1.f };
const GLfloat g_ColorEye[] = { 0.05f, 0.05f, 0.05f, 1.f };
const GLfloat g_ColorMouth[] = { 0.3176f, 0.f, 0.f, 1.f };
const GLfloat g_ColorNose[] = { 0.15f, 0.15f, 0.15f, 1.f };
const GLfloat g_ColorTeeth[] = { 0.95f, 0.95f, 0.95f, 1.f };

const GLfloat g_ColorTableLeg[] = { 0.26666f, 0.1372f, 0.f, 1.f };

const GLfloat g_ColorFloorPlate1[] = { 0.5f, 0.4f, 0.3f, 1.f };
const GLfloat g_ColorFloorPlate2[] = { 0.5f , 0.6f, 0.7f, 1.f };

//Gold:
const GLfloat g_ColorTeapotAmbient[] = { 0.24725f, 0.1995f, 0.0745f };
const GLfloat g_ColorTeapotDiffuse[] = { 0.75164f, 0.60648f, 0.22648f };
const GLfloat g_ColorTeapotSpecular[] = { 0.628281f, 0.555802f, 0.366065f };

class Helpers
{
public:
  // public common helpers
  static float ToRadians(float degrees);

  // Helpers for drawing geometries
  static void DrawCube(float length, float height , float width );
  static void DrawSphere(float radius, int slices, int stacks);
  static void DrawEllipsoid(float fA, float fB, float fC, unsigned int uiSlices, unsigned int uiStacks);
  static void DrawCylinder(GLdouble baseRadius, GLdouble length);

  // Helpers for drawing text
  static void DrawText(string text, unsigned int x, unsigned int y);

  // Helpers for textures
  static void LoadTexture(GLuint texture, const char* filePath);

  // Helpers for Matrix
  static void GetModelViewMatrix(float oMat[16]);
  static void MultiplyMatrixVector(float oRes[4], float* matrix, float* vector);
  static void MultiplyMatrixMatrix(float oRes[16], float* matrix1, float* matrix2);
  static bool GetInvertMatrix(const float m[16], float invOut[16]);

  // Helpers related to Window
  static void AdjustWindow(string windowTitle);

private:
  Helpers();
  ~Helpers();
};