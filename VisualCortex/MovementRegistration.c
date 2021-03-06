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
#include "VisCortexTimer.h"
#include "IntegralImageConversion.h"



unsigned int GetMovementAtBlock(unsigned int movement,struct ImageRegion * source_rgn)
{
   return GetCompressedRegisterPatchSum1Byte( movement, source_rgn->x1 , source_rgn->y1 , source_rgn->width , source_rgn->height );
}



unsigned int PrepareRegisterMovementsRegisters(unsigned char left_eye)
{
  // if left_eye AND CheckRegistersForSynchronization(CALIBRATED_LEFT,MPLA MPLA ) mpla mpla
  return 0;
}

unsigned int RegisterMovementsInternal(unsigned char lefteye,unsigned int last_reg,unsigned int current_reg,unsigned int movement_target,unsigned int generate_grouped,unsigned int movement_target_grouped)
{
 //unsigned char *last_source_block=;
// unsigned char *source_block=     ;
// unsigned char *background_block= video_register[background_block_reg].pixels;
  StartTimer(MOVEMENT_RAW_DELAY);

 unsigned int  changes=0,last_changes = 0; //Tha metrisei athroistika posa pixels allaksan

  unsigned char * lfsptr=video_register[last_reg].pixels; // Last FrameSource Pointer
  unsigned char * ptr=video_register[current_reg].pixels; // Source Pointer
  unsigned char * limit_size = ptr + metrics[RESOLUTION_MEMORY_LIMIT_3BYTE];
  unsigned char * movement_ptr = video_register[movement_target].pixels;

  unsigned char diff;
  ClearVideoRegister(&video_register[movement_target]);

  while (ptr<limit_size)
  {
      last_changes = changes;


      diff=ABSDIFF(*ptr,*lfsptr);
      if (diff>settings[MOVEMENT_R_THRESHOLD]) { changes+=2; }  else if (diff>settings[MOVEMENT_MIN_R_THRESHOLD]) { changes++; }
      ++ptr; ++lfsptr;

      diff=ABSDIFF(*ptr,*lfsptr);
      if (diff>settings[MOVEMENT_G_THRESHOLD]) { changes+=2; }  else if (diff>settings[MOVEMENT_MIN_G_THRESHOLD]) { changes++; }
      ++ptr; ++lfsptr;

      diff=ABSDIFF(*ptr,*lfsptr);
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

CompressRegister(&video_register[movement_target],&xl_video_register[movement_target_grouped]);
video_register[movement_target].depth=1; // Set BitDepth of output array as 1

 EndTimer(MOVEMENT_RAW_DELAY);
 return changes;
}



unsigned int RegisterMovements(unsigned char left_eye)
{
  //TODO
  PrepareRegisterMovementsRegisters(left_eye);
  //RegisterMovementsInternal(BOOLEAN lefteye,unsigned int last_source_block_reg,unsigned int source_block_reg,unsigned int movement_target,unsigned int movement_target_grouped);
  return 0;
}



int IfMovementRegistrationEnabledAndOverOrDisabled(int number_over)
{
   if  ( (settings[CALCULATE_MOVEMENT_FLOW]) && ( ( metrics[CHANGES_LEFT]>number_over)||( metrics[CHANGES_RIGHT]>number_over) ) ) { return 1; }
   if  (!settings[CALCULATE_MOVEMENT_FLOW]) { return 1; }

   return 0;
}

int MoveRegEnabledAndNoMovement(int number_over)
{
    return (!IfMovementRegistrationEnabledAndOverOrDisabled(number_over));
}
