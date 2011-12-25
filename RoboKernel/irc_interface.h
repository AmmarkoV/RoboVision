#ifndef IRC_INTERFACE_H_INCLUDED
#define IRC_INTERFACE_H_INCLUDED

int IRCSay(char * what2say);

int StartIRCInterface();
int StopIRCInterface();
int IRCInterfaceStopped();

#endif // IRC_INTERFACE_H_INCLUDED
