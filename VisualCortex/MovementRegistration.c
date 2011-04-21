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



unsigned int PatchDistanceCompare_RGB(struct ImageRegion *block_1,struct ImageRegion *block_2,unsigned char *patchdata1,unsigned char *patchdata2,unsigned long max_distance,unsigned long *fast_edges)
{
	//BASELINE RGB PROXIMITY!
    //PAIRNOUME KATHE PIXEL K OSO PIO MEGALI I DIAFORA TOU STO KANALI R G KAI B TOSO MEGALYTERO TO SCORE POU EPISTREFEI I SYNARTISI AYTI!
    unsigned int prox=0,curprox=0;
	fast_edges=0;

	unsigned int comp_size_x=block_1->x2-block_1->x1;
	unsigned int comp_size_x_mult_3=comp_size_x*3;
	unsigned int comp_size_y=block_1->y2-block_1->y1;
	unsigned long ptr_1,ptr_2,ptr_1_end;

	//APOFEYGOUME 4 POLLAPLASIASMOUS ANA Y LOOP
    unsigned int full_rgb_block1_x=block_1->full_x * 3;
    unsigned int full_rgb_block2_x=block_2->full_x * 3;
	unsigned int block1_start_x=block_1->x1 * 3;
	unsigned int block2_start_x=block_2->x1 * 3;
	//APOFEYGOUME 4 POLLAPLASIASMOUS ANA Y LOOP
    unsigned int y;
	for (y=0; y<comp_size_y; y++)
	{
		ptr_1 = ( (y+block_1->y1) * full_rgb_block1_x )+( block1_start_x );
	    //ptr_1 = precalc_memplace_3byte[block1_start_x][y+block_1.y1];

		ptr_2 = ( (y+block_2->y1) * full_rgb_block2_x )+( block2_start_x );
		//ptr_2 = precalc_memplace_3byte[block2_start_x][y+block_2.y1];

		ptr_1_end=ptr_1+comp_size_x_mult_3;

		while (ptr_1<ptr_1_end)
	 	 {
			 curprox=precalc_sub[patchdata1[ptr_1]][patchdata2[ptr_2]];
			 prox+=curprox;
			 //if (curprox>30) { ++fast_edges; }
			 ++ptr_2;
			 ++ptr_1;
		 }
        if ( (max_distance!=0)&&(max_distance<=prox) ) { return prox; }
	}

 return prox;
}


unsigned short TotalPatchesMoving(unsigned char *patchdata_compressed)
{ unsigned short retres=0;
  unsigned int i;
  for ( i=0; i<metrics[GROUP_MOVEMENT_ARRAY_SIZE]; i++)
   {
	   if (patchdata_compressed[i]>=settings[MOVEMENT_PATCH_SENSITIVITY]) { ++retres; }
   }
   return retres;
}

unsigned int PatchHasMovement(struct ImageRegion *block_1,unsigned char *patchdata1,unsigned char *patchdata_compressed)
{
	//EPISTREFEI TRUE AN EXOUME MOTION!
    //PAIRNOUME KATHE PIXEL K AN EINAI DIAFORETIKO APO TO 0 EPISTREFOUME TRUE!

  /*
	unsigned int comp_size_x=block_1.x2-block_1.x1;
	unsigned int comp_size_y=block_1.y2-block_1.y1;
	unsigned int ptr_1,ptr_1_end;

   OLD - EXPENSIVE CHECK!
	for (int y=0; y<comp_size_y; y++)
	{
		ptr_1 = ( (y+block_1.y1) * block_1.full_x )+( block_1.x1 );
	    ptr_1_end=ptr_1+comp_size_x;

		while (ptr_1<ptr_1_end)
	 	 {
			 if (patchdata1[ptr_1]!=0) { return true; }
			++ptr_1;
		 }
	}
  */
  if ((block_1->x1>metrics[RESOLUTION_X])||(block_1->y1>metrics[RESOLUTION_Y])) { fprintf(stderr,"Error PatchHasMovement called with out of bounds x/y"); return 0;}
  //AN TO BLOCK EINAI 30x30 7 Pixel einai sxedon tipota..
  if (patchdata_compressed[precalc_group_block_belong[block_1->x1][block_1->y1]]<settings[MOVEMENT_PATCH_SENSITIVITY])   { return 0; }
  return 1;
}

void ClearGroupMovementArray(char lefteye)
{
  if ( lefteye == 1) { memset(video_register[GROUP_MOVEMENT_LEFT].pixels, 0 , metrics[GROUP_MOVEMENT_ARRAY_SIZE]); } else
                     { memset(video_register[GROUP_MOVEMENT_RIGHT].pixels, 0 , metrics[GROUP_MOVEMENT_ARRAY_SIZE]); }
}




void inline FillCompressedMovementArrayBlock(BOOLEAN lefteye,unsigned int ptr_x)
{   // TO POINTER X (ptr_x) ypotithetai oti einai enas pointer pou ypologizetai ws x*y ( diladi grayscale 8bit/1byte BITMAP )
	unsigned int x,y,ptr_comp_mem;

     if ( metrics[RESOLUTION_X] == 0 ) { fprintf(stderr,"Error FillCompressedMovementArrayBlock called with 0 resolution\n"); }

	 y = ( ptr_x / metrics[RESOLUTION_X] );
     x = ( ptr_x % metrics[RESOLUTION_X] );

	 ptr_comp_mem=precalc_group_block_belong[x][y];
	 if (ptr_comp_mem>=metrics[GROUP_MOVEMENT_ARRAY_SIZE] ) { fprintf(stderr,"Incorrect Reference to ptr_comp_mem from FillCompressedMovementArrayBlock "); return; }
	 if ( lefteye ) { if ( video_register[GROUP_MOVEMENT_LEFT].pixels[ptr_comp_mem] < 254 ) { ++ video_register[GROUP_MOVEMENT_LEFT].pixels[ptr_comp_mem]; }  } else
	                { if ( video_register[GROUP_MOVEMENT_RIGHT].pixels[ptr_comp_mem] < 254 ) { ++ video_register[GROUP_MOVEMENT_RIGHT].pixels[ptr_comp_mem]; } }


}

unsigned int RegisterMovements(BOOLEAN lefteye,unsigned int last_source_block_reg,unsigned int source_block_reg,unsigned int background_block_reg,unsigned int target_difference_reg,unsigned int mobility_array_reg)
{
 unsigned char *last_source_block=video_register[last_source_block_reg].pixels;
 unsigned char *source_block=     video_register[source_block_reg].pixels;
 unsigned char *background_block= video_register[background_block_reg].pixels;
 unsigned char* target_difference=video_register[target_difference_reg].pixels;
 unsigned char *mobility_array=   video_register[mobility_array_reg].pixels;

 ClearGroupMovementArray(lefteye); //AYTO GIA NA FTIAKSOUME ENA COMPRESSED ANTIGRAFO TOU MOVEMENT_LEFT
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
	  case 2 :  /* */ { ++mobility_array[ptr_single_byte]; FillCompressedMovementArrayBlock(lefteye,ptr_single_byte); }  break;
	  case 3 :  /* */ { ++mobility_array[ptr_single_byte]; FillCompressedMovementArrayBlock(lefteye,ptr_single_byte); } break;
	};

    //EXOUME TELEIWSEI ME TIN EPEKSERGASIA TOU PIXEL OPOTE
	//AYKSANOUME KATA 1 TON DEIKTI SE 1BYTE ARRAYS TIS EIKONAS
	++ptr_single_byte;

  }

  video_register[target_difference_reg].depth=1; // Set BitDepth of output array as 1
  video_register[mobility_array_reg].depth=1; // Set BitDepth of output array as 1

  return changes;
}


