#include "score.h"

vector<VAO> score;

void rectangle1(float x,float y)
{
    score.push_back(*create_rectangle(x,y+2*lengthx,lengthx,lengthy,0,0,0,0,0,0));
}
void rectangle2(float x,float y)
{
    score.push_back(*create_rectangle(x+lengthx,y+lengthx,lengthx,lengthy,90,0,0,0,0,0));
}
void rectangle3(float x,float y)
{
    score.push_back(*create_rectangle(x-lengthx,y+lengthx,lengthx,lengthy,90,0,0,0,0,0));
}
void rectangle4(float x,float y)
{
    score.push_back(*create_rectangle(x,y,lengthx,lengthy,0,0,0,0,0,0));
}
void rectangle5(float x,float y)
{
    score.push_back(*create_rectangle(x-lengthx,y-lengthx,lengthx,lengthy,90,0,0,0,0,0));
}
void rectangle6(float x,float y)
{
    score.push_back(*create_rectangle(x+lengthx,y-lengthx,lengthx,lengthy,90,0,0,0,0,0));
}
void rectangle7(float x,float y)
{
    score.push_back(*create_rectangle(x,y-2*lengthx,lengthx,lengthy,0,0,0,0,0,0));
}
void display_score()
{
  loop(i,0,score.size()) construct(score[i]);
}
void display_digit(int n,float x,float y)
{
  if(n==1) {rectangle2(x,y);rectangle6(x,y);}
  else if(n==2) {rectangle1(x,y);rectangle2(x,y);rectangle4(x,y);rectangle5(x,y);rectangle7(x,y);}
  else if(n==3) {rectangle1(x,y);rectangle2(x,y);rectangle4(x,y);rectangle6(x,y);rectangle7(x,y);}
  else if(n==4) {rectangle2(x,y);rectangle3(x,y);rectangle4(x,y);rectangle6(x,y);}
  else if(n==5) {rectangle1(x,y);rectangle3(x,y);rectangle4(x,y);rectangle6(x,y);rectangle7(x,y);}
  else if(n==6) {rectangle1(x,y);rectangle3(x,y);rectangle4(x,y);rectangle5(x,y);rectangle6(x,y);rectangle7(x,y);}
  else if(n==7) {rectangle1(x,y);rectangle2(x,y);rectangle6(x,y);}
  else if(n==8) {rectangle1(x,y);rectangle2(x,y);rectangle3(x,y);rectangle4(x,y);rectangle5(x,y);rectangle6(x,y);rectangle7(x,y);}
  else if(n==9) {rectangle1(x,y);rectangle2(x,y);rectangle3(x,y);rectangle4(x,y);rectangle6(x,y);rectangle7(x,y);}
  else if(n==0) {rectangle1(x,y);rectangle2(x,y);rectangle3(x,y);rectangle5(x,y);rectangle6(x,y);rectangle7(x,y);}
}
