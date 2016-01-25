#include "show_projectile.h"

vector<VAO> projectile;
int power_projectile ;
int projectile_state;

void set_power_projectile(int x)
{
  power_projectile = x;
}
void set_projectile()
{
  projectile.clear();
  float xpos = canon[0].trans[0];
  float ypos = canon[0].trans[1];
  float vx = 55*cos(canon[0].rotate_angle*M_PI/180)*((float)power/66);
  float vy = 40*sin(canon[0].rotate_angle*M_PI/180)*((float)power/66);
  float ttime = 0;
  while(1)
  {
    if(xpos<-1040 || xpos>1040 || ypos<-600 ) break;
    xpos=xpos+vx*(ttime)/2;
    ypos=ypos+vy*(ttime)-(ttime)*(ttime);
    vy=vy-ttime;
    if(ypos-10<=-600)
    {
       vy=-0.7*vy;  ypos=-580; ttime=0;
       if(vy<1.3) break;
    }
    ttime += 0.5;
    projectile.push_back(*create_circle(xpos,ypos,10,10,0,1,1,1,0,0));
  }
}
