#include "RVKnowledgeBase.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../InputParser/InputParser_C.h"

boolean KB_ok(struct KnowledgeBase * kb)
{
  return 0;
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// OBJECT SCOPE , MUST COMMIT / DELETE ALL SUBSTRUCTRES
unsigned int NewObject(struct KnowledgeBase * kb,char * obj_id,char * obj_category)
{
  return 0;/*
  unsigned int count=kb->objects_count;
  kb->objects[count].id_hash     = StoreCstr_Hash(&kb->objects[count].id,obj_id);
  kb->objects[count].cat_id_hash = StoreCstr_Hash(&kb->objects[count].category,obj_category);

  kb->objects_count +=1 ;
  return ( kb->objects_count - 1 );*/
}

void DeleteObject(struct KnowledgeBase * kb,unsigned int obj_number)
{
 /*
  Delete_OString(&kb->objects[obj_number].id);
  Delete_OString(&kb->objects[obj_number].category);
  DeleteOStringArray(&kb->objects[obj_number].traits);
  */
}

void AddTraitToObject(struct KnowledgeBase * kb,unsigned int obj_number,char * obj_trait)
{
  /*
  AddToOStringArray(&kb->objects[obj_number].traits,(char *)obj_trait);
  */
}


void PrintObjectsAtKnowledgeBase(struct KnowledgeBase * kb)
{
  /*
  int i=0,z=0;
  fprintf(stderr,"OBJECTS LIST ----------------------------\n");
  for ( i=0; i<kb->objects_count; i++ )
  {
    fprintf(stderr,"%s ( %s ) \n",kb->objects[i].id.str,kb->objects[i].category.str);
    for ( z=0; z<GetOStringArrayLength(&kb->objects[i].traits); z++ )
    {
      fprintf(stderr,"%s | ",kb->objects[i].traits.array[z].str);
    }
    fprintf(stderr,"\n");
  }
  */
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// fact_id , fact_value  ,  should be null terminated strings..!
// Usage is the following fact_id = AMMAR , fact_value = IS GOOD
//                        fact_id = SECURITY , fact_value = ENABLED
void NewFact(struct KnowledgeBase * kb,char * fact_id,char * fact_value)
{

}


unsigned char * SearchFact(struct KnowledgeBase * kb,char * fact_id)
{
  return 0;
}


boolean LoadKnowledgeBase(struct KnowledgeBase * kb,char * filename)
{
   return 0;/*
    boolean retres=0;
    struct InputParserC * ipc=0;
    ipc = InputParser_Create(256,5);
    if ( InputParser_SelfCheck(ipc) == 0 ) { fprintf(stderr,"\nInputParser Self Check Returns erroneous state \n"); return(0); }

    struct AFSContext afs;

    if ( StartParsingFile(&afs,filename) )
    {

       char line[1024]={0};
       unsigned int linelen=1024;

       fprintf(stderr,"GetNextLineFile\n");
       while ( GetNextLineFile(&afs,line,linelen)!=0 )
       {
         fprintf(stderr,"%s\n",line);
         if (Tell(line) != 1 ) { fprintf(stderr,"Command `%s` failed\n",line); }
       }
      retres=1;
    }
    InputParser_Destroy(ipc);
    StopParsingFile(&afs);

    return retres;
    */
}

boolean SaveKnowledgeBase(struct KnowledgeBase * kb,char * filename)
{
   return 0;/*
    FILE* fp;
    fp = fopen((const char *)filename,"w");
    if (fp == NULL) { return 0; }
    if ( !KB_ok(kb) ) { return 0; }

    fclose(fp);
    return 1;
    */
}


struct KnowledgeBase * InitKnowledgeBase()
{
   return 0;/*
   struct KnowledgeBase * kbint=0;
   kbint = ( struct KnowledgeBase * ) malloc ( sizeof ( struct KnowledgeBase ) );

   kbint->objects_max = 1000;
   kbint->objects_count = 0;
   kbint->objects = ( struct Object * ) malloc ( sizeof ( struct Object ) * (kbint->objects_max) );


  return kbint;
  */
}


boolean CloseKnowledgeBase(struct KnowledgeBase * kb)
{
  return 0;/*
  int i=0;
  for ( i=0; i<kb->objects_count; i++ ) { DeleteObject(kb,i); }

  if ( kb != 0 )  free(kb);
  return 0;
  */
}

// SENTENCE CONVERSION

char ConvertTokenToItsType(struct InputParserC * ipc , int i)
{
  return 0;/*
  char rettok = NOTHING;

         if ( InputParser_WordCompareNoCase(ipc,i,"?",1)!=0 )   { rettok=NOTHING; }   else
         if ( InputParser_WordCompareNoCase(ipc,i,"UNKNOWN",7)!=0 )   { rettok=NOTHING; }   else
         if ( InputParser_WordCompareNoCase(ipc,i,"NULL",4)!=0 )   { rettok=NOTHING; }   else
         if ( InputParser_WordCompareNoCase(ipc,i,"TRUE",4)!=0 )   { rettok=TRUE; }   else
         if ( InputParser_WordCompareNoCase(ipc,i,"FALSE",5)!=0 )  { rettok=FALSE; } else
         if ( InputParser_WordCompareNoCase(ipc,i,"0",1)!=0 )  { rettok=NUMBER; } else
         if ( InputParser_GetWordInt(ipc,i)!=0 )  { rettok=NUMBER; } else
         // RELATIONS
         if ( InputParser_WordCompareNoCase(ipc,i,"OBJECT",6)!=0 )   { rettok=OBJECT; }   else
         // RELATIONS
         if ( InputParser_WordCompareNoCase(ipc,i,"PARTOF",6)!=0 )   { rettok=RELATION; } else
         if ( InputParser_WordCompareNoCase(ipc,i,"IS",2)!=0 )       { rettok=RELATION; } else
         if ( InputParser_WordCompareNoCase(ipc,i,"CONTAINS",8)!=0 ) { rettok=RELATION; } else
         // OPERATORS
         if ( InputParser_WordCompareNoCase(ipc,i,"(",1)!=0 )       { rettok=START; } else
         if ( InputParser_WordCompareNoCase(ipc,i,")",1)!=0 )       { rettok=END; } else
         // OPERATORS
         if ( InputParser_WordCompareNoCase(ipc,i,"FORALL",6)!=0 )      { rettok=FORALL; } else
         if ( InputParser_WordCompareNoCase(ipc,i,"EXISTS",6)!=0 )       { rettok=EXISTS; } else
         // OPERATORS
         if ( InputParser_WordCompareNoCase(ipc,i,"<=>",3)!=0 )     { rettok=EQUIVALENT; } else
         if ( InputParser_WordCompareNoCase(ipc,i,"=>",2)!=0 )      { rettok=MEANS; } else
         if ( InputParser_WordCompareNoCase(ipc,i,"=",1)!=0 )       { rettok=EQUALS; } else
         if ( InputParser_WordCompareNoCase(ipc,i,"==",2)!=0 )      { rettok=EQUALS; } else
         if ( InputParser_WordCompareNoCase(ipc,i,":=",2)!=0 )      { rettok=STORE; } else
         if ( InputParser_WordCompareNoCase(ipc,i,"+",1)!=0 )       { rettok=ADD; } else
         if ( InputParser_WordCompareNoCase(ipc,i,"-",1)!=0 )       { rettok=SUB; } else
         if ( InputParser_WordCompareNoCase(ipc,i,"<",1)!=0 )       { rettok=LESSTHAN; } else
         if ( InputParser_WordCompareNoCase(ipc,i,"<=",2)!=0 )      { rettok=LESS_EQ_THAN; } else
         if ( InputParser_WordCompareNoCase(ipc,i,">",1)!=0 )       { rettok=GREATERTHAN; } else
         if ( InputParser_WordCompareNoCase(ipc,i,">=",2)!=0 )      { rettok=GREATER_EQ_THAN; } else
         // OPERATORS
         if ( InputParser_WordCompareNoCase(ipc,i,"AND",3)!=0 )      { rettok=AND; } else
         if ( InputParser_WordCompareNoCase(ipc,i,"OR",2)!=0 )       { rettok=OR; } else
         if ( InputParser_WordCompareNoCase(ipc,i,"XOR",3)!=0 )      { rettok=XOR; } else
         if ( InputParser_WordCompareNoCase(ipc,i,"NOT",3)!=0 )      { rettok=NOT; } else
         // VARIABLE ID`S
                                                                     { rettok=ID; }

   return rettok;
   */
}

char SliceAndProcString(char * question_tokens,unsigned int start,unsigned int end,char ask_true)
{
   return 0;/*
   int i;
   int ending_brackets_count=0;
   for ( i = end; i>=start; i--)
   {
     if (question_tokens[i] == END) { ++ending_brackets_count; } else
     if (question_tokens[i] == START)
     {
        if ( ending_brackets_count==0 ) { fprintf(stderr,"Hm syntax error!\n"); } else
          {

          }
     }

     if ( i == start ) break;
   }
   return 1;
   */
}

char Transaction(char * question,char ask_true)
{
    return 0;/*
    int i;
    char quest_toks[256]={0};
    struct InputParserC * ipc=0;
    ipc = InputParser_Create(256,3);
    InputParser_SetDelimeter(ipc,2,' ');

     int res = InputParser_SeperateWords(ipc,question,0);
     for ( i=0; i<res; i++)
     {
       quest_toks[i] = ConvertTokenToItsType(ipc,i) ;
     }

     if ( ask_true == 1 )  printf("Asked  "); else
     if ( ask_true == 0 )  printf("Told  ");
     for ( i=0; i<res; i++) { printf(" %u ",quest_toks[i]); } printf("\n");

     SliceAndProcString(quest_toks,0,res-1,ask_true);


    InputParser_Destroy(ipc);

 return 0;
 */
}

char Ask(char * question)
{
 return Transaction(question,1);
}


char Tell(char * info)
{
  return Transaction(info,0);
}

char Search(char * what)
{
  return 0;
}
