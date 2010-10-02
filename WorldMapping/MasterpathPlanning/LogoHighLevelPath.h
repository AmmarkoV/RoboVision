#ifndef LOGOHIGHLEVELPATH_H_INCLUDED
#define LOGOHIGHLEVELPATH_H_INCLUDED

#include "NormalizePath.h"
unsigned int ConvertPathToLogo(TraceNode * str8nodes,unsigned int &str8nodes_size,char * storage,unsigned int storagelen);
int PrintoutHTML(char * filename,unsigned int world_x,unsigned int world_y,NodeData * world);

#endif // LOGOHIGHLEVELPATH_H_INCLUDED
