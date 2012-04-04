#include "StringList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long hash(unsigned char *str)
{
        unsigned long hash = 5381;
        int c;
        while (c = *str++) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        return hash;
}

struct StringList * CreateStringList(unsigned int list_size)
{
  struct StringList* retres=0;

  retres = (struct StringList*) malloc(sizeof(struct StringList) * list_size );
  if (retres==0)
   {
       fprintf(stderr,"Could not allocate a StringList sized %u \n",list_size);
       return 0;
   }

  retres->max=list_size;
  retres->current=0;
  retres->item=0;


  retres->item = (struct StringListItem*) malloc(sizeof(struct StringListItem) * list_size );
  if ( retres->item==0 )
   {
       fprintf(stderr,"Could not allocate a StringListItem sized %u \n",list_size);
       free(retres);
       retres=0;
       return 0;
   }

   int i=0;
   for (i=0; i<list_size; i++)
    {
          retres->item[i].hash=0;
          retres->item[i].length=0;
          retres->item[i].string=0;
          retres->item[i].payload=0;
    }


  return retres;
}




unsigned int DeleteStringList(struct StringList * sl)
{
  if ( sl == 0 ) { return 0; }

  if (sl->item!=0)
  {
   int i=0;
   for (i=0; i<sl->max; i++)
    {
          if (sl->item[i].string!=0 )  { free(sl->item[i].string);  sl->item[i].string=0;}
          if (sl->item[i].payload!=0)  { free(sl->item[i].payload); sl->item[i].payload=0;}
    }
    free(sl->item);
    sl->item=0;
  } else
  {

  }

  free(sl);
  return 1;
}


unsigned int AddToStringList(struct StringList * sl,char * inpt)
{
  if ( sl == 0 ) { return 0; }
  if ( sl->item == 0 ) { return 0; }
  if (inpt==0) { return 0; }

  if (sl->current+1>=sl->max) { return 0; }

  unsigned int inpt_length = strlen(inpt);
  if (inpt_length==0) { return 0; }

  if (sl->item[sl->current].string!=0) { free(sl->item[sl->current].string); }

  sl->item[sl->current].string = (char *) malloc(sizeof(char) * (inpt_length+1));
  if (sl->item[sl->current].string==0) { return 0; }

  strncpy(sl->item[sl->current].string,inpt,inpt_length);
  sl->item[sl->current].length=inpt_length;

  sl->item[sl->current].hash=hash((unsigned char*) inpt);

  return sl->current;
}



unsigned int FindStringInList(struct StringList * sl,char * inpt)
{
  if ( sl == 0 ) { return 0; }
  if ( sl->item == 0 ) { return 0; }
  if (inpt==0) { return 0; }

  unsigned int inpt_hash=hash((unsigned char*) inpt);
  int i=0;
  //THIS WILL BECOME BINARY SEARCH IN THE FUTURE :P
  for (i=0; i<sl->current; i++)
   {
     if ( sl->item[sl->current].hash == inpt_hash )
       {
         if (strcmp(inpt,sl->item[sl->current].string)==0)
          {
            return i;
          }
       }
   }
  return 0;
}


unsigned int SwapStringInList(struct StringList * sl,unsigned int swap1,unsigned int swap2)
{
  if ( sl == 0 ) { return 0; }
  if ( sl->item == 0 ) { return 0; }

  if ( (sl->max <= swap1)||(sl->max <= swap2) ) { return 0; }

  struct StringListItem temp=sl->item[swap1];
  sl->item[swap1]=sl->item[swap2];
  sl->item[swap2]=temp;

  return 0;
}


unsigned int DeleteFromStringInList(struct StringList * sl,unsigned int num)
{
  if ( sl == 0 ) { return 0; }
  if ( sl->item == 0 ) { return 0; }

  if  (sl->max <= num) { return 0; }
  if  (sl->current <= num) { return 0; }

  unsigned int i=0;

  if (sl->current > 0)
    {
       i=sl->current-1;
       SwapStringInList(sl,num,i);
       --sl->current;
    }

  if (sl->item[i].string!=0 )  { free(sl->item[i].string);  sl->item[i].string=0;}
  if (sl->item[i].payload!=0)  { free(sl->item[i].payload); sl->item[i].payload=0;}

  return 1;
}
