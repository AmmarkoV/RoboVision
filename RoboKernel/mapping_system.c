#include "mapping_system.h"
#include <stdio.h>
#include <stdlib.h>
#include "../WorldMapping/MasterpathPlanning/MasterpathPlanning.h"
#include "../MotorFoundation/MotorHAL.h"
#include "../VisualCortex/VisualCortex.h"

int PassDepthToMap()
{
   fprintf(stderr,"PassDepthToMap function is a stub \n");

   struct Map * world = RobotGetMapPointer();

   if  ( world == 0 )  { return 0; }

   AddObstacleViewedbyAgent(world,0,-30,-20,VisCortx_DepthUnitsToCM(VisCortx_GetDepth_From_Angle(0,-30,-20)));
   AddObstacleViewedbyAgent(world,0,-20,-20,VisCortx_DepthUnitsToCM(VisCortx_GetDepth_From_Angle(0,-20,-20)));
   AddObstacleViewedbyAgent(world,0,-10,-20,VisCortx_DepthUnitsToCM(VisCortx_GetDepth_From_Angle(0,-10,-20)));

   AddObstacleViewedbyAgent(world,0,0,-20,VisCortx_DepthUnitsToCM(VisCortx_GetDepth_From_Angle(0,0,-20)));

   AddObstacleViewedbyAgent(world,0,10,-20,VisCortx_DepthUnitsToCM(VisCortx_GetDepth_From_Angle(0,10,-20)));
   AddObstacleViewedbyAgent(world,0,20,-20,VisCortx_DepthUnitsToCM(VisCortx_GetDepth_From_Angle(0,20,-20)));
   AddObstacleViewedbyAgent(world,0,30,-20,VisCortx_DepthUnitsToCM(VisCortx_GetDepth_From_Angle(0,30,-20)));

   return 1;
}
