#ifndef COMMAND_HAL_H_INCLUDED
#define COMMAND_HAL_H_INCLUDED

int IssueCommand(char * command);
int WriteConsoleOutput(char * outstr);
int EraseConsoleOutput();

#endif // COMMAND_HAL_H_INCLUDED
