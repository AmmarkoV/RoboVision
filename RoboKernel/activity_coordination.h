#ifndef ACTIVITY_COORDINATION_H_INCLUDED
#define ACTIVITY_COORDINATION_H_INCLUDED

enum ActivityStatus
{
  IDLE = 0,
  LEARNING_THE_WORLD,
  GUARDING,

  RETURNING_TO_CHARGE_BAY,
  CHARGING,

  FACE_TRACKING,
  TALKING_TO_PERSON,
  PLAYING_A_GAME,



  POSSIBLE_ACTIVITY_STATUSES
};

extern int current_activity;

int Danger();
int Safe();


int EngageActivity(int activity_num);
int StopCurrentActivity();
int ActivityLoop();


#endif // ACTIVITY_COORDINATION_H_INCLUDED
