#include "balls.h"

vector<VAO> balls;
void releaseball()
{
  if(balls.size()==0)
  {
		if(keys['L'-'A']==1 && projectile_state==1) {keys['L'-'A']=0; projectile_state = 0; power_projectile--;}
    balls.push_back(*create_rectangle(canon[1].trans[0],canon[1].trans[1],15,8,canon[1].rotate_angle,1,1,1,0,0));
    int t = balls.size();
    balls[t-1].vx=55*cos(canon[0].rotate_angle*M_PI/180)*((float)power/66);
    balls[t-1].vy=40*sin(canon[0].rotate_angle*M_PI/180)*((float)power/66);
  }
}
float dis(float x1,float y1,float x2,float y2)
{
  return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}
void move_balls()
{
  for (int i=0;i<balls.size();i++)
  {
    // printf("%f %f\n",balls[i].vx,balls[i].vy);
    if(balls[i].trans[0]<-570 || balls[i].trans[0]>570 || balls[i].trans[1]<-300 ) {  balls.erase(balls.begin()+i); }
    float etime=glfwGetTime()-balls[i].starttime;
    balls[i].trans[0]=balls[i].trans[0]+balls[i].vx*(etime)/2;
    balls[i].trans[1]=balls[i].trans[1]+balls[i].vy*(etime)-(etime)*(etime);
    balls[i].vy=balls[i].vy-etime;
    if(balls[i].trans[1]-balls[i].sizey<=-300)
    {
       balls[i].vy=-0.7*balls[i].vy; balls[i].starttime=glfwGetTime(); balls[i].trans[1]=-290;
       if(balls[i].vy<1.3) balls.erase(balls.begin()+i);
    }
    balls[i].rotate_angle = atan(balls[i].vy/balls[i].vx)*180/M_PI;
    // printf("%f\n",balls[i].rotate_angle);
    for(int j=0;j<obstructions.size();j++)
    {
      float temp = obstructions[j].trans[0]-obstructions[j].sizex;
      float u = obstructions[j].trans[1]+obstructions[j].sizey; float l = obstructions[j].trans[1]-obstructions[j].sizey;
      if(balls[i].trans[0]+balls[i].sizex>=temp-10 && balls[i].trans[0]+balls[i].sizex<=temp+10 && balls[i].trans[1]<=u+5 && balls[i].trans[1]>=l-5) balls[i].vx*=-1;
    }
    loop(j,0,targets.size())
    {
      if(balls[i].sizex+targets[j].sizex>=dis(balls[i].trans[0],balls[i].trans[1],targets[j].trans[0],targets[j].trans[1]))
      {
        targets.erase(targets.begin()+j);
        score_value+=5;
      }
    }
  }
}
void draw_balls()
{
  loop(i,0,balls.size()) construct(balls[i]);
}
