#include "Objects.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const int g_slices = 50;
const int g_stacks = 50;

const GLfloat g_TableLegSizeX = 2;
const GLfloat g_TableLegSizeY = 15;
const GLfloat g_TableLegSizeZ = 2;
const GLfloat g_TableSurfaceSize = 20;
const GLfloat g_TableSurfaceThickness = 3;
const GLfloat g_TeapotSize = 5;

//**********************************************************************************
// Furniture::Furniture
//**********************************************************************************
Furniture::Furniture(float x, float y, float z)
{
  m_Pos[0] = x;
  m_Pos[1] = y;
  m_Pos[2] = z;
}

//**********************************************************************************
// Furniture::~Furniture
//**********************************************************************************
Furniture::~Furniture()
{
}

//**********************************************************************************
// Furniture::Draw
//**********************************************************************************
void Furniture::Draw() {
  //Translate furniture's initial position
  glPushMatrix();
  glTranslatef(m_Pos[0], m_Pos[1] + (g_TableLegSizeY / 2.f), m_Pos[2]);

  //Table legs
  glColor3fv(g_ColorTableLeg);
  glMaterialfv(GL_FRONT, GL_AMBIENT, g_ColorTableLeg);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, g_ColorTableLeg);

  Helpers::DrawCube(g_TableLegSizeX, g_TableLegSizeY, g_TableLegSizeZ);
  glTranslatef(g_TableSurfaceSize, 0, 0);
  Helpers::DrawCube(g_TableLegSizeX, g_TableLegSizeY, g_TableLegSizeZ);
  glTranslatef(0, 0, g_TableSurfaceSize);
  Helpers::DrawCube(g_TableLegSizeX, g_TableLegSizeY, g_TableLegSizeZ);
  glTranslatef(-g_TableSurfaceSize, 0, 0);
  Helpers::DrawCube(g_TableLegSizeX, g_TableLegSizeY, g_TableLegSizeZ);
  glTranslatef(0, 0, -g_TableSurfaceSize);

  //Table surface
  glColor3fv(g_ColorDarkBlue);
  glMaterialfv(GL_FRONT, GL_AMBIENT, g_ColorDarkBlue);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, g_ColorDarkBlue);

  glPushMatrix();
  glTranslatef(g_TableSurfaceSize / 2, (g_TableLegSizeY / 2) - (g_TableSurfaceThickness / 2), g_TableSurfaceSize / 2);
  glRotatef(90, 0, 1, 0);
  Helpers::DrawEllipsoid(g_TableSurfaceSize, g_TableSurfaceThickness, g_TableSurfaceSize, 50, 50);
  glPopMatrix();

  //Teapot
  glColor3fv(g_ColorTeapotAmbient);
  glMaterialfv(GL_FRONT, GL_AMBIENT, g_ColorTeapotAmbient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, g_ColorTeapotDiffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, g_ColorTeapotSpecular);
  glMaterialf(GL_FRONT, GL_SHININESS, 0.2f * 128);

  glPushMatrix();
  glTranslatef(g_TableSurfaceSize / 2, (g_TableLegSizeY / 2) + g_TableSurfaceThickness + 2, g_TableSurfaceSize / 2);
  glutSolidTeapot(g_TeapotSize);
  glPopMatrix();

  // Transparent Glass
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4fv(g_ColorGlass);
  glMaterialfv(GL_FRONT, GL_AMBIENT, g_ColorGlass);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, g_ColorGlass);

  glPushMatrix();
  glTranslatef(8 , g_TableLegSizeY , 25.0 );
  glRotatef(90, 1, 0, 0);
  Helpers::DrawCylinder(1.5, 6.0);
  glPopMatrix();
  glDisable(GL_BLEND);

  //Disable initial position translation
  glPopMatrix();
}

//**********************************************************************************
// Lamp::Lamp
//**********************************************************************************
Lamp::Lamp()
{
}

//**********************************************************************************
// Lamp::Draw
//**********************************************************************************
void Lamp::Draw(float x, float y, float z)
{
  m_Pos[0] = x;
  m_Pos[1] = y;
  m_Pos[2] = z;

  //Translate to lamp's position
  glPushMatrix();
  glTranslatef(m_Pos[0], m_Pos[1], m_Pos[2]);

  //Round lamp
  glPushMatrix();
  //glTranslatef(0, -55.f, 0);

  glColor3fv(m_color);
  glMaterialfv(GL_FRONT, GL_AMBIENT, m_color);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, m_color);
  glMaterialfv(GL_FRONT, GL_SPECULAR, m_color);


  Helpers::DrawSphere(5, 64, 64);
  glPopMatrix();

  //Remove lamp position translation
  glPopMatrix();
}


//**********************************************************************************
// Lamp::AddLightColor
//**********************************************************************************
void Lamp::AddLightColor(float r, float g, float b)
{
  m_color[0] += r;
  m_color[1] += g;
  m_color[2] += b;

  for (float& i : m_color)
  {
    if (i > 1.0)
      i = 1.0;
    else if (i < 0.0)
      i = 0.0;
  }
}

//**********************************************************************************
// Lamp::ApplyLighting
//**********************************************************************************
void Lamp::ApplyLighting()
{
  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20.0); //30 degress cutoff angle

  //Position
  GLfloat lightPosition[] = { m_Pos[0], m_Pos[1], m_Pos[2], 1.f };
  glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);

  // Set spotlight direction vector
  GLfloat directionPoint[] = { m_Direction[0], m_Direction[1], m_Direction[2], 1.f };
  GLfloat directionVector[] = { directionPoint[0] - m_Pos[0], directionPoint[1] - m_Pos[1], directionPoint[2] - m_Pos[2], 1 };
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, directionVector);

  //Intensity (diffuse / specular)
  this->SetIntensity(0);

  glEnable(GL_LIGHT1);
}

//**********************************************************************************
// Lamp::SetIntensity
//**********************************************************************************
void Lamp::SetIntensity(float diff)
{
  // Apply lamp intensity to opengl light

  // clamp intensity between 0 to 1
  m_intensity += diff;
  if (m_intensity > 0.99f)
    m_intensity = 0.99f;
  else if (m_intensity < 0.f)
    m_intensity = 0.f;

  float intensity[] = { m_color[0] * m_intensity, m_color[1] * m_intensity, m_color[2] * m_intensity, 1 };

  glLightfv(GL_LIGHT1, GL_AMBIENT, intensity);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, intensity);
}

//**********************************************************************************
// Lamp::SetLightDirection
//**********************************************************************************
void Lamp::SetLightDirection(float x, float y, float z)
{
  m_Direction[0] += x;
  m_Direction[1] += y;
  m_Direction[2] += z;
}

//**********************************************************************************
// Room::Room
//**********************************************************************************
Room::Room(float width, float height)
{
  m_width = width;
  m_height = height;

  GLuint textures[3];
  // generate opengl textures
  glGenTextures(3, textures);
  m_textureWall = textures[0];
  m_textureFloor = textures[1];
  m_textureCeiling = textures[2];

  // assign texture with images
  Helpers::LoadTexture(m_textureWall, "textures\\wall.jpg");
  Helpers::LoadTexture(m_textureFloor, "textures\\floor.jpg");
  Helpers::LoadTexture(m_textureCeiling, "textures\\ceiling.jpg");
}


//**********************************************************************************
// Room::Draw
//**********************************************************************************
void Room::Draw() {
  glEnable(GL_TEXTURE_2D);

  //Back wall
  glPushMatrix();
  glBindTexture(GL_TEXTURE_2D, m_textureWall);
  DrawWall();


  //Front wall
  glTranslatef(0.f, 0.f, m_width);
  DrawWall();
  glPopMatrix();

  //Left wall
  glPushMatrix();
  glRotatef(-90, 0, 1, 0);
  DrawWall();
  glPopMatrix();

  //Right Wall
  glPushMatrix();
  glTranslatef(m_width, 0.f, 0.f);
  glRotatef(-90, 0, 1, 0);
  DrawWall();
  glPopMatrix();

  //Floor
  DrawFloor();

  //Ceiling
  glBindTexture(GL_TEXTURE_2D, m_textureCeiling);
  glPushMatrix();
  glTranslatef(0.f, m_height, 0.f);
  DrawCeiling();
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);
}

//**********************************************************************************
// Room::DrawWall
//**********************************************************************************
void Room::DrawWall()
{
  glBegin(GL_POLYGON);

  glColor3fv(g_ColorWhite);
  glMaterialfv(GL_FRONT, GL_AMBIENT, g_ColorWhite);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, g_ColorYellow);
  glMaterialfv(GL_FRONT, GL_SPECULAR, g_ColorPurple);
  glNormal3f(1, 0, 0);

  glTexCoord2f(0.0, 0.0);
  glVertex3f(0, 0, 0);

  glTexCoord2f(0.0, 1.0);
  glVertex3f(0.f, m_height, 0.f);

  glTexCoord2f(1.0, 1.0);
  glVertex3f(m_width, m_height, 0.f);

  glTexCoord2f(1.0, 0.0);
  glVertex3f(m_width, 0.f, 0.f);

  glEnd();
}

//**********************************************************************************
// Room::DrawCeiling
//**********************************************************************************
void Room::DrawCeiling()
{
  glColor3fv(g_ColorWhite);
  // Some stylish lighting conditions
  glMaterialfv(GL_FRONT, GL_AMBIENT, g_ColorWhite);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, g_ColorYellow);
  glMaterialfv(GL_FRONT, GL_SPECULAR, g_ColorPurple);
  glMaterialf(GL_FRONT, GL_SHININESS, 60.0);

  // Draw polygon with 4 vertices
  glBegin(GL_POLYGON);

  glNormal3f(0, 1, 0);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(0, 0, 0);

  glNormal3f(0, 1, 0);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(m_width, 0.f, 0.f);

  glNormal3f(0, 1, 0);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(m_width, 0.f, m_width);

  glNormal3f(0, 1, 0);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(0.f, 0.f, m_width);

  glEnd();
}

//**********************************************************************************
// Room::DrawFloor
//**********************************************************************************
void Room::DrawFloor()
{
  // Draw stylish floor patten
  glMaterialf(GL_FRONT, GL_SHININESS, 10.0);

  glNormal3f(0, 1, 0);
  glBegin(GL_QUADS);

  float x, y;
  int quadSize = 8;
  for (int i = 0; i < (m_width / quadSize); i++)
  {
    for (int j = 0; j < (m_width / quadSize); j++)
    {
      if ((i + j) % 2 == 0)
      {
        glColor3fv(g_ColorFloorPlate1);
        glMaterialfv(GL_FRONT, GL_AMBIENT, g_ColorFloorPlate1);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, g_ColorFloorPlate1);
      }
      else
      {
        glColor3fv(g_ColorFloorPlate2);
        glMaterialfv(GL_FRONT, GL_AMBIENT, g_ColorFloorPlate2);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, g_ColorFloorPlate2);
      }

      x = (float)i * quadSize;
      y = (float)j * quadSize;
      glVertex3f(x, 0.f, y);
      glVertex3f(x, 0.f, y + quadSize);
      glVertex3f(x + quadSize, 0.f, y + quadSize);
      glVertex3f(x + quadSize, 0.f, y);
    }
  }

  glEnd();
}

//**********************************************************************************
// ElephantLeg::ElephantLeg
//**********************************************************************************
ElephantLeg::ElephantLeg(int index, float bodyLength, float bodyHeight, float bodyWidth)
{
  m_upperLen = bodyLength * 0.3f;
  m_upperThickness = bodyHeight * 0.2f;
  m_upperWidth = bodyWidth * 0.6f;

  m_lowerLen = bodyLength * 0.2f;
  m_lowerThickness = bodyHeight * 0.2f;
  m_lowerWidth = bodyWidth * 0.6f;

  m_feetAngle = -m_upperAngle - m_lowerAngle;
  m_feetLen = m_lowerThickness * 1.5f;
  m_feetThickness = m_lowerLen * 0.4f;
  m_feetWidth = m_lowerWidth;
  if (index % 2 == 0) {
    m_lowerAngle = 0.0f;
  }
}

//**********************************************************************************
// ElephantLeg::ElephantLeg
//**********************************************************************************
ElephantLeg::ElephantLeg()
{

}

//**********************************************************************************
// ElephantLeg::Draw
//**********************************************************************************
void ElephantLeg::Draw() {
  glPushMatrix();

  m_feetAngle = -m_lowerAngle - m_upperAngle;

  //Draw upper leg
  glRotatef(m_upperAngle, 0.0, 0.0, 1.0);
  glTranslatef(m_upperLen / 2.0f, 0.0f, 0.0f);
  Helpers::DrawCube(m_upperLen, m_upperThickness, m_upperWidth);

  //Draw lower leg
  glTranslatef(m_upperLen / 2.0f, 0.0f, 0.0f);
  glRotatef(m_lowerAngle, 0.0, 0.0, 1.0);
  glTranslatef(m_lowerLen / 2.0f, 0.0f, 0.0f);
  Helpers::DrawCube(m_lowerLen, m_lowerThickness, m_lowerWidth);

  //Draw Feet
  glTranslatef(m_lowerLen / 2.0f, -m_lowerThickness / 2.0f, 0.0f);
  glRotatef(m_feetAngle, 0.0, 0.0, 1.0);
  glTranslatef(m_feetLen / 2.0f, 0.0f, 0.0f);
  Helpers::DrawCube(m_feetLen, m_feetThickness, m_feetWidth);

  glPopMatrix();
}

//**********************************************************************************
// Elephant::Elephant
//**********************************************************************************
Elephant::Elephant(int baseLength) {
	m_baseLength = (float)baseLength;
	m_bodyHeight = m_baseLength * 0.3f;
	m_bodyWidth = m_baseLength * 0.3f;
	
	//Create 4 legs
	for (int i = 0; i < 4; i++)
		m_legs[i] = ElephantLeg(i, m_baseLength, m_bodyHeight, m_bodyWidth);
	
	ResetParameters(baseLength);
}

//**********************************************************************************
// Elephant::ResetParameters
//**********************************************************************************
//Resets all angles
void Elephant::ResetParameters(int base) {
	m_neckAngleZ = 0;
	m_neckAngleY = 0;
	m_tailAngleZ = -30;
	m_tailAngleY = 0;
}


//**********************************************************************************
// Elephant::Draw
//**********************************************************************************
//Draw the whole elephant
void Elephant::Draw() {
	glPushMatrix();
	glTranslatef(m_elephantPos[0], m_elephantPos[1], m_elephantPos[2]);
  glRotatef(m_RotationAngle, 0.f, 1.f, 0.f);

	DrawLegs();
	DrawTorso();
	DrawTail();
	DrawFace();
	
	glPopMatrix();
}

//**********************************************************************************
// Elephant::DrawTorso
//**********************************************************************************
//Draw the body
void Elephant::DrawTorso() {
	glPushMatrix();

	glColor3fv(g_ColorTorso);
	glMaterialfv(GL_FRONT, GL_AMBIENT, g_ColorTorso);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, TorsoDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, g_ColorBlack);

	Helpers::DrawEllipsoid(m_baseLength * 0.7f, m_bodyHeight , m_bodyWidth *1.5f, 10, 10);
	glPopMatrix();
}

//**********************************************************************************
// Elephant::DrawFace
//**********************************************************************************
void Elephant::DrawFace() {
	glPushMatrix();
	
	//Neck
	glTranslatef(m_baseLength * 0.65f, m_bodyHeight / 2.0f, 0.0f);
	glPushMatrix();
	
	glColor3fv(g_ColorNeck);
	glMaterialfv(GL_FRONT, GL_AMBIENT, g_ColorNeck);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, g_ColorNeck);
	
	float neckLength = m_baseLength * 0.45f;
	glRotatef(45 + m_neckAngleZ, 0, 0, 1); 
	glRotatef(90, 0, 1, 0);
  Helpers::DrawEllipsoid(m_bodyWidth / 2.f, m_bodyWidth * 0.4f, neckLength * 0.5f, 100, 100);

	glPopMatrix();

	//Head
	glRotatef(m_neckAngleZ, 0, 0, 1);
	glRotatef(m_neckAngleY, 0, 1, 0);

	float neckHeight = m_bodyHeight * 0.4f;
	float headRadius = neckLength * 0.5f;
	glTranslatef(neckLength * 0.3f, neckHeight * 0.8f, 0.0f);
	glColor3fv(g_ColorHead);
	glMaterialfv(GL_FRONT, GL_AMBIENT, g_ColorHead);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, g_ColorHead);
	glMaterialfv(GL_FRONT, GL_SPECULAR, g_ColorBlack);
	Helpers::DrawSphere(headRadius, g_slices, g_stacks);

	//Ears
	glColor3fv(g_ColorEar);
	glMaterialfv(GL_FRONT, GL_AMBIENT, g_ColorEar);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, g_ColorEar);
	
	glPushMatrix();
	glTranslatef(headRadius * 0.3f, -headRadius * 0.1f , headRadius);
	//glRotatef(45, 1, 0, 0);	
	Helpers::DrawEllipsoid(2, 3, 2, g_slices, g_stacks);
	glPopMatrix();

	glPushMatrix();
	//glTranslatef(0, headRadius * 0.6, -headRadius * 0.7);
  glTranslatef(headRadius * 0.3f, -headRadius * 0.1f, -headRadius);
	//glRotatef(-45, 1, 0, 0);
	Helpers::DrawEllipsoid(2, 3,2, g_slices, g_stacks);
	glPopMatrix();

  // Teeth
  glColor3fv(g_ColorTeeth);
  glMaterialfv(GL_FRONT, GL_AMBIENT, g_ColorTeeth);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, g_ColorTeeth);

  glPushMatrix();
  glTranslatef(headRadius * 0.7f, -headRadius * 0.1f , headRadius * 0.5f );
  glRotatef(-45, 0, 0, 1);	
  Helpers::DrawEllipsoid(3.f, 0.2f, 0.2f, g_slices, g_stacks);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(headRadius * 0.7f, -headRadius * 0.1f, -headRadius * 0.5f);
  glRotatef(-45, 0, 0, 1);
  Helpers::DrawEllipsoid(3.f, 0.2f, 0.2f, g_slices, g_stacks);
  glPopMatrix();


	//Eyes
	glColor3fv(g_ColorEye);
	glMaterialfv(GL_FRONT, GL_AMBIENT, g_ColorEye);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, g_ColorEye);

	glPushMatrix();
	glTranslatef(headRadius * 0.75f, headRadius * 0.15f, headRadius * 0.3f);
	Helpers::DrawEllipsoid(1.f, 1.f, 0.5f, g_slices, g_stacks);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(headRadius * 0.75f, headRadius * 0.15f, -headRadius * 0.3f);
	Helpers::DrawEllipsoid(1.f, 1.f, 0.5f, g_slices, g_stacks);
	glPopMatrix();

	//Nose
	glColor3fv(g_ColorNose);
	glMaterialfv(GL_FRONT, GL_AMBIENT, g_ColorNose);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, g_ColorNose);

	glPushMatrix();
	glTranslatef(headRadius * 1.2f, -headRadius * 0.2f, 0.f);

	//Calculate eye's position in world cooordinates (according to nose's position)
	CalcEyesPosition();

  glRotatef(-45.f, 0.f, 0.f, 1.f);
  Helpers::DrawEllipsoid(7.f, 1.f, 1.f, g_slices, g_stacks);
	glPopMatrix();

	glPopMatrix();
}

//**********************************************************************************
// Elephant::DrawTail
//**********************************************************************************
void Elephant::DrawTail() {

  float tailLength = m_baseLength * 0.3f;
  float tailWidthHeight = m_bodyWidth * 0.15f;

	glPushMatrix();
	glTranslatef(-m_baseLength / 2.0f - tailLength / 2.0f, -0.5, 0.0f);
	
	//Rotate according to user's selected angle
	glRotatef(m_tailAngleZ, 0, 0, 1);
	glRotatef(m_tailAngleY, 0, 1, 0);
	
	glColor3fv(g_ColorTail);
	glMaterialfv(GL_FRONT, GL_AMBIENT, g_ColorTail);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, g_ColorTail);

	Helpers::DrawEllipsoid(tailLength, tailWidthHeight, tailWidthHeight, g_slices, g_stacks);
	glPopMatrix();
}

//**********************************************************************************
// Elephant::DrawLegs
//**********************************************************************************
void Elephant::DrawLegs() {
	// Setup the legs in order
	int v[4][3] = {
		{	 1,	 1,	 1 },
		{	-1,	-1,	 1 },
		{	 1,	 1,	-1 },
		{	-1,	-1,	-1 }
	};

	float legX = m_baseLength * 0.3f;
  float legY = 0.0f;
  float legZ = m_bodyWidth * 0.5f;

	for (int i = 0; i < 4; i++) {
		glPushMatrix();

		glColor3fv(g_ColorLeg);
		glMaterialfv(GL_FRONT, GL_AMBIENT, g_ColorLeg);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, g_ColorLeg);

		glTranslatef(v[i][0] * legX, v[i][1] * legY, v[i][2] * legZ);
		m_legs[i].Draw();

		glPopMatrix();
	}
}

//**********************************************************************************
// Elephant::SetPosition
//**********************************************************************************
//Set the elephant's position (center of torso position)
void Elephant::SetPosition(float x, float y, float z) {
  m_elephantPos[0] = x;
  m_elephantPos[1] = y;
  m_elephantPos[2] = z;
}


//**********************************************************************************
// Elephant::AddPosition
//**********************************************************************************
void Elephant::AddPosition(float x, float y, float z)
{
  m_elephantPos[0] += x;

	if(m_elephantPos[1] + y > 11.f)
    m_elephantPos[1] += y;
  m_elephantPos[2] += z;
}


void Elephant::AddRotation(float angle)
{
  m_RotationAngle += angle;
}

//**********************************************************************************
// Elephant::SetTailAngle
//**********************************************************************************
void Elephant::SetTailAngle(int yDiff, int zDiff)
{
	m_tailAngleY += yDiff;
	if (m_tailAngleY <= -40) {
		m_tailAngleY = -40;
	}
	else if (m_tailAngleY >= 40) {
		m_tailAngleY = 40;
	}

	m_tailAngleZ += zDiff;
	if (m_tailAngleZ <= -32) {
		m_tailAngleZ = -32;
	}
	else if (m_tailAngleZ >= 0) {
		m_tailAngleZ = 0;
	}
}

//**********************************************************************************
// Elephant::SetNeckAngle
//**********************************************************************************
void Elephant::SetNeckAngle(int yDiff, int zDiff)
{
	m_neckAngleY += yDiff;
	if (m_neckAngleY <= -40) {
		m_neckAngleY = -40;
	}
	else if (m_neckAngleY >= 40) {
		m_neckAngleY = 40;
	}

	m_neckAngleZ += zDiff;
	if (m_neckAngleZ <= -32) {
		m_neckAngleZ = -32;
	}
	else if (m_neckAngleZ >= 45) {
		m_neckAngleZ = 45;
	}
}

//**********************************************************************************
// Elephant::GetEyesPosition
//**********************************************************************************
void Elephant::GetEyesPosition(GLfloat oEyePos[4])
{
	memcpy(oEyePos, m_eyesPosition, sizeof(m_eyesPosition));
}

//**********************************************************************************
// Elephant::GetEyesLookAt
//**********************************************************************************
void Elephant::GetEyesLookAt(GLfloat oEyeLookAt[4])
{
	memcpy(oEyeLookAt, m_eyesLookAt, sizeof(m_eyesLookAt));
}

//**********************************************************************************
// Elephant::SaveViewMatrix
//**********************************************************************************
void Elephant::SaveViewMatrix()
{
	//Get the current model view matrix which includes only the view transformation
	float modelViewMat[16];
	Helpers::GetModelViewMatrix(modelViewMat);
	
	//Calculate the inverse of the view matrix and save it (for later use of eye position calculation)
	Helpers::GetInvertMatrix(modelViewMat, m_viewInvMatrix);
}

//**********************************************************************************
// Elephant::CalcEyesPosition
//**********************************************************************************
void Elephant::CalcEyesPosition()
{
	//Get the model view matrix at the moment (just before the eyes are going to be drawn)
	float modelViewMat[16];
	Helpers::GetModelViewMatrix(modelViewMat);
	
	//Multiply by the inverse matrix of the view point itself in order to disable the effect of the camera view.
	/*
	V = view transformation matrix
	M = model transformation matrix
	Current model view matrix = V*M
	But we need just the model transformation matrix (M)
	If we multiply by V^-1 from the left then we'll get V^-1*V*M = I*M = M
	*/
	float modelMat[16];
	Helpers::MultiplyMatrixMatrix(modelMat,m_viewInvMatrix, modelViewMat);

	//Multiply model transformation matrix by the unit vector in order to get the eye's position
	float vec[] = { 1, 1, 1, 1 };
	Helpers::MultiplyMatrixVector(m_eyesPosition, modelMat, vec);


	//Now calculate also the lookAt point. We'll make the elephant look straight (parallel to the nose)
	//To do so, translate by 10 on the X axis and then take again the modle-view matrix and calculate the new point's position
	glTranslatef(10, 0, 0);
	Helpers::GetModelViewMatrix(modelViewMat);
	Helpers::MultiplyMatrixMatrix(modelMat, m_viewInvMatrix, modelViewMat);
	Helpers::MultiplyMatrixVector(m_eyesLookAt, modelMat, vec);
	glTranslatef(-10, 0, 0);
}