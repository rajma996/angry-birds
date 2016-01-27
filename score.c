#include "score.h"

vector<VAO> score;
vector< VAO > rul;
vector < vector<VAO> > final_rul;
float lengthx = 10;
float lengthy = 2.5;
void rectangle1(float x,float y,int state)
{
  if(state==1)
    score.push_back(*create_rectangle(x,y+2*lengthx,lengthx,lengthy,0,0,0,0,0,0));
  else
    { ;rul.push_back(*create_rectangle(x,y+2*lengthx,lengthx,lengthy,0,0,0,0,0,0)); }

}
void rectangle2(float x,float y,int state)
{
  if(state==1)
    score.push_back(*create_rectangle(x+lengthx,y+lengthx,lengthx,lengthy,90,0,0,0,0,0));
  else
    { rul.push_back(*create_rectangle(x+lengthx,y+lengthx,lengthx,lengthy,90,0,0,0,0,0));}

}
void rectangle3(float x,float y,int state)
{
  if(state==1)
    score.push_back(*create_rectangle(x-lengthx,y+lengthx,lengthx,lengthy,90,0,0,0,0,0));
  else
    {rul.push_back(*create_rectangle(x-lengthx,y+lengthx,lengthx,lengthy,90,0,0,0,0,0));}
}
void rectangle4(float x,float y,int state)
{
  if(state==1)
    score.push_back(*create_rectangle(x,y,lengthx,lengthy,0,0,0,0,0,0));
  else
    {rul.push_back(*create_rectangle(x,y,lengthx,lengthy,0,0,0,0,0,0));}
}
void rectangle5(float x,float y,int state)
{
  if(state==1)
    score.push_back(*create_rectangle(x-lengthx,y-lengthx,lengthx,lengthy,90,0,0,0,0,0));
  else
    {rul.push_back(*create_rectangle(x-lengthx,y-lengthx,lengthx,lengthy,90,0,0,0,0,0));}
}
void rectangle6(float x,float y,int state)
{
  if(state==1)
    score.push_back(*create_rectangle(x+lengthx,y-lengthx,lengthx,lengthy,90,0,0,0,0,0));
  else
    { rul.push_back(*create_rectangle(x+lengthx,y-lengthx,lengthx,lengthy,90,0,0,0,0,0));}
}
void rectangle7(float x,float y,int state)
{
  if(state==1)
    score.push_back(*create_rectangle(x,y-2*lengthx,lengthx,lengthy,0,0,0,0,0,0));
  else
    { rul.push_back(*create_rectangle(x,y-2*lengthx,lengthx,lengthy,0,0,0,0,0,0));}
}
void display_score()
{
  loop(i,0,score.size()) construct(score[i]);
}
void display_digit(int n,float x,float y)
{
  if(n==1) {rectangle2(x,y,1);rectangle6(x,y,1);}
  else if(n==2) {rectangle1(x,y,1);rectangle2(x,y,1);rectangle4(x,y,1);rectangle5(x,y,1);rectangle7(x,y,1);}
  else if(n==3) {rectangle1(x,y,1);rectangle2(x,y,1);rectangle4(x,y,1);rectangle6(x,y,1);rectangle7(x,y,1);}
  else if(n==4) {rectangle2(x,y,1);rectangle3(x,y,1);rectangle4(x,y,1);rectangle6(x,y,1);}
  else if(n==5) {rectangle1(x,y,1);rectangle3(x,y,1);rectangle4(x,y,1);rectangle6(x,y,1);rectangle7(x,y,1);}
  else if(n==6) {rectangle1(x,y,1);rectangle3(x,y,1);rectangle4(x,y,1);rectangle5(x,y,1);rectangle6(x,y,1);rectangle7(x,y,1);}
  else if(n==7) {rectangle1(x,y,1);rectangle2(x,y,1);rectangle6(x,y,1);}
  else if(n==8) {rectangle1(x,y,1);rectangle2(x,y,1);rectangle3(x,y,1);rectangle4(x,y,1);rectangle5(x,y,1);rectangle6(x,y,1);rectangle7(x,y,1);}
  else if(n==0) {rectangle1(x,y,1);rectangle2(x,y,1);rectangle3(x,y,1);rectangle5(x,y,1);rectangle6(x,y,1);rectangle7(x,y,1);}
  else if(n==9) {rectangle1(x,y,1);rectangle2(x,y,1);rectangle3(x,y,1);rectangle4(x,y,1);rectangle6(x,y,1);rectangle7(x,y,1);}
}
void display_rules()
{
  loop(i,0,rul.size()) construct(rul[i]);
}
void set_rul(string a,float x,float y)
{

  loop(i,0,a.size())
  {
    if(a[i]=='1') { rectangle1(x,y,2); }
    else if(a[i]=='2'){  rectangle2(x,y,2);}
    else if(a[i]=='3'){  rectangle3(x,y,2);}
    else if(a[i]=='4'){ rectangle4(x,y,2);}
    else if(a[i]=='5'){ rectangle5(x,y,2);}
    else if(a[i]=='6'){ rectangle6(x,y,2);}
    else if(a[i]=='7'){ rectangle7(x,y,2);}
  }
}
void display_alphabet(char a,float x,float y)
{
  // printf("%c\n",a );
  x=x-ortho_x_shift;
  y=y-ortho_y_shift;
  if(a=='a') {string temp = "123456"; set_rul(temp,x,y); }
  if(a=='b') {string temp = "34567"; set_rul(temp,x,y); }
  if(a=='c') {string temp = "1357"; set_rul(temp,x,y); }
  if(a=='d') {string temp = "24567"; set_rul(temp,x,y); }
  if(a=='e') {string temp = "13457"; set_rul(temp,x,y); }
  if(a=='f') {string temp = "1345"; set_rul(temp,x,y); }
  if(a=='g') {string temp = "13567"; set_rul(temp,x,y); rul.push_back(*create_rectangle(x+lengthx/2,y,lengthx/2,lengthy,0,0,0,0,0,0));}
  if(a=='h') {string temp = "32456"; set_rul(temp,x,y); }
  if(a=='i') {string temp = "17"; set_rul(temp,x,y); rul.push_back(*create_rectangle(x,y,2*lengthx,lengthy,90,0,0,0,0,0));}
  if(a=='j') {string temp = "267"; set_rul(temp,x,y); }
  if(a=='k') {string temp = ""; set_rul(temp,x,y); }
  if(a=='l') {string temp = "357"; set_rul(temp,x,y); }
  if(a=='m') {string temp = ""; set_rul(temp,x,y); }
  if(a=='n') {string temp = "2356"; set_rul(temp,x,y); rul.push_back(*create_rectangle(x,y,2*lengthx,lengthy,135,0,0,0,0,0));  }
  if(a=='o') {string temp = "123567"; set_rul(temp,x,y); }
  if(a=='p') {string temp = "12345"; set_rul(temp,x,y); }
  if(a=='q') {string temp = ""; set_rul(temp,x,y); }
  if(a=='r') {string temp = "12345"; set_rul(temp,x,y); rul.push_back(*create_rectangle(x,y-lengthx,lengthx,lengthy,135,0,0,0,0,0));}
  if(a=='s') {string temp = "13467"; set_rul(temp,x,y); }
  if(a=='t') {string temp = "1"; set_rul(temp,x,y); rul.push_back(*create_rectangle(x,y,2*lengthx,lengthy,90,0,0,0,0,0));}
  if(a=='u') {string temp = "23567"; set_rul(temp,x,y); }
  if(a=='v') {string temp = ""; set_rul(temp,x,y); }
  if(a=='w') {string temp = ""; set_rul(temp,x,y); }
  if(a=='x') {string temp = ""; set_rul(temp,x,y); rul.push_back(*create_rectangle(x,y,2.23*lengthx,lengthy,45,0,0,0,0,0)); rul.push_back(*create_rectangle(x,y,2.23*lengthx,lengthy,135,0,0,0,0,0));}
  if(a=='y') {string temp = ""; set_rul(temp,x,y); rul.push_back(*create_rectangle(x+lengthx/2,y+lengthx/2,lengthx*1.1,lengthy,45,0,0,0,0,0));rul.push_back(*create_rectangle(x-lengthx/2,y+lengthx/2,lengthx*1.1,lengthy,135,0,0,0,0,0)); rul.push_back(*create_rectangle(x,y-lengthx/2,lengthx,lengthy,90,0,0,0,0,0)); }
  if(a=='z') {string temp = ""; set_rul(temp,x,y); }
}
