#ifndef PATHPLANNINGCORE_H_INCLUDED
#define PATHPLANNINGCORE_H_INCLUDED

#include "MasterpathPlanning.h"

unsigned int AddSensorDataToMap(struct Map * themap,unsigned int agentnum,int ultrasonic_left_cm,int ultrasonic_right_cm);

int PathPlanCore_CleanMap(struct Map * themap);
void FillInTurningOverheads();
int PathPlanCore_FindPath(struct Map * themap,struct Path * theroute,unsigned int x1,unsigned int y1,unsigned int start_direction,unsigned int oursize,unsigned int x2,unsigned int y2,unsigned int timelimit_ms);

#endif // PATHPLANNINGCORE_H_INCLUDED
