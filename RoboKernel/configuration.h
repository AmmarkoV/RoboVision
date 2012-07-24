#ifndef CONFIGURATION_H_INCLUDED
#define CONFIGURATION_H_INCLUDED

#define MAX_STR 256

extern int system_autonomous;


extern char video_device_1[MAX_STR];
extern char video_device_2[MAX_STR];
extern char joystick_device[MAX_STR];
extern char rd01_device[MAX_STR];
extern char arduino_device[MAX_STR];
extern char user[MAX_STR];
extern char group[MAX_STR];

extern double camera_params_1[12];
extern double camera_params_2[12];

extern unsigned int IRC_Interface_Enabled;

extern char sound_play_command[MAX_STR];

extern char sound_record_command[MAX_STR];
extern char sound_record_parameter[MAX_STR];

extern char ENVDIR[MAX_STR];
extern char INITIALIZATION_CONFIGURATION_PATH[MAX_STR];
extern char SNAPSHOT_PATH[MAX_STR];
extern char WEB_SERVER_ROOT[MAX_STR];
extern char CONSOLE_OUT_PATH[MAX_STR];
extern char COMMANDS_PATH[MAX_STR];
extern char VIEWERS_PATH[MAX_STR];
extern char SENSORS_PATH[MAX_STR];

extern char SMS_SEND_PATH[MAX_STR];
extern char SMS_RECEIVE_PATH[MAX_STR];

extern char CLIPART_PATH[MAX_STR];


extern char tts_app[MAX_STR];
extern char tts_pre_command[MAX_STR];
extern char tts_command[MAX_STR];
extern char tts_command_parameter[MAX_STR];

extern unsigned int fps;
extern unsigned int draw_on;
extern unsigned int web_interface;
extern unsigned int web_interface_snaptime;
extern unsigned int motion_lock_on;
extern unsigned int swap_inputs;

int quickcat(char * outfilename,char *infilename1,char * infilename2);
int filename_stripper_found_attack(char * filename);
void LoadConfiguration();
int RefreshDeviceNumbering();

#endif // CONFIGURATION_H_INCLUDED
