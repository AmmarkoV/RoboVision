#ifndef FEATURELISTS_H_INCLUDED
#define FEATURELISTS_H_INCLUDED

struct FeatureData
{
  char lost;
  char lost_since;

  unsigned int x,y,z;
  unsigned int mem;
  unsigned int group;
};

struct PointCorrespondence
{
   unsigned int mapped_to;
   unsigned int score;
};

struct FeatureList
{
  unsigned int total_features;
  struct FeatureData * feature;

  unsigned int reg_for_correspondance_1;
  struct PointCorrespondence * correspondance_1;

  unsigned int reg_for_correspondance_2;
  struct PointCorrespondence * correspondance_2;

  unsigned int reg_for_correspondance_3;
  struct PointCorrespondence * correspondance_3;

};


#endif // FEATURELISTS_H_INCLUDED
