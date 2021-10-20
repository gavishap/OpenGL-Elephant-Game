#include "Helpers.h"

#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION

//**********************************************************************************
// Helpers::Helpers
//**********************************************************************************
Helpers::Helpers()
{
}


//**********************************************************************************
// Helpers::~Helpers
//**********************************************************************************
Helpers::~Helpers()
{
}

//**********************************************************************************
// Helpers::ToRadians
//**********************************************************************************
float Helpers::ToRadians(float degrees) {
  return (float)(degrees / 180 * PI);
}

//**********************************************************************************
// Helpers::DrawCube
//**********************************************************************************
void Helpers::DrawCube(float length, float height , float width ) {

  // Draw the quad of 24 vertices
  glBegin(GL_QUADS);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(length / 2.0f, height / 2.0f, -width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(-length / 2.0f, height / 2.0f, -width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(-length / 2.0f, height / 2.0f, width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(length / 2.0f, height / 2.0f, width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(length / 2.0f, -height / 2.0f, width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(-length / 2.0f, -height / 2.0f, width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(-length / 2.0f, -height / 2.0f, -width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(length / 2.0f, -height / 2.0f, -width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(length / 2.0f, height / 2.0f, width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(-length / 2.0f, height / 2.0f, width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(-length / 2.0f, -height / 2.0f, width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(length / 2.0f, -height / 2.0f, -width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(length / 2.0f, -height / 2.0f, -width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(-length / 2.0f, -height / 2.0f, -width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(-length / 2.0f, height / 2.0f, -width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(length / 2.0f, height / 2.0f, -width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(-length / 2.0f, height / 2.0f, width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(-length / 2.0f, height / 2.0f, -width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(-length / 2.0f, -height / 2.0f, -width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(-length / 2.0f, -height / 2.0f, width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(length / 2.0f, height / 2.0f, -width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(length / 2.0f, height / 2.0f, width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(length / 2.0f, -height / 2.0f, width / 2.0f);

  glNormal3f(0, 1.0, 0.0);
  glVertex3f(length / 2.0f, -height / 2.0f, -width / 2.0f);

  glEnd();
}

//**********************************************************************************
// Helpers::DrawSphere
//**********************************************************************************
void Helpers::DrawSphere(float radius, int slices, int stacks) {
  glutSolidSphere(radius, slices, stacks);
}

//**********************************************************************************
// Helpers::DrawText
//**********************************************************************************
void Helpers::DrawText(string text, unsigned int x, unsigned int y)
{
  //Save model view matrix
  glPushMatrix();
  glLoadIdentity();

  //Save projection matrix
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  // Set ortho projection depending upon current viewport size
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  gluOrtho2D(0, viewport[2], 0, viewport[3]);

  //Draw bitmap at give x y
  glColor3fv(g_ColorBlack);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, g_ColorBlack);
  glRasterPos2f((GLfloat)x, (GLfloat)y);
  for (unsigned int i = 0; i < text.length(); i++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]); //Draw one letter according to the font
  }

  //Reset to previously saved projection matrix
  glPopMatrix();

  //Reset to previously saved model view matrix
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

//**********************************************************************************
// Helpers::DrawEllipsoid
//**********************************************************************************
void Helpers::DrawEllipsoid(float fA, float fB, float fC, unsigned int uiSlices, unsigned int uiStacks)
{
  // Calculate tessellation
  float tStep = (float)PI / (float)uiSlices;
  float sStep = (PI) / (float)uiStacks;
  for (float t = -PI / 2; t <= (PI / 2) + .0001; t += tStep)
  {
    // Render tessellated vertices
    glBegin(GL_TRIANGLE_STRIP);
    for (float s = -PI; s <= PI + .0001; s += sStep)
    {
      glNormal3f(1.0, 0.0, 0.0);
      glVertex3f((float)(fA * cos(t) * cos(s)), (float)(fB * cos(t) * sin(s)), (float)(fC * sin(t)));

      glNormal3f(1.0, 0.0, 0.0);
      glVertex3f((float)(fA * cos(t + tStep) * cos(s)), (float)(fB * cos(t + tStep) * sin(s)), (float)(fC * sin(t + tStep)));
    }
    glEnd();
  }
}

//**********************************************************************************
// Helpers::LoadTexture
//**********************************************************************************
void Helpers::LoadTexture(GLuint texture, const char* filePath)
{
  // Standard steps to bind and load textures
  glBindTexture(GL_TEXTURE_2D, texture);
  // set texture wrapping/filtering 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load and generate the texture
  int width, height, nrChannels;
  unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  }
  else
  {
    printf("*** Error loading textures!!!\n");
  }
  stbi_image_free(data);
}

//**********************************************************************************
// Helpers::DrawCylinder
//**********************************************************************************
void Helpers::DrawCylinder(GLdouble baseRadius, GLdouble length)
{
  // Use inbuilt glut function to draw cylinder
  GLUquadricObj* quadratic;
  quadratic = gluNewQuadric();
  gluCylinder(quadratic, baseRadius, baseRadius, length, 32, 32);
}

//**********************************************************************************
// Helpers::AdjustWindow
//**********************************************************************************
void Helpers::AdjustWindow(string windowTitle)
{
  // called once while init to start the app in maximized mode
  HWND win_handle = FindWindow(0, windowTitle.c_str());
  if (!win_handle)
  {
    printf("*** Created OpenGL window not found!!!\n");
  }

  SetWindowLong(win_handle, GWL_STYLE, (GetWindowLong(win_handle, GWL_STYLE) | WS_MAXIMIZE));
  ShowWindowAsync(win_handle, SW_SHOWMAXIMIZED);
}

//**********************************************************************************
// Helpers::GetModelViewMatrix
//**********************************************************************************
void Helpers::GetModelViewMatrix(float oMat[16])
{
  memset(oMat, 0, 16 * sizeof(float));
  glGetFloatv(GL_MODELVIEW_MATRIX, oMat);
}

//**********************************************************************************
// Helpers::MultiplyMatrixVector
//**********************************************************************************
void Helpers::MultiplyMatrixVector(float oRes[4], float* matrix, float* vector)
{
  //Multiple matrix with vector

  memset(oRes, 0, 4 * sizeof(float));

  for (int i = 0; i < 4; i++)
  {
    oRes[0] += (matrix[0 + 4 * i] * vector[i % 4]);
    oRes[1] += (matrix[1 + 4 * i] * vector[i % 4]);
    oRes[2] += (matrix[2 + 4 * i] * vector[i % 4]);
    oRes[3] += (matrix[3 + 4 * i] * vector[i % 4]);
  }
}

//**********************************************************************************
// Helpers::MultiplyMatrixMatrix
//**********************************************************************************
void Helpers::MultiplyMatrixMatrix(float oRes[16], float* matrix1, float* matrix2)
{
  // Matrix matrix multiplication
  memset(oRes, 0, 16 * sizeof(float));

  int i, j, k;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      oRes[j * 4 + i] = 0;

      for (k = 0; k < 4; k++) {
        oRes[j * 4 + i] += matrix1[k * 4 + i] * matrix2[j * 4 + k];
      }
    }
  }
}

//**********************************************************************************
// Helpers::GetInvertMatrix
//**********************************************************************************
bool Helpers::GetInvertMatrix(const float m[16], float invOut[16])
{
  float inv[16], det;
  int i;

  inv[0] = m[5] * m[10] * m[15] -
    m[5] * m[11] * m[14] -
    m[9] * m[6] * m[15] +
    m[9] * m[7] * m[14] +
    m[13] * m[6] * m[11] -
    m[13] * m[7] * m[10];

  inv[4] = -m[4] * m[10] * m[15] +
    m[4] * m[11] * m[14] +
    m[8] * m[6] * m[15] -
    m[8] * m[7] * m[14] -
    m[12] * m[6] * m[11] +
    m[12] * m[7] * m[10];

  inv[8] = m[4] * m[9] * m[15] -
    m[4] * m[11] * m[13] -
    m[8] * m[5] * m[15] +
    m[8] * m[7] * m[13] +
    m[12] * m[5] * m[11] -
    m[12] * m[7] * m[9];

  inv[12] = -m[4] * m[9] * m[14] +
    m[4] * m[10] * m[13] +
    m[8] * m[5] * m[14] -
    m[8] * m[6] * m[13] -
    m[12] * m[5] * m[10] +
    m[12] * m[6] * m[9];

  inv[1] = -m[1] * m[10] * m[15] +
    m[1] * m[11] * m[14] +
    m[9] * m[2] * m[15] -
    m[9] * m[3] * m[14] -
    m[13] * m[2] * m[11] +
    m[13] * m[3] * m[10];

  inv[5] = m[0] * m[10] * m[15] -
    m[0] * m[11] * m[14] -
    m[8] * m[2] * m[15] +
    m[8] * m[3] * m[14] +
    m[12] * m[2] * m[11] -
    m[12] * m[3] * m[10];

  inv[9] = -m[0] * m[9] * m[15] +
    m[0] * m[11] * m[13] +
    m[8] * m[1] * m[15] -
    m[8] * m[3] * m[13] -
    m[12] * m[1] * m[11] +
    m[12] * m[3] * m[9];

  inv[13] = m[0] * m[9] * m[14] -
    m[0] * m[10] * m[13] -
    m[8] * m[1] * m[14] +
    m[8] * m[2] * m[13] +
    m[12] * m[1] * m[10] -
    m[12] * m[2] * m[9];

  inv[2] = m[1] * m[6] * m[15] -
    m[1] * m[7] * m[14] -
    m[5] * m[2] * m[15] +
    m[5] * m[3] * m[14] +
    m[13] * m[2] * m[7] -
    m[13] * m[3] * m[6];

  inv[6] = -m[0] * m[6] * m[15] +
    m[0] * m[7] * m[14] +
    m[4] * m[2] * m[15] -
    m[4] * m[3] * m[14] -
    m[12] * m[2] * m[7] +
    m[12] * m[3] * m[6];

  inv[10] = m[0] * m[5] * m[15] -
    m[0] * m[7] * m[13] -
    m[4] * m[1] * m[15] +
    m[4] * m[3] * m[13] +
    m[12] * m[1] * m[7] -
    m[12] * m[3] * m[5];

  inv[14] = -m[0] * m[5] * m[14] +
    m[0] * m[6] * m[13] +
    m[4] * m[1] * m[14] -
    m[4] * m[2] * m[13] -
    m[12] * m[1] * m[6] +
    m[12] * m[2] * m[5];

  inv[3] = -m[1] * m[6] * m[11] +
    m[1] * m[7] * m[10] +
    m[5] * m[2] * m[11] -
    m[5] * m[3] * m[10] -
    m[9] * m[2] * m[7] +
    m[9] * m[3] * m[6];

  inv[7] = m[0] * m[6] * m[11] -
    m[0] * m[7] * m[10] -
    m[4] * m[2] * m[11] +
    m[4] * m[3] * m[10] +
    m[8] * m[2] * m[7] -
    m[8] * m[3] * m[6];

  inv[11] = -m[0] * m[5] * m[11] +
    m[0] * m[7] * m[9] +
    m[4] * m[1] * m[11] -
    m[4] * m[3] * m[9] -
    m[8] * m[1] * m[7] +
    m[8] * m[3] * m[5];

  inv[15] = m[0] * m[5] * m[10] -
    m[0] * m[6] * m[9] -
    m[4] * m[1] * m[10] +
    m[4] * m[2] * m[9] +
    m[8] * m[1] * m[6] -
    m[8] * m[2] * m[5];

  det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

  if (det == 0)
    return false;

  det = 1.f / det;

  for (i = 0; i < 16; i++)
    invOut[i] = inv[i] * det;

  return true;
}

