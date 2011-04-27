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




unsigned int RegisterMovementsOLD(BOOLEAN lefteye,unsigned int last_source_block_reg,unsigned int source_block_reg,unsigned int background_block_reg,unsigned int target_difference_reg,unsigned int mobility_array_reg)
{
 unsigned char *last_source_block=video_register[last_source_block_reg].pixels;
 unsigned char *source_block=     video_register[source_block_reg].pixels;
 unsigned char *background_block= video_register[background_block_reg].pixels;
 unsigned char* target_difference=video_register[target_difference_reg].pixels;
 unsigned char *mobility_array=   video_register[mobility_array_reg].pixels;

// ClearGroupMovementArray(lefteye); //AYTO GIA NA FTIAKSOUME ENA COMPRESSED ANTIGRAFO TOU MOVEMENT_LEFT
 if ( (last_source_block==0)||(source_block==0)||(background_block==0)||(target_difference==0)||(mobility_array==0)) { return 0; }

 register unsigned int ptr_single_byte=0,ptr_x=0,str_ptr_x=0,cola,colb;
 register unsigned int diff1,diff2,diff3,changes,last_changes;
 unsigned int limit_size=metrics[RESOLUTION_X]*metrics[RESOLUTION_Y]*3;
 changes=0; //Tha metrisei athroistika posa pixels allaksan

 while (ptr_x<limit_size)
  {
    last_changes=changes; // changes-last_changes = 0

	// ANTI NA KANW MIA (AKRIVI) DIAIRESI TON AYKSANW MIA FORA STO TELOS!
      // ptr_single_byte=(int) ptr_x / 3; // O pointer gia single byte arrays
    // ANTI NA KANW MIA (AKRIVI) DIAIRESI TON AYKSANW MIA FORA STO TELOS!

	//Arxizoume apo ena R pixel
	str_ptr_x=ptr_x; cola=source_block[ptr_x]; colb=last_source_block[ptr_x];
	diff1=precalc_sub[cola] [colb];
	last_source_block[ptr_x]=precalc_med[cola] [colb]; // Metavaloume to last_source
    ptr_x++;

	//Pame se ena G pixel
    cola=source_block[ptr_x]; colb=last_source_block[ptr_x];
	diff2=precalc_sub[ cola ] [ colb ];
    last_source_block[ptr_x]=precalc_med[ cola ] [ colb ]; // Metavaloume to last_source
    ptr_x++;

	//Kai telos ena B pixel
	cola=source_block[ptr_x]; colb=last_source_block[ptr_x];
	diff3=precalc_sub[ cola ] [ colb ];
    last_source_block[ptr_x]=precalc_med[ cola ] [ colb ]; // Metavaloume to last_source
	ptr_x++; //Epomeno Pixel R

	if (diff1>settings[MOVEMENT_R_THRESHOLD]) { changes+=2; }  else if (diff1>settings[MOVEMENT_MIN_R_THRESHOLD]) { changes++; }
	if (diff2>settings[MOVEMENT_G_THRESHOLD]) { changes+=2; }  else if (diff2>settings[MOVEMENT_MIN_G_THRESHOLD]) { changes++; }
	if (diff3>settings[MOVEMENT_B_THRESHOLD]) { changes+=2; }  else if (diff3>settings[MOVEMENT_MIN_B_THRESHOLD]) { changes++; }

	target_difference[ptr_single_byte]=changes-last_changes; // To pixel exei status oso einai to changes - last_changes
	if ( changes-last_changes>3 ) { changes=last_changes+3; }

	switch ( changes-last_changes )
	{
	case 0 :  //An dn exoume kinisi to teleytaio einai to background pixel
			  if (mobility_array[ptr_single_byte]<=1){ background_block[str_ptr_x] = source_block[str_ptr_x];
                                                       background_block[str_ptr_x+1] = source_block[str_ptr_x+1];
                                                       background_block[str_ptr_x+2] = source_block[str_ptr_x+2];
													 }
		      if (mobility_array[ptr_single_byte]>0) { --mobility_array[ptr_single_byte]; }
		      break;
	  case 1 :  /* CAMERA NOISE */   break;
//	  case 2 :  /* */ { ++mobility_array[ptr_single_byte]; FillCompressedMovementArrayBlock(lefteye,ptr_single_byte); }  break;
//	  case 3 :  /* */ { ++mobility_array[ptr_single_byte]; FillCompressedMovementArrayBlock(lefteye,ptr_single_byte); } break;
	};

    //EXOUME TELEIWSEI ME TIN EPEKSERGASIA TOU PIXEL OPOTE
	//AYKSANOUME KATA 1 TON DEIKTI SE 1BYTE ARRAYS TIS EIKONAS
	++ptr_single_byte;

  }

  video_register[target_difference_reg].depth=1; // Set BitDepth of output array as 1
  video_register[mobility_array_reg].depth=1; // Set BitDepth of output array as 1

  return changes;
}


unsigned int RegisterMovements(BOOLEAN lefteye,unsigned int last_source_block_reg,unsigned int source_block_reg,unsigned int background_block_reg,unsigned int movement_target)
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

      diff=precalc_sub[*ptr] [*lfsptr];
      if (diff>settings[MOVEMENT_R_THRESHOLD]) { changes+=2; }  else if (diff>settings[MOVEMENT_MIN_R_THRESHOLD]) { changes++; }
      ++ptr; ++lfsptr;

      diff=precalc_sub[*ptr] [*lfsptr];
      if (diff>settings[MOVEMENT_G_THRESHOLD]) { changes+=2; }  else if (diff>settings[MOVEMENT_MIN_G_THRESHOLD]) { changes++; }
      ++ptr; ++lfsptr;

      diff=precalc_sub[*ptr] [*lfsptr];
      if (diff>settings[MOVEMENT_B_THRESHOLD]) { changes+=2; }  else if (diff>settings[MOVEMENT_MIN_B_THRESHOLD]) { changes++; }
      ++ptr; ++lfsptr;

      switch ( changes - last_changes )
	  {
	     case 0 :  /*SHOULD BE MOVEMENT*/   break;
	     case 1 :  /* CAMERA NOISE */   break;
	    // case 2 :  /* */ {  *movement_ptr=50;  }  break;
	     case 3 :  /* */ {  *movement_ptr=100;}  break;
	     case 4 :  /* */ {  *movement_ptr=150;}  break;
	     case 5 :  /* */ {  *movement_ptr=200;}  break;
	     case 6 :  /* */ {  *movement_ptr=255;}  break;
	  };

      ++movement_ptr;
  }

video_register[movement_target].depth=1; // Set BitDepth of output array as 1

 return changes;
}

