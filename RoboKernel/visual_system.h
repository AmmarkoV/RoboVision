#ifndef VISUAL_SYSTEM_H_INCLUDED
#define VISUAL_SYSTEM_H_INCLUDED

#include "../VideoInput/VideoInput.h"
#include "../VisualCortex/VisualCortex.h"

extern int width,height;

int InitVisualSystem();
int PassVideoInputToCortex(unsigned int clock_time);
int CloseVisualSystem();


#endif // VISUAL_SYSTEM_H_INCLUDED
