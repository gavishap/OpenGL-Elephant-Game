#include "Scene.h"

/*
Stuff to bind glut handler with current scene object
*/
Scene* g_CurrentInstance;
extern "C"
void drawCallback()
{
	g_CurrentInstance->Draw();
}
void menuCallback(int value)
{
	g_CurrentInstance->OnMenuClick(value);
}
void reshapeCallback(int width, int height)
{
	g_CurrentInstance->Resize(width, height);
}
void onKeyPressCallback(unsigned char key, int x, int y)
{
	g_CurrentInstance->OnKeyPress(key, x, y);
}
void onSpecialKeyPressCallback(int key, int x, int y)
{
	g_CurrentInstance->OnSpecialKeyPress(key, x, y);
}

// Initial dimension of application
int g_WIDTH = 640;
int g_HEIGHT = 480;

float g_ROOM_WIDTH = 160;
float g_ROOM_HEIGHT = 120;
int g_ELEPHANT_SIZE = 20;
string g_WINDOW_TITLE = "Maman_17_Gavriel_Shapiro";

//**********************************************************************************
// Scene::Scene
//**********************************************************************************
Scene::Scene(int argc, char** argv)
{
	glutInit(&argc, argv);
	
	//Setup glut init
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	g_WIDTH = glutGet(GLUT_SCREEN_WIDTH);
	g_HEIGHT = glutGet(GLUT_SCREEN_HEIGHT);
	glutInitWindowSize(g_WIDTH, g_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(g_WINDOW_TITLE.c_str());

	//Enable depth testing when rendering objects (z-buffer)
	glEnable(GL_DEPTH_TEST);

	//Set perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	float aspect = (float)w / h;
	gluPerspective(Camera::GetAngle(), aspect, Camera::GetNear(), Camera::GetFar());

	// Create elephant object
	m_pElephant = new Elephant(g_ELEPHANT_SIZE);
	m_pElephant->SetPosition(-10, m_pElephant->m_legs[0].m_upperLen + m_pElephant->m_legs[0].m_lowerLen + m_pElephant->m_bodyHeight / 2.0f - 2, 10);

	//Create room, lamp and furniture objects
	m_pRoom = new Room(g_ROOM_WIDTH, g_ROOM_HEIGHT);
	m_pLamp = new Lamp();
	m_pFurniture = new Furniture(40, 0, -40);

	//Set glut callbacks
	::g_CurrentInstance = this;
	glutDisplayFunc(::drawCallback);
	glutReshapeFunc(::reshapeCallback);
	glutKeyboardFunc(::onKeyPressCallback);
	glutSpecialFunc(::onSpecialKeyPressCallback);

	Helpers::AdjustWindow(g_WINDOW_TITLE);

	// start render loop
	glutMainLoop();
}

//**********************************************************************************
// Scene::!Scene
//**********************************************************************************
Scene::~Scene()
{
	// garbage collection
	delete m_pElephant;
	delete m_pRoom;
	delete m_pLamp;
	delete m_pFurniture;
}

//**********************************************************************************
// Scene::Draw
//**********************************************************************************
void Scene::Draw() {
	//Reset colors and model view matrix
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Set camera position
	if (m_bElephantEyesView) {
		GLfloat eyesPos[4], eyesLookat[4];
		m_pElephant->GetEyesPosition(eyesPos);
		m_pElephant->GetEyesLookAt(eyesLookat);
		// customized look at with respect to elephant view
		Camera::LookAt(eyesPos[0], eyesPos[1], eyesPos[2], eyesLookat[0], eyesLookat[1], eyesLookat[2]);
	}
	else
	{
		Camera::LookAt();
	}
	
	//Save the view matrix to be used later for eye position calculation
	m_pElephant->SaveViewMatrix();

	//Lighting
	InitLight();

	//Draw lamp and set its light
	m_pLamp->Draw(m_LightPos[0], m_LightPos[1], m_LightPos[2]);
	m_pLamp->ApplyLighting();

	//Draw room
	glPushMatrix();
	glTranslatef((GLfloat)-g_ROOM_WIDTH / 2, 0.f, (GLfloat)-g_ROOM_WIDTH / 2);
	m_pRoom->Draw();
	glPopMatrix();

	//Draw elephant
	m_pElephant->Draw();

	//Draw furniture
	m_pFurniture->Draw();

	//Init menu while drawing since the text of the menus may change
	InitMenu();

	//Draw text with instructions for controlling
	DrawKeysControlText();
	
	glutSwapBuffers();
}

//**********************************************************************************
// Scene::InitLight
//**********************************************************************************
void Scene::InitLight()
{
	//Enable lighting
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH); //Gourad shading

	glEnable(GL_LIGHT0);
	SetLightGlobalIntensity(0);
}

//**********************************************************************************
// Scene::SetLightGlobalIntensity
//**********************************************************************************
void Scene::SetLightGlobalIntensity(float diff)
{
	m_lightGlobalIntensity += diff;
	if (m_lightGlobalIntensity > 0.99f)
		m_lightGlobalIntensity = 0.99f;
	//else if (m_lightGlobalIntensity < 0)
	//	m_lightGlobalIntensity = 0.0;

	float intensity[] = { m_GlobalLightColor[0] * m_lightGlobalIntensity, m_GlobalLightColor[1] * m_lightGlobalIntensity, m_GlobalLightColor[2] * m_lightGlobalIntensity, 1};

	glLightfv(GL_LIGHT0, GL_AMBIENT, intensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, intensity);
	glLightfv(GL_LIGHT0, GL_SPECULAR, intensity);
}

//**********************************************************************************
// Scene::Resize
//**********************************************************************************
void Scene::Resize(int width, int height) {
	g_WIDTH = width;
	g_HEIGHT = height;
	glViewport(0, 0, g_WIDTH, g_HEIGHT);
}

//**********************************************************************************
// Scene::InitMenu
//**********************************************************************************
void Scene::InitMenu() {
	int cameraMenu = glutCreateMenu(::menuCallback);
	glutAddMenuEntry("Control camera position", KeysMap::CAMERA_POSITION);
	glutAddMenuEntry("Control lookat", KeysMap::CAMERA_LOOKAT);
	if (m_bElephantEyesView)
	{
		glutAddMenuEntry("Disable elephant eyes' view", KeysMap::CAMERA_ELEPHANTVIEW);
	}
	else
	{
		glutAddMenuEntry("Elephant eyes' view", KeysMap::CAMERA_ELEPHANTVIEW);
	}

	int lightMenu = glutCreateMenu(::menuCallback);
	glutAddMenuEntry("Control lamp (spotlight) position", KeysMap::LAMP_POSITION);
	glutAddMenuEntry("Control lamp (spotlight) direction", KeysMap::LAMP_DIRECTION);
	glutAddMenuEntry("Change lamp (spotlight) intensity", KeysMap::LAMP_INTENSITY);
  glutAddMenuEntry("Change lamp (spotlight) color", KeysMap::LAMP_COLOR);
	glutAddMenuEntry("Change global light intensity", KeysMap::GLOBAL_LIGHT_INTENSITY);
  glutAddMenuEntry("Change global light color", KeysMap::GLOBAL_LIGHT_COLOR);

	int elephantMenu = glutCreateMenu(::menuCallback);
  glutAddMenuEntry("Move Elephant", KeysMap::MOVE_ELEPHANT);
	glutAddMenuEntry("Move tail", KeysMap::MOVE_TAIL);
	glutAddMenuEntry("Move head", KeysMap::MOVE_HEAD);

	int mainMenu = glutCreateMenu(::menuCallback);
	glutAddSubMenu("Camera", cameraMenu);
	glutAddSubMenu("Light", lightMenu);
	glutAddSubMenu("Elephant", elephantMenu);
	glutAddMenuEntry("Stop controlling", KeysMap::DEFAULT);
  glutAddMenuEntry("Help", KeysMap::HELP);
	glutAddMenuEntry("Quit", KeysMap::EXIT);

	// Associate to right click
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void Scene::OnMenuClick(int value) {
	m_CurKeyMap = (KeysMap)value;
	switch (m_CurKeyMap)
	{
		case KeysMap::CAMERA_ELEPHANTVIEW:
		{
			m_bElephantEyesView = !m_bElephantEyesView;
			break;
		}
		case KeysMap::EXIT:
		{
			exit(0);
		}
	}

	glutPostRedisplay();
}

//**********************************************************************************
// Scene::DrawKeysControlText
//**********************************************************************************
void Scene::DrawKeysControlText() {

	unsigned int baseX = 50;
	unsigned int baseY = g_HEIGHT - 50;
	unsigned int lineGap = 30;

	switch (m_CurKeyMap)
	{
		case KeysMap::DEFAULT:
		{
			Helpers::DrawText("Right mouse click for options menu." , baseX, baseY);
      Helpers::DrawText("Press 'h' for help", baseX, baseY - lineGap);
			break;
		}
		case KeysMap::CAMERA_POSITION:
		{
			Helpers::DrawText("Control camera position :", baseX, baseY);
			Helpers::DrawText("Use left/right keys to control X axis", baseX, baseY - lineGap);
			Helpers::DrawText("Use up/down keys to control Y axis", baseX, baseY - 2 * lineGap);
			Helpers::DrawText("Use PageUp/PageDown keys to control Z axis", baseX, baseY - 3 * lineGap);
			break;
		}
		case KeysMap::CAMERA_LOOKAT:
		{
      Helpers::DrawText("Control camera look at :", baseX, baseY);
      Helpers::DrawText("Use left/right keys to control X axis", baseX, baseY - lineGap);
      Helpers::DrawText("Use up/down keys to control Y axis", baseX, baseY - 2 * lineGap);
      Helpers::DrawText("Use PageUp/PageDown keys to control Z axis", baseX, baseY - 3 * lineGap);
			break;
		}
		case KeysMap::CAMERA_ELEPHANTVIEW:
		{
			if (m_bElephantEyesView) {
				Helpers::DrawText("Camera is positioned at elephant's eyes", baseX, baseY);
        Helpers::DrawText("Use left/right keys to control X axis", baseX, baseY - lineGap);
        Helpers::DrawText("Use up/down keys to control Y axis", baseX, baseY - 2 * lineGap);
        Helpers::DrawText("Use PageUp/PageDown keys to control Z axis", baseX, baseY - 3 * lineGap);
			}
			break;
		}

		case KeysMap::MOVE_ELEPHANT:
    {
      Helpers::DrawText("Control elephant's position :", baseX, baseY);
      Helpers::DrawText("Use left/right keys to control X axis", baseX, baseY - lineGap);
      Helpers::DrawText("Use up/down keys to control Y axis", baseX, baseY - 2 * lineGap);
      Helpers::DrawText("Use PageUp/PageDown keys to control Z axis", baseX, baseY - 3 * lineGap);
			Helpers::DrawText("Use +/- keys to rotate", baseX, baseY - 4 * lineGap);
      break;
    }
		case KeysMap::MOVE_TAIL:
		{
      Helpers::DrawText("Control elephant's tail:", baseX, baseY);
      Helpers::DrawText("Use left/right keys to move tail left and right", baseX, baseY - lineGap);
      Helpers::DrawText("Use up/down keys to move tail up and down", baseX, baseY - 2 * lineGap);
			break;
		}
		case KeysMap::MOVE_HEAD:
		{
      Helpers::DrawText("Control elephant's head:", baseX, baseY);
      Helpers::DrawText("Use left/right keys to move head left and right", baseX, baseY - lineGap);
      Helpers::DrawText("Use up/down keys to move head up and down", baseX, baseY - 2 * lineGap);
			break;
		}
		case KeysMap::LAMP_POSITION:
		{
      Helpers::DrawText("Control spotlight position :", baseX, baseY);
      Helpers::DrawText("Use left/right keys to control X axis", baseX, baseY - lineGap);
	  Helpers::DrawText("Use up/down keys to control Y axis", baseX, baseY - 2 * lineGap);
	  Helpers::DrawText("Use PageUp/PageDown keys to control Z axis", baseX, baseY - 3 * lineGap);
			break;
		}
		case KeysMap::LAMP_DIRECTION:
		{
      Helpers::DrawText("Control spotlight direction :", baseX, baseY);
      Helpers::DrawText("Use left/right keys to control X axis", baseX, baseY - lineGap);
      Helpers::DrawText("Use PageUp/PageDown keys to control Z axis", baseX, baseY - 2 * lineGap);
			break;
		}
		case KeysMap::LAMP_INTENSITY:
		{
      Helpers::DrawText("Control spotlight intensity :", baseX, baseY);
      Helpers::DrawText("Use +/- keys to control intensity", baseX, baseY - lineGap);
			break;
		}
    case KeysMap::LAMP_COLOR:
    {
      Helpers::DrawText("Control spotlight color :", baseX, baseY);
      Helpers::DrawText("Use left/right keys to control red color", baseX, baseY - lineGap);
			Helpers::DrawText("Use up/down keys to control green color", baseX, baseY - 2 * lineGap);
			Helpers::DrawText("Use PageUp/PageDown keys to control blue color", baseX, baseY - 3 * lineGap);
      break;
    }
		case KeysMap::GLOBAL_LIGHT_INTENSITY:
		{
      Helpers::DrawText("Control global light intensity :", baseX, baseY);
      Helpers::DrawText("Use +/- keys to control intensity", baseX, baseY - lineGap);
			break;
		}
    case KeysMap::GLOBAL_LIGHT_COLOR:
    {
      Helpers::DrawText("Control global light color :", baseX, baseY);
      Helpers::DrawText("Use left/right keys to control red color", baseX, baseY - lineGap);
      Helpers::DrawText("Use up/down keys to control green color", baseX, baseY - 2 * lineGap);
      Helpers::DrawText("Use PageUp/PageDown keys to control blue color", baseX, baseY - 3 * lineGap);
      break;
    }
    case KeysMap::HELP:
    {
      Helpers::DrawText("To control camera position : ", baseX, baseY);
      Helpers::DrawText("Press 'p' or Right click -> Camera -> Control camera position", baseX, baseY - lineGap);
      Helpers::DrawText("To control camera look at : ", baseX, baseY - 2 * lineGap);
      Helpers::DrawText("Press 'l' or Right click -> Camera -> Control look at", baseX, baseY - 3 * lineGap);
      Helpers::DrawText("To enable/disable elephant's eye view : ", baseX, baseY - 4 * lineGap);
      Helpers::DrawText("Press 'e' or Right click -> Camera -> Control look at", baseX, baseY - 5 * lineGap);
      Helpers::DrawText("To control lamp position : ", baseX, baseY - 6 * lineGap);
      Helpers::DrawText("Press 'n' or Right click -> Light -> Control lamp position", baseX, baseY - 7 * lineGap);
      Helpers::DrawText("To control lamp direction : ", baseX, baseY - 8 * lineGap);
      Helpers::DrawText("Press 'd' or Right click -> Light -> Control lamp direction", baseX, baseY - 9 * lineGap);
      Helpers::DrawText("To control lamp intensity : ", baseX, baseY - 10 * lineGap);
      Helpers::DrawText("Press 'i' or Right click -> Light -> Change lamp intensity", baseX, baseY - 11 * lineGap);
      Helpers::DrawText("To control lamp color : ", baseX, baseY - 12 * lineGap);
      Helpers::DrawText("Press 'c' or Right click -> Light -> Change lamp color", baseX, baseY - 13 * lineGap);
      Helpers::DrawText("To control global light intensity : ", baseX, baseY - 14 * lineGap);
      Helpers::DrawText("Press 'g' or Right click -> Light -> Change global light intensity", baseX, baseY - 15 * lineGap);
      Helpers::DrawText("To control global light color : ", baseX, baseY - 16 * lineGap);
      Helpers::DrawText("Press 'b' or Right click -> Light -> Change global light color", baseX, baseY - 17 * lineGap);
      Helpers::DrawText("To control elephant position : ", baseX, baseY - 18 * lineGap);
      Helpers::DrawText("Press 'm' or Right click -> Elephant -> Move elephant", baseX, baseY - 19 * lineGap);
      Helpers::DrawText("To control elephant tail : ", baseX, baseY - 20 * lineGap);
      Helpers::DrawText("Press 't' or Right click -> Elephant -> Move tail", baseX, baseY - 21 * lineGap);
      Helpers::DrawText("To control elephant head : ", baseX, baseY - 22 * lineGap);
      Helpers::DrawText("Press 'a' or Right click -> Elephant -> Move head", baseX, baseY - 23 * lineGap);
      Helpers::DrawText("To stop controlling camera : ", baseX, baseY - 24 * lineGap);
      Helpers::DrawText("Press 's' or Right click -> Stop Controlling", baseX, baseY - 25 * lineGap);
      break;
    }
	}
}

//**********************************************************************************
// Scene::OnKeyPress
//**********************************************************************************
void Scene::OnKeyPress(unsigned char key, int x, int y) {
	// Link keyboard shortcuts to menu items
  switch (key) {
  case 'h':
  {
		OnMenuClick(KeysMap::HELP);
    return;
  }
  case 'p':
  {
		OnMenuClick(KeysMap::CAMERA_POSITION);
    return;
  }
  case 'l':
  {
		OnMenuClick(KeysMap::CAMERA_LOOKAT);
    return;
  }
  case 'e':
  {
		OnMenuClick(KeysMap::CAMERA_ELEPHANTVIEW);
    return;
  }
  case 'n':
  {
		OnMenuClick(KeysMap::LAMP_POSITION);
    return;
  }
  case 'd':
  {
		OnMenuClick(KeysMap::LAMP_DIRECTION);
    return;
  }
  case 'i':
  {
    OnMenuClick(KeysMap::LAMP_INTENSITY);
    return;
  }
  case 'c':
  {
    OnMenuClick(KeysMap::LAMP_COLOR);
    return;
  }
  case 'g':
  {
    OnMenuClick(KeysMap::GLOBAL_LIGHT_INTENSITY);
    return;
  }
  case 'b':
  {
    OnMenuClick(KeysMap::GLOBAL_LIGHT_COLOR);
    return;
  }
  case 'm':
  {
    OnMenuClick(KeysMap::MOVE_ELEPHANT);
    return;
  }
  case 't':
  {
    OnMenuClick(KeysMap::MOVE_TAIL);
    return;
  }
  case 'a':
  {
    OnMenuClick(KeysMap::MOVE_HEAD);
    return;
  }
  case 's':
  {
    OnMenuClick(KeysMap::DEFAULT);
    return;
  }
  }

	// Need to handle +/- keys under this function and other keys under OnSpecialKeyPress
  switch (m_CurKeyMap)
  {
  case KeysMap::GLOBAL_LIGHT_INTENSITY:
    HandleLightGlobalIntensity(key);
    break;
  case KeysMap::LAMP_INTENSITY:
    HandleSpotlightIntensity(key);
    break;;
  case KeysMap::MOVE_ELEPHANT:
    HandleElephantView(key);
    break;
  }
}

//**********************************************************************************
// Scene::OnSpecialKeyPress
//**********************************************************************************
void Scene::OnSpecialKeyPress(unsigned char key, int x, int y) {

	// Control keys like Left/Right/Up/Down/PageUp/PageDown are managed in this function
	switch (m_CurKeyMap)
	{
		case KeysMap::CAMERA_LOOKAT:
			HandleCameraLookAt(key);
			break;
		case KeysMap::CAMERA_POSITION:
			HandleCameraPosition(key);
			break;
		case KeysMap::LAMP_DIRECTION:
			HandleSpotlightDirection(key);
			break;
		case KeysMap::LAMP_POSITION:
			HandleSpotlightPosition(key);
			break;
    case KeysMap::LAMP_COLOR:
      HandleSpotlightColor(key);
      break;
    case KeysMap::GLOBAL_LIGHT_COLOR:
      HandleGlobalLightColor(key);
      break;
    case KeysMap::MOVE_ELEPHANT:
			HandleElephantView(key);
      break;
		case KeysMap::MOVE_TAIL:
			HandleMoveTail(key);
			break;
		case KeysMap::MOVE_HEAD:
			HandleMoveHead(key);
			break;
    case KeysMap::CAMERA_ELEPHANTVIEW:
			if(m_bElephantEyesView)
				HandleElephantView(key);
      break;
	}
}

//**********************************************************************************
// Scene::HandleCameraLookAt
//**********************************************************************************
void Scene::HandleCameraLookAt(unsigned char key)
{
	switch (key) {
		case GLUT_KEY_RIGHT:
		{
			Camera::AddLookAtPosition(1.f, 0.f, 0.f);
			break;
		}
		case GLUT_KEY_LEFT:
		{
			Camera::AddLookAtPosition(-1.f, 0.f, 0.f);
			break;
		}
		case GLUT_KEY_UP:
		{
			Camera::AddLookAtPosition(0.f, 1.f, 0.f);
			break;
		}
		case GLUT_KEY_DOWN:
		{
			Camera::AddLookAtPosition(0.f, -1.f, 0.f);
			break;
		}
    case GLUT_KEY_PAGE_UP:
    {
      Camera::AddLookAtPosition(0.f, 0.f, -1.f);
      break;
    }
    case GLUT_KEY_PAGE_DOWN:
    {
      Camera::AddLookAtPosition(0.f, 0.f, 1.f);
      break;
    }
	}
  Camera::LookAt();
  Draw();
}

//**********************************************************************************
// Scene::HandleCameraPosition
//**********************************************************************************
void Scene::HandleCameraPosition(unsigned char key)
{
	switch (key) {
	case GLUT_KEY_RIGHT:
	{
		Camera::AddPosition(1.f, 0.f, 0.f);
		break;
	}
	case GLUT_KEY_LEFT:
	{
		Camera::AddPosition(-1.f, 0.f, 0.f);
		break;
	}
	case GLUT_KEY_UP:
	{
		Camera::AddPosition(0.f, 1.f, 0.f);
		break;
	}
	case GLUT_KEY_DOWN:
	{
		Camera::AddPosition(0.f, -1.f, 0.f);
		break;
	}
	case GLUT_KEY_PAGE_UP:
	{
		Camera::AddPosition(0.f, 0.f, -1.f);
		break;
	}
	case GLUT_KEY_PAGE_DOWN:
	{
		Camera::AddPosition(0.f, 0.f, 1.f);
		break;
	}
	}
	Camera::LookAt();
  Draw();
}

//**********************************************************************************
// Scene::HandleSpotlightPosition
//**********************************************************************************
void Scene::HandleSpotlightPosition(unsigned char key)
{
	switch (key) {
		case GLUT_KEY_RIGHT:
		{
			SetLampPosition(1.f, 0.f, 0.f);
			break;
		}
		case GLUT_KEY_LEFT:
		{
			SetLampPosition(-1.f, 0.f, 0.f);
			break;
		}
    case GLUT_KEY_UP:
    {
			SetLampPosition(0.f, 1.f, 0.f);
      break;
    }
    case GLUT_KEY_DOWN:
    {
			SetLampPosition(0.f, -1.f, 0.f);
      break;
    }
		case GLUT_KEY_PAGE_UP:
		{
			SetLampPosition(0.f, 0.f, -1.f);
			break;
		}
		case GLUT_KEY_PAGE_DOWN:
		{
			SetLampPosition(0.f, 0.f, 1.f);
			break;
		}
	}

	Draw();
}


//**********************************************************************************
// Scene::HandleSpotlightColor
//**********************************************************************************
void Scene::HandleSpotlightColor(unsigned char key)
{
  switch (key) {
  case GLUT_KEY_RIGHT:
  {
		m_pLamp->AddLightColor(0.1f, 0.f, 0.f);
    break;
  }
  case GLUT_KEY_LEFT:
  {
    m_pLamp->AddLightColor(-0.1f, 0.f, 0.f);
    break;
  }
  case GLUT_KEY_UP:
  {
    m_pLamp->AddLightColor(0.f, 0.1f, 0.f);
    break;
  }
  case GLUT_KEY_DOWN:
  {
    m_pLamp->AddLightColor(0.f, -0.1f, 0.f);
    break;
  }
  case GLUT_KEY_PAGE_UP:
  {
    m_pLamp->AddLightColor(0.f, 0.f, 0.1f);
    break;
  }
  case GLUT_KEY_PAGE_DOWN:
  {
    m_pLamp->AddLightColor(0.f, 0.f, -0.1f);
    break;
  }
  }
	Draw();
}

//**********************************************************************************
// Scene::HandleSpotlightDirection
//**********************************************************************************
void Scene::HandleSpotlightDirection(unsigned char key)
{
	switch (key) {
	case GLUT_KEY_RIGHT:
	{
		m_pLamp->SetLightDirection(1, 0.f, 0);
		break;
	}
	case GLUT_KEY_LEFT:
	{
		m_pLamp->SetLightDirection(-1, 0.f, 0);
		break;
	}
	case GLUT_KEY_PAGE_UP:
	{
		m_pLamp->SetLightDirection(0, 0.f, 1);
		break;
	}
	case GLUT_KEY_PAGE_DOWN:
	{
		m_pLamp->SetLightDirection(0, 0.f, -1);
		break;
	}
	}

	Draw();
}


//**********************************************************************************
// Scene::HandleMoveTail
//**********************************************************************************
void Scene::HandleMoveTail(unsigned char key)
{
	switch (key) {
		case GLUT_KEY_RIGHT:
		{
			m_pElephant->SetTailAngle(4, 0);
			break;
		}
		case GLUT_KEY_LEFT:
		{
			m_pElephant->SetTailAngle(-4, 0);
			break;
		}
		case GLUT_KEY_UP:
		{
			m_pElephant->SetTailAngle(0, -2);
			break;
		}
		case GLUT_KEY_DOWN:
		{
			m_pElephant->SetTailAngle(0, 2);
			break;
		}
	}

	Draw();
}

//**********************************************************************************
// Scene::HandleMoveHead
//**********************************************************************************
void Scene::HandleMoveHead(unsigned char key)
{
	switch (key) {
	case GLUT_KEY_RIGHT:
	{
		m_pElephant->SetNeckAngle(-4, 0);
		break;
	}
	case GLUT_KEY_LEFT:
	{
		m_pElephant->SetNeckAngle(4, 0);
		break;
	}
	case GLUT_KEY_UP:
	{
		m_pElephant->SetNeckAngle(0, 4);
		break;
	}
	case GLUT_KEY_DOWN:
	{
		m_pElephant->SetNeckAngle(0, -4);
		break;
	}
	}

	Draw();
}

//**********************************************************************************
// Scene::HandleLightGlobalIntensity
//**********************************************************************************
void Scene::HandleLightGlobalIntensity(unsigned char key)
{
	switch (key) {
		case '+':
		{
			SetLightGlobalIntensity(0.05f);
			break;
		}
		case '-':
		{
			SetLightGlobalIntensity(-0.05f);
			break;
		}
	}

	Draw();
}

//**********************************************************************************
// Scene::HandleGlobalLightColor
//**********************************************************************************
void Scene::HandleGlobalLightColor(unsigned char key)
{
  switch (key) {
  case GLUT_KEY_RIGHT:
  {
		m_GlobalLightColor[0] += 0.1f;
    break;
  }
  case GLUT_KEY_LEFT:
  {
		m_GlobalLightColor[0] -= 0.1f;
    break;
  }
  case GLUT_KEY_UP:
  {
		m_GlobalLightColor[1] += 0.1f;
    break;
  }
  case GLUT_KEY_DOWN:
  {
		m_GlobalLightColor[1] -= 0.1f;
    break;
  }
  case GLUT_KEY_PAGE_UP:
  {
		m_GlobalLightColor[2] += 0.1f;
    break;
  }
  case GLUT_KEY_PAGE_DOWN:
  {
		m_GlobalLightColor[2] += -0.1f;
    break;
  }
  }
  Draw();
}

//**********************************************************************************
// Scene::HandleSpotlightIntensity
//**********************************************************************************
void Scene::HandleSpotlightIntensity(unsigned char key)
{
	switch (key) {
	case '+':
	{
		m_pLamp->SetIntensity(0.05f);
		break;
	}
	case '-':
	{
		m_pLamp->SetIntensity(-0.05f);
		break;
	}
	}

	Draw();
}

//**********************************************************************************
// Scene::HandleElephantView
//**********************************************************************************
void Scene::HandleElephantView(unsigned char key)
{
  switch (key) {
  case GLUT_KEY_RIGHT:
  {
    m_pElephant->AddPosition(0, 0, 1);
    break;
  }
  case GLUT_KEY_LEFT:
  {
    m_pElephant->AddPosition(0, 0, -1);
    break;
  }
  case GLUT_KEY_UP:
  {
    m_pElephant->AddPosition(1, 0, 0);
    break;
  }
  case GLUT_KEY_DOWN:
  {
    m_pElephant->AddPosition(-1, 0, 0);
    break;
  }
  case GLUT_KEY_PAGE_UP:
  {
    m_pElephant->AddPosition(0, 1, 0);
    break;
  }
  case GLUT_KEY_PAGE_DOWN:
  {
    m_pElephant->AddPosition(0, -1, 0);
    break;
  }
  case '+':
  {
		m_pElephant->AddRotation(10.f);
    break;
  }
  case '-':
  {
		m_pElephant->AddRotation(-10.f);
    break;
  }
  }

  Draw();
}

//**********************************************************************************
// Scene::SetLampPosition
//**********************************************************************************
void Scene::SetLampPosition(float x, float y, float z)
{
	m_LightPos[0] += x;
	m_LightPos[1] += y;
	m_LightPos[2] += z;
}

