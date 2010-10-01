#ifndef MAP_H
#define MAP_H
#include "AStarCompute.h"

// POSSIBLE OBJECTS
enum Possible_Objects
   {
      FREE = 0,
      BLOCKED,
      MOVING
   };

struct Actor
{
  unsigned int x,y,heading;
  unsigned int logical_size,size_to_sub;

};


class Map
{
    public:
        Map(unsigned int size_x,unsigned int size_y);
        virtual ~Map();
        void SetActorCurrentPosition(unsigned int ptx,unsigned int pty,unsigned int heading);
        void SetActorPhysicalSize(unsigned int units);

        unsigned int GetObjectAt(unsigned int ptx,unsigned int pty);
        unsigned int GetNeighborsAt(unsigned int ptx,unsigned int pty);
        unsigned int SetObjectAt(unsigned int ptx,unsigned int pty,unsigned int obj);

        void AddNeighbors(unsigned int x,unsigned int y,unsigned char state);
        void CalculateAllNeighbors();
        unsigned int FindPathTo(unsigned int to_x,unsigned int to_y,unsigned int timelimit_ms);
        unsigned int GetRoutePoint(unsigned char level,unsigned int pt_num,unsigned int &from_x,unsigned int &from_y);
        void Clear();

        unsigned int HTMLOutput(char * filename);
    protected:
    private:
    unsigned char add_neighbors_mode;


    AStarCompute *pathcomputer;

    NodeNeighborsCount * world_neighbors;
    NodeData * world;
    unsigned int world_size,world_x,world_y;
    Actor us;


    NodeRef *openlist;
    unsigned int openlist_size;
    unsigned int openlist_current_size;

    int error_number;

    unsigned short diagonal_penalty;
    unsigned short horizontal_penalty;
    unsigned short turning_penalty;
    bool block_perfect_diagonal;
};



#endif // MAP_H
