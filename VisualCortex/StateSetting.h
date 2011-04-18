#ifndef STATESETTING_H_INCLUDED
#define STATESETTING_H_INCLUDED

#include "StateSetting.h"


unsigned int SetCamerasGeometry(float distance_between_cameras,float diagonal_field_of_view,float horizontal_field_of_view,float vertical_field_of_view);
unsigned int InitializeEverything(unsigned int res_x,unsigned int res_y);
unsigned int CloseEverything();
unsigned int SelfCheckEverything();
void SetSetting(unsigned int set_num,unsigned int set_val);
void SetDefaultSettings();
unsigned int GetSetting(unsigned int get_num);
unsigned int GetMetric(unsigned int get_num);
unsigned int GetVideoRegisterStats(unsigned int metric_num);
void SetTime(unsigned int thetime);
unsigned int GetTime();

#endif // STATESETTING_H_INCLUDED
