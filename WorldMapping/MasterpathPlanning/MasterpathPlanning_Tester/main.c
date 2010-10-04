#include <stdio.h>
#include <stdlib.h>
#include "../MasterpathPlanning.h"


int main()
{
    printf("MasterpathPlanning tester commencing!!\n");
    struct Map * testmap = CreateMap(1000,1000,100);

    unsigned int x=0,y=0;

    printf("State Cleaning Test!!\n");
    for ( y=0; y<1000; y++)
     {
       for ( x=0; x<1000; x++)
       {
           if ( ObstacleExists(testmap,x,y) ) { fprintf(stderr,"Bug , world should be automatically cleared when created!\n"); }
       }
     }


    printf("Obstacle Setting Test part 1/2!!\n");
    for ( y=0; y<1000; y++)
     {
       for ( x=0; x<1000; x++)
       {
           if ( !SetObstacle(testmap,x,y,2) ) { fprintf(stderr,"Bug , could not set obstacle !\n"); }
       }
     }

     printf("Obstacle Setting Test part 2/2!!\n");
    for ( y=0; y<1000; y++)
     {
       for ( x=0; x<1000; x++)
       {
           if ( !ObstacleExists(testmap,x,y) ) { fprintf(stderr,"Bug , world should be completely filled with obstacles !\n"); }
       }
     }

    ClearMap(testmap);


    if ( !FindSponteneousPath(testmap,0,0,0,10,10,5000) ) { fprintf(stderr,"Bug , could not find empty close diagonal line without size initialization!\n");  }

    SetAgentHeading(testmap,0,120);
    SetAgentSize(testmap,0,4,2);
    SetMapUnit_In_cm(testmap,15); /* 15 cm per map unit */



    if ( !FindSponteneousPath(testmap,0,0,0,999,999,10000) ) { fprintf(stderr,"Bug , could not find empty far diagonal line !\n");  }

    if ( !FindSponteneousPath(testmap,0,0,0,999,999,10000) ) { fprintf(stderr,"Bug , could not find empty far diagonal line !\n");  }



    DeleteMap(testmap);
    return 0;
}
