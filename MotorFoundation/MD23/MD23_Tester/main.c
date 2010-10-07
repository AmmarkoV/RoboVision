#include <stdio.h>
#include <stdlib.h>
#include "../MD23.h"

int main()
{
    printf("MD23 !\n");
    struct md23_device * guard_base=MD23_Init("/dev/ttyUSB0",1);

    if ( guard_base==0 ) {
                           fprintf(stderr,"MD23 Initialization failed\n");
                           return 0;
                         }
    fprintf(stderr,"MD23 Inited\n");


    MD23_ZeroEncoders(guard_base);

    fprintf(stderr,"MoveBothMotorsDifferentDegrees\n");

//    MD23_MoveMotorsDegrees(guard_base,2,20,100);
    MD23_MoveBothMotorsDifferentDegrees(guard_base,15,30,15,15);

   while ( MD23_MovementDone(guard_base)==0 )
    {
     MD23_Wait();
     fprintf(stderr,"Waiting to finish move %f , %f\n",MD23_GetEncoder(guard_base,0),MD23_GetEncoder(guard_base,1));
    }

    int i=0;
    for ( i =0; i<10; i++ )  MD23_Wait();
    fprintf(stderr,"Waiting to finish move %f , %f\n",MD23_GetEncoder(guard_base,0),MD23_GetEncoder(guard_base,1));


    fprintf(stderr,"Close MD23\n");
    MD23_Close(guard_base);
    return 0;
}
