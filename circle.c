#include "circle.h"

VAO* create_circle(float x,float y,float a,float b,float angle,float red,float green , float blue)
{
  GLfloat vertex_buffer_data[650];
  int i=0,j=0;
  for (i=1;i<=72;i++)
  {
      int angle = i*5;
      vertex_buffer_data[j++]=a*cos((angle*(3.14159265))/180);
      vertex_buffer_data[j++]=b*sin((angle*(3.14159265))/180);
      vertex_buffer_data[j++]=0;
      vertex_buffer_data[j++]=0;
      vertex_buffer_data[j++]=0;
      vertex_buffer_data[j++]=0;
      vertex_buffer_data[j++]=a*cos(((angle-5)*(3.14159265))/180);
      vertex_buffer_data[j++]=b*sin(((angle-5)*(3.14159265))/180);
      vertex_buffer_data[j++]=0;
  }
  GLfloat color_buffer_data [650];j=0;
  for (int r=1;r<=72;r++)
  {
    color_buffer_data[j++]=red;color_buffer_data[j++]=green;color_buffer_data[j++]=blue;
    color_buffer_data[j++]=red;color_buffer_data[j++]=green;color_buffer_data[j++]=blue;
    color_buffer_data[j++]=red;color_buffer_data[j++]=green;color_buffer_data[j++]=blue;
  }
  struct VAO* vao = new struct VAO;
  vao = create3DObject(GL_TRIANGLES, 216 , vertex_buffer_data, color_buffer_data, GL_FILL);
  vao->trans[0]=x; vao->trans[1]=y; vao->trans[2]=0; vao->sizex=a; vao->sizey=b;
  vao->rotate_angle=angle; vao->starttime=glfwGetTime();vao->vx=0; vao->vy=0;
  return vao;
}
