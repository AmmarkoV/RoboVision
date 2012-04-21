#include "ObjectRecognition.h"
#include "VisionMemory.h"


double compareSURFDescriptors( const float* d1, const float* d2, double best, int length )
{
    double total_cost = 0;
    if ( length % 4 != 0 ) { return 0.0; }
    int i;
    for( i = 0; i < length; i += 4 )
    {
        double t0 = d1[i  ] - d2[i  ];
        double t1 = d1[i+1] - d2[i+1];
        double t2 = d1[i+2] - d2[i+2];
        double t3 = d1[i+3] - d2[i+3];
        total_cost += t0*t0 + t1*t1 + t2*t2 + t3*t3;
        if( total_cost > best )
            break;
    }
    return total_cost;
}


unsigned int RememberRegisterPart(struct VideoRegister * frame , unsigned int x , unsigned int y , unsigned int width , unsigned int height , char * str , unsigned int length)
{

 return 0;
}
