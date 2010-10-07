#ifndef CONFIGURATION_H_INCLUDED
#define CONFIGURATION_H_INCLUDED

const unsigned int MAX_STR=256;

extern char video_device_1[MAX_STR];
extern char video_device_2[MAX_STR];
extern char rd01_device[MAX_STR];
extern unsigned int fps;
extern unsigned int draw_on;
extern unsigned int web_interface;
extern unsigned int motion_lock_on;

//void Say(char * what2say);
void LoadConfiguration();

#endif // CONFIGURATION_H_INCLUDED
