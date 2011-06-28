#ifndef PRECALCULATIONS_H_INCLUDED
#define PRECALCULATIONS_H_INCLUDED

extern unsigned int resection_left_precalc[321*241*3];
extern unsigned int resection_right_precalc[321*241*3];
extern unsigned char precalc_med[256][256];
extern unsigned char precalc_sub[256][256];
extern unsigned int precalc_group_block_belong[641][481];
extern unsigned long precalc_memplace_3byte[641][481];
extern unsigned long precalc_memplace_1byte[641][481];

unsigned int RGB(unsigned char R,unsigned char G,unsigned char B);
unsigned int PrecalcResectioning(unsigned int * frame ,  double fx,double fy , double cx,double cy ,
                                                         double k1,double k2 , double p1,double p2 , double k3   );
void Precalculations();

#endif // PRECALCULATIONS_H_INCLUDED
