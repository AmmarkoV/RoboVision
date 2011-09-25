#ifndef ROBOKERNEL_H_INCLUDED
#define ROBOKERNEL_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned int clock_count;

int IssueCommand(char * cmd,char * res,unsigned int resmaxsize,char * from);

unsigned char * GetVideoRegister(unsigned int num);
unsigned int GetCortexSetting(unsigned int option);
void SetCortexSetting(unsigned int option,unsigned int value);
unsigned int GetCortexMetric(unsigned int option);

int StartRoboKernel();
int StopRoboKernel();

int RoboKernelAlive();

#ifdef __cplusplus
}
#endif

#endif // ROBOKERNEL_H_INCLUDED
