#pragma once
#include <Windows.h>
#include <math.h>
#include <string>
#include <iostream>

#include "Helpers.h"
#include "Camera.h"
#include "Objects.h"

using namespace std;

class Scene
{
public:
	Scene(int argc, char** argv);
	~Scene();

	// Basic handlers needed for glut library
	// Main draw function
	void Draw();

	// Window resize function
	void Resize(int width, int height);

	// callbacks for key press
	void OnKeyPress(unsigned char key, int x, int y);

	// callbacks for special key press like left/right/up/down etc
	void OnSpecialKeyPress(unsigned char key, int x, int y);

	// Hanlder for menu
	void OnMenuClick(int value);

private:

  // Setup menu
	void InitMenu();

  // Setup lights
	void InitLight();

  // Help for current command control
	void DrawKeysControlText();

  // Utility functions for handling different command callbacks
  // To manage camera sight 
	void HandleCameraLookAt(unsigned char key);

  // To manage camera position 
	void HandleCameraPosition(unsigned char key);

  // To manage elephant's tail
	void HandleMoveTail(unsigned char key);

  // To manage elephant's head
	void HandleMoveHead(unsigned char key);

  // To manage lamp (spot-light) position
	void HandleSpotlightPosition(unsigned char key);

  // To manage lamp (spot-light) color
	void HandleSpotlightColor(unsigned char key);

  // To manage lamp (spot-light) direction
	void HandleSpotlightDirection(unsigned char key);

  // To manage lamp (spot-light) intensity
  void HandleSpotlightIntensity(unsigned char key);

  // To manage global light intensity
	void HandleLightGlobalIntensity(unsigned char key);

  // To manage global light color
  void HandleGlobalLightColor(unsigned char key);

  // To manage elephant position + rotation
  void HandleElephantView(unsigned char key);

  // To setup default light intensity
	void SetLightGlobalIntensity(float diff);

  // To setup lamp position in scene
	void SetLampPosition(float x, float y, float z);

  // data members
  Elephant* m_pElephant;
  Room* m_pRoom;
  Lamp* m_pLamp;
  Furniture* m_pFurniture;

  enum KeysMap {
    DEFAULT = -1,
    EXIT,
    CAMERA_POSITION,
    CAMERA_LOOKAT,
    CAMERA_ELEPHANTVIEW,
    LAMP_POSITION,
    LAMP_DIRECTION,
    LAMP_INTENSITY,
    LAMP_COLOR,
    GLOBAL_LIGHT_INTENSITY,
    GLOBAL_LIGHT_COLOR,
    MOVE_ELEPHANT,
    MOVE_TAIL,
    MOVE_HEAD,
    HELP
  };
  
  KeysMap m_CurKeyMap = KeysMap::DEFAULT;

  float m_lightGlobalIntensity = 0.8f;
  bool m_bElephantEyesView = false;
  float m_LightPos[3] = { -20.f , 60.f, -20.f };
  float m_GlobalLightColor[3] = { 1.f, 1.f, 1.f };

};

