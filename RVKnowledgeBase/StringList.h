#ifndef STRINGLIST_H_INCLUDED
#define STRINGLIST_H_INCLUDED


struct StringListItem
{
   unsigned int hash;
   unsigned int length;

   char * string;


   void * payload;
};

struct StringList
{
   unsigned int max;
   unsigned int current;

   struct StringListItem * item;
};






struct StringList * CreateStringList(unsigned int list_size);
unsigned int DeleteStringList(struct StringList * sl);


unsigned int FindStringInList(struct StringList * sl,char * inpt);
unsigned int SwapStringInList(struct StringList * sl,unsigned int swap1,unsigned int swap2);
unsigned int DeleteFromStringInList(struct StringList * sl,unsigned int num);

#endif // STRINGLIST_H_INCLUDED
