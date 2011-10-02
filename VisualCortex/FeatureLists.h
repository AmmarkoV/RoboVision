#ifndef FEATURELISTS_H_INCLUDED
#define FEATURELISTS_H_INCLUDED

struct FeatureData
{
  char lost;
  char lost_since;

  float x,y,z;
  unsigned int dim_x,dim_y,dim_z;


  float last_x,last_y,last_z;
  unsigned int last_dim_x,last_dim_y,last_dim_z;

  unsigned int patch_width,patch_height;

  unsigned int correspondance_score;
  unsigned int mem;
  unsigned int group;
};

struct PointCorrespondence
{
   unsigned int mapped_to;
   unsigned int patch_width,patch_height;
   unsigned int score;
};

struct FeatureList
{
  unsigned int max_features;
  unsigned int current_features;
  unsigned int last_track_time;

  struct FeatureData * list;
  unsigned int reg_for_correspondance;
};

int FeatureListIsOk(struct FeatureList * list);
struct FeatureList * CreateFeatureList(unsigned int size , unsigned int def_patch_width,unsigned int def_patch_height);
int DestroyFeatureList(struct FeatureList * list);
int CopyFeatureList(struct FeatureList * source,struct FeatureList * target);
int ClearFeatureList(struct FeatureList * list);
int RenewTrackPoints(struct FeatureList * list,int point);
int AddToFeatureList(struct FeatureList * list, float x, float y,float z , int dim_x, int dim_y,int dim_z);
int SwapPointsAtFeatureList(struct FeatureList * list, int pointA , int pointB );
void RemoveTrackPointsIfTimedOut(struct FeatureList * list,unsigned int timeout);
unsigned int RemoveTrackPointsIfMovementMoreThan(struct FeatureList * list,unsigned int movement_max);
unsigned int Remove2DTrackPointsIfOutOfBounds(struct FeatureList * list,unsigned int x,unsigned int y ,unsigned int width , unsigned int height);
int GetFeatureData(struct FeatureList * list, unsigned int point_num,unsigned int data_type);
int PrintFeatureListContents(struct FeatureList * list);
int SaveFeatureListContents(struct FeatureList * list,char * filename);
#endif // FEATURELISTS_H_INCLUDED
