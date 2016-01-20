#include "rectangle.h"

VAO* create_rectangle(float x,float y,float a,float b,float angle,float red,float green , float blue,float vx=0,float vy=0 )
{
  GLfloat vertex_buffer_data[100];
  int i=0,j=0;
  vertex_buffer_data[i++]=a;vertex_buffer_data[i++]=b;vertex_buffer_data[i++]=0;
  vertex_buffer_data[i++]=-1*a;vertex_buffer_data[i++]=-1*b;vertex_buffer_data[i++]=0;
  vertex_buffer_data[i++]=a;vertex_buffer_data[i++]=-1*b;vertex_buffer_data[i++]=0;
  vertex_buffer_data[i++]=-1*a;vertex_buffer_data[i++]=b;vertex_buffer_data[i++]=0;
  vertex_buffer_data[i++]=-1*a;vertex_buffer_data[i++]=-1*b;vertex_buffer_data[i++]=0;
  vertex_buffer_data[i++]=a;vertex_buffer_data[i++]=b;vertex_buffer_data[i++]=0;
  GLfloat color_buffer_data [100];j=0;
  for(int i=1;i<=6;i++)
  {
    color_buffer_data[j++]=red;color_buffer_data[j++]=green;color_buffer_data[j++]=blue;
    color_buffer_data[j++]=red;color_buffer_data[j++]=green;color_buffer_data[j++]=blue;
    color_buffer_data[j++]=red;color_buffer_data[j++]=green;color_buffer_data[j++]=blue;
  }
  struct VAO* vao = new struct VAO;
  vao = create3DObject(GL_TRIANGLES, 6 , vertex_buffer_data, color_buffer_data, GL_FILL);
  vao->trans[0]=x; vao->trans[1]=y; vao->trans[2]=0; vao->sizex=a; vao->sizey=b;
  vao->rotate_angle=angle; vao->starttime=glfwGetTime(); vao->vx=vx; vao->vy=vy; vao->starttime=glfwGetTime();
  return vao;

}
