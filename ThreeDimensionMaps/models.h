#ifndef MODELS_H_INCLUDED
#define MODELS_H_INCLUDED

void DrawAxis();
void DrawFloor();
void DrawAgent(unsigned int agent_num , float x , float y , float z , float heading , float pitch , float roll);
void DrawEmptyDepthMap(int num,float transx,float transy,float transz,float heading , float pitch , float roll);
void DrawDepthMap(int num,float transx,float transy,float transz,float heading , float pitch , float roll);
void DrawDepthMapOLD(int num,float transx,float transy,float transz,float heading , float pitch , float roll);

#endif // MODELS_H_INCLUDED
