#ifndef ROBOKERNEL_H_INCLUDED
#define ROBOKERNEL_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif


int IssueCommand(unsigned char * cmd,unsigned char * res,unsigned int resmaxsize,unsigned char * from);
unsigned char * GetVideoRegister(unsigned int num);

void StartRoboKernel();
void StopRoboKernel();

int RoboKernelAlive();

#ifdef __cplusplus
}
#endif

#endif // ROBOKERNEL_H_INCLUDED
