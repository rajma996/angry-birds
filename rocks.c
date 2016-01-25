#include "rocks.h"

vector<VAO> rocks;
vector<bool> rocks_status;
vector<VAO> threads;

float dis(float x1,float y1,float x2,float y2)
{
  return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}
void set_rocks()
{
  while(rocks.size()<3)
  {
    float x_temp = rand()%1200;
    float y_temp = rand()%60;
    rocks.push_back(*create_circle(x_temp,y_temp,50,50,0,0,0,0,0,0));
    threads.push_back(*create_rectangle(x_temp,(y_temp+800)/2,0.5,(y_temp+800)/2,0,1,1,1,0,0));
    rocks_status.push_back(0);
  }
}
void move_rocks()
{
  loop(i,0,rocks.size())
  {
    if(rocks_status[i]==0){ rocks[i].starttime=glfwGetTime(); continue; }
    float etime = glfwGetTime()-rocks[i].starttime;
    rocks[i].trans[1]=rocks[i].trans[1]+rocks[i].vy*etime-(etime)*(etime);
    rocks[i].vy-=etime;
    loop(j,0,targets.size())
    {
      if(rocks[i].sizey+targets[j].sizey>=dis(rocks[i].trans[0],rocks[i].trans[1],targets[j].trans[0],targets[j].trans[1]))
      {
        targets.erase(targets.begin()+j);
        score_value+=5;
      }
    }
  }
}
void draw_rocks()
{
  loop(i,0,rocks.size())
  {construct(threads[i]); construct(rocks[i]);  }
}
