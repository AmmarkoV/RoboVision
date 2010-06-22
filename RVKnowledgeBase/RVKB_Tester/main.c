#include <stdio.h>
#include <stdlib.h>
#include "../../../AFScripts/AFScripts.h"
#include "../../../InputParser_C/InputParser_C.h"
#include "../RVKnowledgeBase.h"

void LinkinPark()
{
    AFSVersion();
    InputParserC_Version();
}

int main()
{
    LinkinPark();
    struct KnowledgeBase * kb;
    kb = InitKnowledgeBase();
    LoadKnowledgeBase(kb,"my_knowledgebase");

      PrintObjectsAtKnowledgeBase(kb);

      Ask("AMMAR CONTAINS TECHNOLOGY");


    //SaveKnowledgeBase(kb,"my_knowledgebase");
    CloseKnowledgeBase(kb);
    return 0;
}
