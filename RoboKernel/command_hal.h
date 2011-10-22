#ifndef COMMAND_HAL_H_INCLUDED
#define COMMAND_HAL_H_INCLUDED

extern int keep_snapshots;
extern int last_snapshot_activation;
extern int snapshot_activation_interval;

int IssueCommandInternal(char * command,char * from,char* outptstr,unsigned int output_length);


#endif // COMMAND_HAL_H_INCLUDED
