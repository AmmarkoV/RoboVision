/*
The Visual Cortex of guarddog
URLs: http://ammarkov.ath.cx
Written by Ammar Qammaz a.k.a. AmmarkoV 2010

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "MovementRegistration.h"
#include "Precalculations.h"
#include "IntegralImageConversion.h"



unsigned int GetMovementAtBlock(unsigned int movement,struct ImageRegion * source_rgn)
{
   return GetCompressedRegisterPatchSum1Byte( movement, source_rgn->x1 , source_rgn->y1 , source_rgn->width , source_rgn->height );
}


unsigned int RegisterMovements(BOOLEAN lefteye,unsigned int last_source_block_reg,unsigned int source_block_reg,unsigned int movement_target,unsigned int movement_target_grouped)
{
 //unsigned char *last_source_block=;
// unsigned char *source_block=     ;
// unsigned char *background_block= video_register[background_block_reg].pixels;

 unsigned int  changes=0,last_changes = 0; //Tha metrisei athroistika posa pixels allaksan

  unsigned char * lfsptr=video_register[last_source_block_reg].pixels; // Last FrameSource Pointer
  unsigned char * ptr=video_register[source_block_reg].pixels; // Source Pointer
  unsigned char * limit_size = ptr + metrics[RESOLUTION_MEMORY_LIMIT_3BYTE];
  unsigned char * movement_ptr = video_register[movement_target].pixels;

  unsigned char diff;
  ClearVideoRegister(movement_target);

  while (ptr<limit_size)
  {
      last_changes = changes;


      diff=AbsUCharDiff(ptr,lfsptr);
      if (diff>settings[MOVEMENT_R_THRESHOLD]) { changes+=2; }  else if (diff>settings[MOVEMENT_MIN_R_THRESHOLD]) { changes++; }
      ++ptr; ++lfsptr;

      diff=AbsUCharDiff(ptr,lfsptr);
      if (diff>settings[MOVEMENT_G_THRESHOLD]) { changes+=2; }  else if (diff>settings[MOVEMENT_MIN_G_THRESHOLD]) { changes++; }
      ++ptr; ++lfsptr;

      diff=AbsUCharDiff(ptr,lfsptr);
      if (diff>settings[MOVEMENT_B_THRESHOLD]) { changes+=2; }  else if (diff>settings[MOVEMENT_MIN_B_THRESHOLD]) { changes++; }
      ++ptr; ++lfsptr;

      switch ( changes - last_changes )
	  {
	     case 0 :  /*SHOULD BE MOVEMENT*/   break;
	     case 1 :  /* CAMERA NOISE */   break;
	    // case 2 :  /* */ {  *movement_ptr=50;  }  break;
	     case 3 :  /* */ {  *movement_ptr=1;}  break;
	     case 4 :  /* */ {  *movement_ptr=2;}  break;
	     case 5 :  /* */ {  *movement_ptr=3;}  break;
	     case 6 :  /* */ {  *movement_ptr=4;}  break;
	  };

      ++movement_ptr;
  }

//CompressRegister(movement_target,movement_target_grouped);
video_register[movement_target].depth=1; // Set BitDepth of output array as 1

 return changes;
}

