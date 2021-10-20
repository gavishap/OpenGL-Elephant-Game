#pragma once
#include <Windows.h>
#include <math.h>
#include "Helpers.h"

//**********************************************************************************
// class Furniture
//**********************************************************************************
class Furniture
{
private:
  float m_Pos[3];

public:
  Furniture(float x, float y, float z);
  ~Furniture();

  void Draw();
};

//**********************************************************************************
// class Lamp
//**********************************************************************************
class Lamp
{
private:
  float m_Pos[3] = { 0.f , 0.f , 0.f };
  float m_Direction[3] = { -20.f , 0.f, -20.f };
  float m_intensity = 0.35f;
  float m_color[3] = { 0.f , 1.f, 1.f };

public:
  Lamp();
  void Draw(float x, float y, float z);
  void AddLightColor(float r, float g, float b);
  void ApplyLighting();
  void SetLightDirection(float x, float y, float z);
  void SetIntensity(float diff);
};


//**********************************************************************************
// class Room
//**********************************************************************************
class Room
{
private:
  float m_width;
  float m_height;
  GLuint m_textureWall;
  GLuint m_textureFloor;
  GLuint m_textureCeiling;

public:
  Room(float width, float height);
  void Draw();

private:
  void DrawWall();
  void DrawCeiling();
  void DrawFloor();
};

//**********************************************************************************
// class ElephantLeg
//**********************************************************************************
class ElephantLeg
{
public:
  float m_upperLen = 0.f;		// length of upper leg
  float m_lowerLen = 0.f;		// length of lower leg
  float m_upperWidth = 0.f;		// width of upper leg
  float m_lowerWidth = 0.f;		// width of lower leg
  float m_upperAngle = -90;		// Upper Angle between torso and leg
  float m_lowerAngle = 25;		// Lower angle between upper leg and lower leg
  float m_upperThickness = 20.f;	// thickness of upper leg
  float m_lowerThickness = 20.f;	// thickness of lower leg
  float m_feetAngle = 90;		// feet angle with lower leg
  float m_feetLen = 20.f;			// length of feet
  float m_feetThickness = 5.f;	// thickness of feet
  float m_feetWidth = 5.f;		// width of feet

	ElephantLeg();
  ElephantLeg(int index, float bodyLength, float bodyHeight, float bodyWidth);

  void Draw();
};


//**********************************************************************************
// class Elephant
//**********************************************************************************
class Elephant
{
private:
	float m_baseLength = 30.f;	// Size of elephant
	float m_bodyWidth = 30.f;	// base parameter to calculate other body parts
	
  float m_elephantPos[3] = { 0.f, 0.f, 0.f };	// custom x position of elephant in scene
  float m_RotationAngle = 0.f;

	float m_neckAngleZ = 0.f;	// custom z neck angle used while moving head
	float m_neckAngleY = 0.f; // custom y neck angle used while moving head

	float m_tailAngleY = 0.f; // custom y tail angle used while moving tail
	float m_tailAngleZ = 0.f; // custom z tail angle used while moving tail

	GLfloat m_eyesPosition[4]; // camera position
	GLfloat m_eyesLookAt[4];	// look at position
	float m_viewInvMatrix[16]; //Inverted matrix of the view transformation

public:	
	float m_bodyHeight;
	ElephantLeg m_legs[4];

public:
	Elephant(int baseLength);
	void Draw();
	void SetPosition(float x, float y, float z);
  void AddPosition(float x, float y, float z);
  void AddRotation(float angle);
	void SetTailAngle(int yDiff, int zDiff);
	void SetNeckAngle(int yDiff, int zDiff);
	void GetEyesPosition(GLfloat oEyePos[4]);
	void GetEyesLookAt(GLfloat oEyeLookAt[4]);
	void SaveViewMatrix();

private:	
	void ResetParameters(int base);
	void DrawTorso();
	void DrawFace();
	void DrawTail();
	void DrawLegs();
	void CalcEyesPosition();
};