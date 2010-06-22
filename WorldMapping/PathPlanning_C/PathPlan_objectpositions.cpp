#include "PathPlan_objectpositions.h"

struct WorldObject
{
   float posx,posy,posz;
   unsigned int place_in;
   unsigned int facing_direction;
};

WorldObject objs[10];

void SetWorldObjectPos(unsigned int objnum,float posx,float posy,float posz)
{
  objs[objnum].posx=posx;
  objs[objnum].posy=posy;
  objs[objnum].posz=posz; 
}

void GetWorldObjectPos(unsigned int objnum,float &posx,float &posy,float &posz)
{
  posx=objs[objnum].posx;
  posy=objs[objnum].posy;
  posz=objs[objnum].posz; 
}

void AdditiveMovement(unsigned int sample_time_in_ms,unsigned int encoder_1_add,unsigned int encoder_2_add)
{
 /*float rot_rad=( (angle_y) * 3.14159265 ) /180;  
            pos_z-=Get_Max_Velocity(0)*cos(rot_rad);
            pos_x-=Get_Max_Velocity(0)*sin(rot_rad);      
            angle_z= 1.5*sin(pos_x+pos_z);    */
}


unsigned int CalculateWheelDegreeTurnForAxelTurn(unsigned int axel_turn,unsigned int wheel_diameter_in_mm,unsigned int axel_length_in_mm)
{
	if ( wheel_diameter_in_mm == 0 ) { return 0; } 
    return ( ( axel_turn * axel_length_in_mm ) / ( wheel_diameter_in_mm ) );
}