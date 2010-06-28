#include "InputParser_C.h"
/* InputParser..
   A small generic library for parsing a string and tokenizing it!
   URLs: http://62.103.22.50
   Written by Ammar Qammaz a.k.a. AmmarkoV 2006

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
*/


const char * _ipc_ver=" 0.33 ( 1/3 ) written from scratch - 8/2/10 \0";

char * InputParserC_Version()
{
  return (char *) _ipc_ver;
}


inline signed int Str2Int_internal(char * inpt,unsigned int start_from,unsigned int length)
{
    if ( inpt == 0 ) { fprintf(stderr,"Null string to Str2IntInternal!\n"); return 0;}
    int intresult=0,multiplier=1,curnum=0;
    unsigned char error_flag=0;
    signed int i;

    //fprintf(stderr,"Converting to int string (%p) begining from %u and ending at %u ",inpt,start_from,start_from+length);
    for (i=start_from+length-1; i>=start_from; i--)
    {
        if ( i < 0 ) { //fprintf("Gone negative! %u \n",i);
                       break; }
        //fprintf(stderr,"Translating %u , %c \n",i,inpt[i]);
        curnum=(char) (inpt[i])-'0';

        if ((curnum>=0)&(curnum<=9))
        {
            intresult=intresult+(multiplier*curnum);
            multiplier=multiplier*10;
        }
        else
        {
            error_flag=1;
        }
    }


    return intresult;
}



inline unsigned char CheckIPCOk(struct InputParserC * ipc)
{
    if ( ipc->guardbyte1.checksum != ipc->guardbyte2.checksum ) { fprintf(stderr,"Input Parser - Wrong GuardChecksums #1\n"); return 0; }
    if ( ipc->guardbyte3.checksum != ipc->guardbyte4.checksum ) { fprintf(stderr,"Input Parser - Wrong GuardChecksums #2\n"); return 0; }
    if ( ipc->guardbyte1.checksum != ipc->guardbyte4.checksum ) { fprintf(stderr,"Input Parser - Wrong GuardChecksums #3\n"); return 0; }
    if ( (ipc->tokenlist==0) ||  (ipc->tokens_max<ipc->tokens_count) ) { fprintf(stderr,"Input Parser - Tokenlist error\n"); return 0; }
    if ( (ipc->delimeters==0) || (ipc->max_delimeter_count<ipc->cur_delimeter_count) ) { fprintf(stderr,"Input Parser - Delimeter error\n"); return 0; }

    return 1;
}


void InputParser_DefaultDelimeters(struct InputParserC * ipc)
{
    if ( CheckIPCOk(ipc) == 0 ) { return; }
    if ( ipc->delimeters == 0 ) { return; }

    int i;
    //fprintf(stderr,"Default Delimters ( %u ) ",ipc->max_delimeter_count);
    for ( i=0; i<ipc->max_delimeter_count; i++)
     {
         switch (i)
         {
           case 0 : ipc->delimeters[0]='\n'; break;
           case 1 : ipc->delimeters[1]=',';  break;
           case 2 : ipc->delimeters[2]='=';  break;
           case 3 : ipc->delimeters[3]='(';  break;
           case 4 : ipc->delimeters[4]=')';  break;
           default : ipc->delimeters[i]='\n'; break;
         };

         //fprintf(stderr," %u - %c ",i,ipc->delimeters[i]);

     }
   ipc->cur_delimeter_count = ipc->max_delimeter_count;
   //fprintf(stderr,"\n");

   return;
}

/*
  InitInputParser..

  Allocates an ipc structure and with memory to it according to options for future use!
  for example InitInputParser( 256 , 5 ) will allocate an InputParser Instance capeable of tokenizing up to 256 different words
  from 5 delimeters
*/
struct InputParserC * InputParser_Create(unsigned int max_string_count,unsigned int max_delimiter_count)
{
    struct InputParserC * ipc=0;

    ipc = ( struct InputParserC * ) malloc ( sizeof ( struct InputParserC ) );
    if ( ipc  == 0 ) { fprintf(stderr,"InputParserC unable to commit memory for a new instance\n"); return 0; }

    ipc->tokenlist = (struct tokens *) malloc( sizeof(struct tokens) * (max_string_count+1) );
    if ( ipc->tokenlist  == 0 ) { fprintf(stderr,"InputParserC unable to commit memory for a new Token List\n"); return 0; }
    ipc->tokens_count=0;
    ipc->tokens_max = max_string_count;


    ipc->delimeters = (char *) malloc( sizeof(char) * (max_delimiter_count+1) );
    if ( ipc->delimeters  == 0 ) { fprintf(stderr,"InputParserC unable to commit memory for a new Delimeter List\n"); return 0; }
    ipc->max_delimeter_count=max_delimiter_count;
    ipc->cur_delimeter_count=max_delimiter_count;


    ipc->container_start=0;
    ipc->container_end=0;

    ipc->guardbyte1.checksum=66666;
    ipc->guardbyte2.checksum=66666;
    ipc->guardbyte3.checksum=66666;
    ipc->guardbyte4.checksum=66666;

    ipc->local_allocation = 0; // No allocation by default! :)
    ipc->str_length=0;
    ipc->str=0;

    InputParser_DefaultDelimeters(ipc);

    return ipc;
}

/*
  InputParser_Destroy..

  Safely destroys a non null ipc structure and deallocates all commited memory
*/
void InputParser_Destroy(struct InputParserC * ipc)
{
    if ( ipc == 0 ) { return; }

    //fprintf(stderr,"InputParserC freeing delimeters\n");
    if ( ipc->delimeters != 0 )
    {
     free(ipc->delimeters);
     //ipc->delimeters=0;
    }
    ipc->cur_delimeter_count=0;
    ipc->max_delimeter_count=0;

/*
    if ( ipc->container_start != 0 )
     {
       free(ipc->container_start);
       ipc->container_start=0;
     }
    if ( ipc->container_end != 0 )
     {
       free(ipc->container_end);
       ipc->container_end=0;
     }
     */
    ipc->cur_container_count = 0;
    ipc->max_container_count = 0;

    //fprintf(stderr,"InputParserC freeing tokenlist\n");
    if ( ipc->tokenlist != 0 )
     {
      free(ipc->tokenlist);
      //ipc->tokenlist=0;
     }
    ipc->tokens_max=0;
    ipc->tokens_count=0;

    //fprintf(stderr,"InputParserC freeing local allocation\n");
    if ( ipc->local_allocation == 1 ) { if ( ipc->str!=0 ) free(ipc->str); }
    //ipc->local_allocation=0;
    ipc->str_length=0;



    // RESULT
    ipc->guardbyte1.checksum = 0;
    ipc->guardbyte2.checksum = 0;
    ipc->guardbyte3.checksum = 0;
    ipc->guardbyte4.checksum = 0;

    //fprintf(stderr,"InputParserC freeing ipc\n");
    free(ipc);


}

// ........................................................
// Delimeters ........................................................
// ........................................................

/*
   CheckDelimeterNumOk..
   Checks if delimeter with number num has allocated space in memory
*/
inline unsigned char CheckDelimeterNumOk(struct InputParserC * ipc,int num)
{
    if ( ipc->max_delimeter_count <= num ) return 0;
    return 1;
}

/*
   SetDelimeter..
   Sets Delimeter number num with value tmp
*/
void InputParser_SetDelimeter(struct InputParserC * ipc,int num,char tmp)
{
    if (CheckDelimeterNumOk(ipc,num)==0) { return;}
    ipc->delimeters[num]=tmp;
}

/*
   GetDelimeter..
   Returns value of  Delimeter with number num
*/
char InputParser_GetDelimeter(struct InputParserC * ipc,int num)
{
    if (CheckDelimeterNumOk(ipc,num)==0) { return 0;}
    return ipc->delimeters[num];
}
// .......................................................................
// .......................................................................
// .......................................................................




/*
   Selfchecks IPC instance and returns 1 if everything is ok , 0 if error..
*/
unsigned char InputParser_SelfCheck(struct InputParserC * ipc)
{
    if (CheckIPCOk(ipc)==0) { fprintf(stderr,"\n\n!!!!\nThis instance of InputParser is broken\n!!!!\n\n");
                              return 0; }
    return 1;
}

/*
   CheckWordNumOk..
   Checks if word with number num has allocated space in memory
*/
inline unsigned char CheckWordNumOk(struct InputParserC * ipc,unsigned int num)
{
  if ( CheckIPCOk(ipc)==0) { return 0; }
  if ( (ipc->tokenlist==0) || ( ipc->tokens_count <= num ) ) { return 0; }


  return 1;
}

/*
   InputParser_GetWord..
   Copies token with number (num) to c string (wheretostore) , variable storagesize contains the total size of wheretostore..!
*/
unsigned int InputParser_GetWord(struct InputParserC * ipc,unsigned int num,char * wheretostore,unsigned int storagesize)
{
    if ( CheckWordNumOk(ipc,num) == 0 ) { return 0; }
    if ( storagesize < ipc->tokenlist[num].length+1 ) /* +1 gia to \0 */ return 0;


    int i=0;
    for ( i = ipc->tokenlist[num].token_start; i<ipc->tokenlist[num].token_start+ipc->tokenlist[num].length; i++ )
    wheretostore[i-ipc->tokenlist[num].token_start] = ipc->str[i];

    wheretostore[ipc->tokenlist[num].length] = 0;

    return ipc->tokenlist[num].length;
}


/*
   InputParser_WordCompareNoCase..
   Compares word (word) with token with number (num) , null terminating character is not required , NO CASE SENSITIVITY..!
*/
unsigned char InputParser_WordCompareNoCase(struct InputParserC * ipc,unsigned int num,char * word,unsigned int wordsize)
{
    if ( wordsize != InputParser_GetWordLength(ipc,num) ) { return 0; }
    int i=0;
    for ( i=0; i<wordsize; i++ )
    {
      if (toupper(ipc->str[ipc->tokenlist[num].token_start+i])!=toupper(word[i])) {  return 0; }
    }
    return 1;
}


/*
   InputParser_WordCompare..
   Compares word (word) with token with number (num) , null terminating character is not required..!
*/
unsigned char InputParser_WordCompare(struct InputParserC * ipc,unsigned int num,char * word,unsigned int wordsize)
{
    if ( wordsize != InputParser_GetWordLength(ipc,num) ) { return 0; }

    int i=0;
    for ( i=0; i<wordsize; i++ )
    {
      if (ipc->str[ipc->tokenlist[num].token_start+i]!=word[i]) {  return 0; }
    }
    return 1;
}


/*
   InputParser_GetUpcaseWord..
   Same with InputParser_GetWord , the result is converted to upcase..!
*/
unsigned int InputParser_GetUpcaseWord(struct InputParserC * ipc,unsigned int num,char * wheretostore,unsigned int storagesize)
{
    if ( CheckWordNumOk(ipc,num) == 0 ) { return 0; }
    if ( storagesize < ipc->tokenlist[num].length+1 ) /* +1 gia to \0 */  return 0;

    int i=0;
    for ( i = ipc->tokenlist[num].token_start; i<ipc->tokenlist[num].token_start+ipc->tokenlist[num].length; i++ )
    wheretostore[i-ipc->tokenlist[num].token_start] = toupper(ipc->str[i]);

    wheretostore[ipc->tokenlist[num].length] = 0;

    return ipc->tokenlist[num].length;
}

/*
   InputParser_GetUpcaseWord..
   Same with InputParser_GetWord , the result is converted to lowercase..!
*/
unsigned int InputParser_GetLowercaseWord(struct InputParserC * ipc,unsigned int num,char * wheretostore,unsigned int storagesize)
{
    if ( CheckWordNumOk(ipc,num) == 0 ) { return 0; }
    if ( storagesize < ipc->tokenlist[num].length+1 ) /* +1 gia to \0 */  return 0;

    int i=0;
    for ( i = ipc->tokenlist[num].token_start; i<ipc->tokenlist[num].token_start+ipc->tokenlist[num].length; i++ )
    wheretostore[i-ipc->tokenlist[num].token_start] = tolower(ipc->str[i]);

    wheretostore[ipc->tokenlist[num].length] = 0;

    return ipc->tokenlist[num].length;
}


/*
   InputParser_GetChar..
   Returns character (pos) from token (num)..!
*/
char InputParser_GetWordChar(struct InputParserC * ipc,unsigned int num,unsigned int pos)
{
    if ( CheckWordNumOk(ipc,num) == 0 ) { return 0; }

    if ( pos>=ipc->tokenlist[num].token_start+ipc->tokenlist[num].length ) { return 0; }

    return ipc->str[ipc->tokenlist[num].token_start + pos ];
}

/*
   InputParser_GetWordInt..
   Same with InputParser_GetWord , if the result can be converted to a number , it returns this number
   else 0 is returned
*/
signed int InputParser_GetWordInt(struct InputParserC * ipc,unsigned int num)
{
    if ( CheckWordNumOk(ipc,num) == 0 ) { return 0; }
    return Str2Int_internal(ipc->str,ipc->tokenlist[num].token_start,ipc->tokenlist[num].length);
}

/*
   InputParser_GetChar..
   Returns total length of token (num)..!
*/
unsigned int InputParser_GetWordLength(struct InputParserC * ipc,unsigned int num)
{
    if ( CheckWordNumOk(ipc,num) == 0 ) { return 0; }
    return ipc->tokenlist[num].length;
}

/*
   InputParser_SeperateWords..
   Seperates words in c string (inpt) to tokens using delimiters set-up at structure ipc and keeps result at structure ipc
   if the c string will be erased before getting back the tokens you can set the keepcopy byte to 1 , this will allocate memory to
   keep a copy of the string..!
   the number returned is the total of tokens extracted!
*/
int InputParser_SeperateWords(struct InputParserC * ipc,char * inpt,char keepcopy)
{

  if (CheckIPCOk(ipc)==0) { return 0; }
  if  ( inpt == 0 ) return 0; // NULL INPUT -> NULL OUTPUT

  unsigned int   STRING_END = strlen(inpt) ;
  int WORDS_SEPERATED = 0 , NEXT_SHOULD_NOT_BE_A_DELIMITER=1 , FOUND_DELIMETER ; // Ignores starting ,,,,,string,etc

  if ( STRING_END == 0 ) { return 0; } // NULL INPUT -> NULL OUTPUT pt 2

  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // COPY STRING ( OR POINTER ) TO IPC STRUCTURE
  if ( keepcopy == 1 ) {  // IF WE HAVE ALREADY ALLOCATED A STRING TO ipc->str , we should free it to prevent memory leaks!
                          if (ipc->local_allocation == 1)
                          {
                            if (ipc->str!=0)
                            { // ipc->str contains a previous value!
                              free(ipc->str);
                              ipc->local_allocation = 0;
                            }
                          }
                          ipc->str = (char * ) malloc( sizeof(char) * (STRING_END+1) );
                          ipc->local_allocation = 1;
                          strncpy( ipc->str , inpt , STRING_END ) ;
                       } else
                       { ipc->str = inpt; }

  ipc->str_length = STRING_END;
  // COPY STRING ( OR POINTER ) TO IPC STRUCTURE
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


  register int i,z;
  ipc->tokens_count = 0 , ipc->tokenlist[0].token_start=0;
  for (i=0; i<STRING_END; i++)
  {
    FOUND_DELIMETER = 0;
    for (z=0; z<ipc->cur_delimeter_count; z++)
    {

    if ( inpt[i] == ipc->delimeters[z] )
      {
        FOUND_DELIMETER = 1;
        if (NEXT_SHOULD_NOT_BE_A_DELIMITER==0)
        {
         ipc->tokenlist[ipc->tokens_count].length = i - ipc->tokenlist[ipc->tokens_count].token_start;
         ipc->tokens_count+=1;
         ipc->tokenlist[ipc->tokens_count].token_start = i+1;
         WORDS_SEPERATED+=1;
         break;
        } else
        {
          ipc->tokenlist[ipc->tokens_count].token_start=i+1;
        }
      }
    }

    if (FOUND_DELIMETER == 0 ) NEXT_SHOULD_NOT_BE_A_DELIMITER=0; else
    if (FOUND_DELIMETER == 1 ) NEXT_SHOULD_NOT_BE_A_DELIMITER=1;

  }

   if (NEXT_SHOULD_NOT_BE_A_DELIMITER==0)
        {
         ipc->tokenlist[ipc->tokens_count].length = i - ipc->tokenlist[ipc->tokens_count].token_start;
         ipc->tokens_count+=1;
         ipc->tokenlist[ipc->tokens_count].token_start = i+1;
         WORDS_SEPERATED+=1;

        } else
  ipc->tokenlist[ipc->tokens_count].length = i - ipc->tokenlist[ipc->tokens_count].token_start;

  return WORDS_SEPERATED;
}

/*
 Same with InputParser_SeperateWords it does the (char *) type cast
*/
int InputParser_SeperateWordsCC(struct InputParserC * ipc,const char * inpt,char keepcopy)
{
  return InputParser_SeperateWords(ipc,(char * )inpt,keepcopy);
}

/*
 Same with InputParser_SeperateWords it does the (char *) type cast
*/
int InputParser_SeperateWordsUC(struct InputParserC * ipc,unsigned char * inpt,char keepcopy)
{
  return InputParser_SeperateWords(ipc,(char * )inpt,keepcopy);
}
