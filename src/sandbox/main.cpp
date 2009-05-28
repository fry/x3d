#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <gl/gl.h>
#include <gl/glu.h>

#include <iostream>
#include <x3d/Vector3.hpp>

#include "Win32Device.hpp"

#define PI 3.141592654
#define TWOPI 6.283185308
#define PID2 (PI/2.0)

void draw_sphere(double radius, int precision) {
  int i,j;
  double theta1,theta2,theta3;
  x3d::Vector3f e, p;

  for (j=0;j<precision/2;j++) {
    theta1 = j * TWOPI / precision - PID2;
    theta2 = (j + 1) * TWOPI / precision - PID2;

    glBegin(GL_QUAD_STRIP);
    for (i=0;i<=precision;i++) {
      theta3 = i * TWOPI / precision;

      e.x = cos(theta2) * cos(theta3);
      e.y = sin(theta2);
      e.z = cos(theta2) * sin(theta3);
      p.x = radius * e.x;
      p.y = radius * e.y;
      p.z = radius * e.z;

      glNormal3fv(e);
      glTexCoord2f(i/(double)precision,2*(j+1)/(double)precision);
      glVertex3fv(p);

      e.x = cos(theta1) * cos(theta3);
      e.y = sin(theta1);
      e.z = cos(theta1) * sin(theta3);
      p.x = radius * e.x;
      p.y = radius * e.y;
      p.z = radius * e.z;

      glNormal3fv(e);
      glTexCoord2f(i/(double)precision,2*j/(double)precision);
      glVertex3fv(p);
    }
    glEnd();
  }
}

void setup_opengl(x3d::BasicRenderer* renderer) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float aspect_ratio = static_cast<float>(renderer->get_screen_width())/ renderer->get_screen_height();
  gluPerspective(60.0, aspect_ratio, 0.1, 10000.0);
  glMatrixMode(GL_MODELVIEW);

  float diffuseLight[] =  {1.0f, 1.0f, 1.0f, 0.43f};
  float specularLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
  float shininessLight[] = {50.0f};
  float ambientLight[] = {0.7f, 0.7f, 0.7f, 1.0f};

  glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
  glLightfv(GL_LIGHT0, GL_SHININESS, shininessLight);
  //glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);

  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

int main(int argc, char* argv[]) {
  x3d::BasicDevice* device = new x3d::Win32Device(24, 800, 600, "Device/Driver Interface Test", true);
  device->set_resizeable(false);
  setup_opengl(device->get_renderer());

  float rot = 0.0;
  while (device->run()) {
    rot += 0.9f;
    device->get_renderer()->begin();
    glLoadIdentity();
    glTranslatef(0, 0, -5);
    glRotatef(rot, 0, 0, 1);
    glTranslatef(0, 2, 0);
    glRotatef(-rot, 0, 0, 1);
    draw_sphere(1, 40);
    device->get_renderer()->end();
  }

  /*ReleaseDC(hwnd, hdc);
  wglDeleteContext(hrc);*/
  return 0;
}