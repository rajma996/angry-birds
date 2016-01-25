#include "balls.h"

vector<VAO> balls;
void releaseball()
{
  if(balls.size()==0 && power>0)
  {
		if(keys[GLFW_KEY_L]==1 && projectile_state==1) {keys[GLFW_KEY_L]=0; projectile_state = 0; power_projectile--;}
    balls.push_back(*create_rectangle(canon[1].trans[0],canon[1].trans[1],20,2,canon[1].rotate_angle,1,1,1,0,0));
    int t = balls.size();
    balls[t-1].vx=55*cos(canon[0].rotate_angle*M_PI/180)*((float)power/66);
    balls[t-1].vy=40*sin(canon[0].rotate_angle*M_PI/180)*((float)power/66);
  }
}
void erase_ball(int i)
{
	ortho_x = 1; ortho_y = 1; ortho_x_shift = 0; ortho_y_shift = 0;
	balls.erase(balls.begin()+i);
}
void move_balls()
{
  for (int i=0;i<balls.size();i++)
  {
    // printf("%f %f\n",balls[i].vx,balls[i].vy);
    if(balls[i].trans[0]<-1040 || balls[i].trans[0]>1040 || balls[i].trans[1]<-600 || balls[i].trans[1]>820) {  erase_ball(i);}
    float etime=glfwGetTime()-balls[i].starttime;
    balls[i].trans[0]=balls[i].trans[0]+balls[i].vx*(etime)/2;
    balls[i].trans[1]=balls[i].trans[1]+balls[i].vy*(etime)-(etime)*(etime);
    balls[i].vy=balls[i].vy-etime;
    if(balls[i].trans[1]-balls[i].sizey<=-600)
    {
      //  balls[i].vy=-0.7*balls[i].vy; balls[i].starttime=glfwGetTime(); balls[i].trans[1]=-290;
       erase_ball(i);

    }
    balls[i].rotate_angle = atan(balls[i].vy/balls[i].vx)*180/M_PI;
    // printf("%f\n",balls[i].rotate_angle);
		if(balls.size()>0)
		{ortho_x_shift = balls[i].trans[0]; ortho_y_shift = balls[i].trans[1];
		ortho_x = 0.99; ortho_y= 0.99; }
    for(int j=0;j<obstructions.size();j++)
    {
      float temp = obstructions[j].trans[0]-obstructions[j].sizex;
      // printf("%f\n", temp);
      float u = obstructions[j].trans[1]+obstructions[j].sizey; float l = obstructions[j].trans[1]-obstructions[j].sizey;
      if(balls[i].trans[0]+balls[i].sizex>=temp-10 && balls[i].trans[0]+balls[i].sizex<=temp+10 && balls[i].trans[1]<=u+5 && balls[i].trans[1]>=l-5) balls[i].vx*=-1;
    }
    loop(j,0,targets.size())
    {
      if(balls[i].sizex+targets[j].sizex>=dis(balls[i].trans[0],balls[i].trans[1],targets[j].trans[0],targets[j].trans[1]))
      {
        targets[j].sizex-=10; targets[j].sizey-=10;
        erase_ball(i);
        if(targets[j].sizex<=0 && j>=0 && j<targets.size())
        {
          targets.erase(targets.begin()+j);
          score_value+=5;
        }
        else
        targets[j]=*create_bubble(targets[j].trans[0],targets[j].trans[1],targets[j].sizex,targets[j].sizey,0,1,1,1,0,targets[j].vy);
      }
    }
    loop(j,0,rocks.size())
    {
      float x_cor = rocks[j].trans[0]; float y_cor_up=800; float y_cor_down = rocks[j].trans[1];
      if(balls[i].trans[1]>=y_cor_down && balls[i].trans[1]<=y_cor_up && balls[i].trans[0]+balls[i].sizex>=x_cor-20 && balls[i].trans[0]+balls[i].sizex<=x_cor+20)
      {
        threads[j]=*create_rectangle(threads[j].trans[0],threads[j].trans[1],threads[j].sizex,threads[j].sizey,0,0.2f,0.2f,0.2f,0,0);
        rocks_status[j]=1;
      }
    }

  }
}
void draw_balls()
{
  loop(i,0,balls.size()) construct(balls[i]);
}
