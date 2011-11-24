#include "FramesOSD.h"


int FeaturesAquired(unsigned int video_reg_num)
{
  if ( VisCortx_GetSetting(PASS_TO_FEATURE_DETECTOR)==0 )
   {
       //Features will never be aquired because they are disabled.. :P
       return 0;
   }

   /*
  if ( VisCortx_GetFeature(video_reg_num,0,TOTAL_POINTS)==0 )
   {
       fprintf(stderr,"TODO :No Features detected,VisCortex must keep last features or make new\n");
       //Features will never be aquired because there are no features :P
       return 0;
   }*/

 unsigned int wait_time = 0 , max_wait_time = 800;
     while ( (wait_time<max_wait_time) && ( VisCortx_GetFeature(video_reg_num,0,FEATURE_LAST_TRACK_TIME ) != VisCortx_GetVideoRegisterData(video_reg_num,0 /*TODO ADD AN ENUM*/) ) )
      {
         ++wait_time;
         wxMilliSleep(1);
      }
      if (wait_time>=max_wait_time)
      {
            fprintf(stderr,"Features for video_register ( %u ) timedOut , will not be drawn \n",video_reg_num);
            return 0;
      }
  return 1;
}

int DrawFeaturesAtFeed(wxPaintDC & dc , unsigned int x , unsigned int y , unsigned int video_reg_num , unsigned int point , unsigned int line)
{
if  (VisCortx_GetFeature(video_reg_num,0,TOTAL_POINTS)>0)
      {
        //------------------------------------ FEATURES ---------------------------------------------*/
        /*We want to draw features on left eye ---------------------------------------------------------------------------------*/
       if (VisCortx_GetFeature(video_reg_num,0,TOTAL_POINTS)>0  )
       {
        unsigned int i;
        for ( i=0; i<VisCortx_GetFeature(video_reg_num,0,TOTAL_POINTS); i++ )
         {
            if ( VisCortx_GetFeature(video_reg_num,i,FEATURE_IS_LOST ) == 0 )
            {
             if (point)
               {
                 dc.DrawRectangle(x+VisCortx_GetFeature(video_reg_num,i,FEATURE_X),y+VisCortx_GetFeature(video_reg_num,i,FEATURE_Y),3,3);
               }
              if (line)
               {
                 dc.DrawLine(x+VisCortx_GetFeature(video_reg_num,i,FEATURE_X),y+VisCortx_GetFeature(video_reg_num,i,FEATURE_Y),
                             x+VisCortx_GetFeature(video_reg_num,i,FEATURE_LAST_X),y+VisCortx_GetFeature(video_reg_num,i,FEATURE_LAST_Y));
               }
            }
         }
       }
     }
  return 1;
}



int DrawFacesAtFeed(wxPaintDC & dc , unsigned int x , unsigned int y , unsigned int video_reg_num )
{
       if (VisCortx_GetFaces(video_reg_num,0,TOTAL_POINTS)>0  )
       {
         unsigned int center_x,center_y,scale,i;
         for ( i=0; i<VisCortx_GetFaces(video_reg_num,0,TOTAL_POINTS); i++ )
         {
            if ( VisCortx_GetFaces(video_reg_num,i,FEATURE_IS_LOST) == 0 )
            {
               center_x=VisCortx_GetFaces(video_reg_num,i,FEATURE_X) + ( VisCortx_GetFaces(video_reg_num,i,FEATURE_DIM_X) / 2 ) ;
               center_y=VisCortx_GetFaces(video_reg_num,i,FEATURE_Y) + ( VisCortx_GetFaces(video_reg_num,i,FEATURE_DIM_Y) / 2 ) ;
               scale = ( ( VisCortx_GetFaces(video_reg_num,i,FEATURE_DIM_X) / 2 ) + ( VisCortx_GetFaces(video_reg_num,i,FEATURE_DIM_Y) / 2 ) ) / 2 ;
               dc.DrawCircle( x + center_x , y + center_y , scale );

            }
         }
         return 1;
       }
   return 0;
}

