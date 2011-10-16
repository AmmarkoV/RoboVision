#ifndef RVKNOWLEDGEBASE_H_INCLUDED
#define RVKNOWLEDGEBASE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif


enum TOKENTYPE            /* Defines an enumeration type    */
{
    NOTHING = 0,
    TRUE,
    FALSE,
    NUMBER,
    UNDER_IS_ABS_VALUE, //<<<<<<<<<<<<<<<<<<<<<<<<
    ID ,
    UNDER_IS_VALUE, //<<<<<<<<<<<<<<<<<<<<<<<<

    AFTER_IS_BRACKET, //>>>>>>>>>>>>>>>>>>>>>>>>
    START,
    END,
    UNDER_IS_BRACKET, //<<<<<<<<<<<<<<<<<<<<<<<<



    AFTER_IS_CONCEPT, //>>>>>>>>>>>>>>>>>>>>>>>>
    OBJECT,
    RELATION,
    LOCATION,
    TIME,
    FUNCTION,
    UNDER_IS_CONCEPT, //<<<<<<<<<<<<<<<<<<<<<<<<

    AFTER_IS_OPERATOR, //>>>>>>>>>>>>>>>>>>>>>>>>
    AND,
    OR,
    XOR,
    NOT,
    ADD,
    SUB,
    GREATERTHAN,
    GREATER_EQ_THAN,
    LESSTHAN,
    LESS_EQ_THAN,

    EXISTS,
    FORALL,

    STORE,
    MEANS,
    EQUALS,
    EQUIVALENT,
    UNDER_IS_OPERATOR, //<<<<<<<<<<<<<<<<<<<<<<<<

    END_OF_AXIOMATIC_TOKENS

} toktype;


struct CStringO
{
    unsigned int TODO_ADD_THINGS_HERE;
};

struct CStringOArray
{
    unsigned int TODO_ADD_THINGS_HERE;
};

struct Event
{  //     id       id2       place       time          description
   //my IGUANA BIT ME ,  IN MY ROOM , 29-1-10 00:00 , I WAS JUST PETTING HIM BUT GOT TOO CLOSE TO ITS HEAD
   unsigned long id_hash;
   struct CStringO id;
   unsigned long id2_hash;
   struct CStringO id2;

   struct CStringO place;
   struct CStringO time;
   struct CStringO description;

};



struct Object
{  //      id       traits ....................
   //my IGUANA is  GREEN , LIKES HEAT , BITES , GETS EASILY UPSET
   unsigned long id_hash;
   struct CStringO id;

   struct CStringO category;
   unsigned long cat_id_hash;

   struct CStringOArray traits;
};

struct Fact
{  //      id         value
   //the FISH is an ANIMAL
   //      I HAVE LOST 2 YEARS OF MY LIFE IN MY UNIVERSITY

   unsigned long id_hash;
   struct CStringO id;
   struct CStringO value;
};

struct Hypotheses
{  //         id1        id2                 value
   //IF the ANIMAL is a FISH then it is of COLD BLOOD
   unsigned long id_hash1,id_hash2;
   struct CStringO if_id1;
   struct CStringO if_id2;

   struct CStringO then_value;
   /*
     Universal contingent: If the animal is a fish, then it is of cold blood
     Temporal/causal:    If the glass is hit, it will break
     Council:            If you work hard, you will go well in the life
     Promise:            If you pick up your toys, I will give you an ice cream
     Threatens:   If you make it again, I will hit you
     Warning:            If you jump yourself here the limit of speed it will plunder you the police
     Counterfactuals     If it had gotten some short blows it would have won easily
     Not consistent with charts really:      If you want to read a good book, there is one on the table 6
   */
   struct CStringO emotion;
   unsigned char certainty; // 0 ( very unnlikely ) .. 255 ( sure thing )
   // universal law , council , promise , threatening , cause , warning , counterfactual , funny , null

};


struct Relations
{
   unsigned long id_hash;
   struct CStringO id;
   struct CStringO value;
};

struct KnowledgeBase
{
   unsigned long total_facts;
   struct Fact * facts;

   struct Object * objects;
   unsigned long objects_count;
   unsigned long objects_max;

};

struct Model
{
  struct Hypotheses hypotheses_for_model;
  struct KnowledgeBase newmodel;
};


typedef unsigned char boolean;

struct KnowledgeBase * InitKnowledgeBase();
boolean CloseKnowledgeBase(struct KnowledgeBase * kb);

void PrintObjectsAtKnowledgeBase(struct KnowledgeBase * kb);

char AskTheInternet(char * question,char * answer,unsigned int answer_length,char * website);
char Ask(char * question);
char Tell(char * info);
char Search(char * what);

boolean SaveKnowledgeBase(struct KnowledgeBase * kb,char * filename);
boolean LoadKnowledgeBase(struct KnowledgeBase * kb,char * filename);


#ifdef __cplusplus
}
#endif

#endif // RVKNOWLEDGEBASE_H_INCLUDED
