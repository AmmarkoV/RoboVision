#ifndef CONFIGURATION_H_INCLUDED
#define CONFIGURATION_H_INCLUDED

#define MAX_STR 256

extern char video_device_1[MAX_STR];
extern char video_device_2[MAX_STR];
extern char rd01_device[MAX_STR];
extern char arduino_device[MAX_STR];
extern char user[MAX_STR];
extern char group[MAX_STR];
extern char parentdir[MAX_STR];
extern double camera_params_1[12];
extern double camera_params_2[12];


extern unsigned int fps;
extern unsigned int draw_on;
extern unsigned int web_interface;
extern unsigned int web_interface_snaptime;
extern unsigned int motion_lock_on;
extern unsigned int swap_inputs;

void LoadConfiguration();
int RefreshDeviceNumbering();

#endif // CONFIGURATION_H_INCLUDED
