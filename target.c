#include "target.h"

vector<VAO>targets;
void set_targets()
{
  while(targets.size()<5)
  {
    targets.push_back(*create_bubble(rand()%600,-200,30,30,0,1,1,1,0,rand()%5);
  }
}
void draw_targets()
{
  loop(i,0,targets.size()) construct(targets[i]);
}
