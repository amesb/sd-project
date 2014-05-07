#ifdef MACOSX
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <iostream>
using namespace std;

#include "globaldefs.h"
#include "koala.h"
#include "texture.h"

Koala::Koala() : x(10), y(120), atBottom(true)
{
  targetx = x;
  targety = y;
}

bool Koala::isAtTarget()
{
  return !(x==targetx && y==targety);
}

void Koala::approachTarget()
{
  cerr << "approaching" << endl;
  x += (targetx - x) / 15;
  y += (targety - y) / 15;
}

void Koala::scrollKoalaUp()
{
  targety += HEIGHT - 200 - y;
  y = HEIGHT - 200;
}

void Koala::scrollKoalaDown()
{
  targety += 100 - y;
  y = 100;
}

void Koala::drawTrajectory(int mouseposx, int mouseposy)
{
  /* not yet implemented -- see also some code removed in
   * 5bdf728d5fa4cff5eccdabd27f18cc06a9df895f
  int hypotenuse = sqrt(((mouseposx-getCtrX())*(mouseposx-getCtrX())) +
                        ((mouseposy-getCtrY())*(mouseposy-getCtrY())));

  if(hypotenuse > 100)
    hypotenuse = 100;
  */

  glColor3f(1,1,1);
    glLineStipple(1, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glVertex3f(getCtrX(), getCtrY(), 0);
    glVertex3f(mouseposx, mouseposy, 0);
  glEnd();
}

void Koala::drawKoala(int mouseposx)
{
  if(mouseposx > x)
    drawTexture(texture,x,y,100,-100);
  else
    drawTexture(texture, x, y, 100,-100, 1.0,M_PI);
}
