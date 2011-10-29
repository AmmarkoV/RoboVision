#ifndef MASTERWORLD_H_INCLUDED
#define MASTERWORLD_H_INCLUDED

struct Points
{
   unsigned char r,g,b;
   unsigned char exists;
   unsigned char size;
   float x,y,z;
};


struct Agents
{
   float x,y,z;
   float heading , float pitch , float yaw;
};



#endif // MASTERWORLD_H_INCLUDED
