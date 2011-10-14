#ifndef NETWORK_FRAMEWORK_H_INCLUDED
#define NETWORK_FRAMEWORK_H_INCLUDED

extern int network_receive_stop;
extern int network_transmit_stop;
extern char * peer_feed;

int StartupNetworkServer();
int StartupNetworkClient(char * ip,unsigned int port);

#endif // NETWORK_FRAMEWORK_H_INCLUDED
