#ifndef INFRARED_H_INCLUDED
#define INFRARED_H_INCLUDED
 
 int listenForIR(void);
void printpulses(void);
void SendCode(int numpulses,int Signal[]);
void SendLastCode();
int IRcompare(int numpulses, int Signal[]);

#endif  
