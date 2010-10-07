#include "webinterface.h"
#include "command_hal.h"
int last_snapshot;
int count_snapshot;

int OpenWebInterface()
{
 EraseConsoleOutput();

   FILE * pFile=0;
   pFile = fopen ("memfs/public_html/commands.dat","w");
   if (pFile!=NULL ) { fclose(pFile); return 1; }
   return 0;
}


int CloseWebInterface()
{
 int i=0;
 i=system((const char *)"cp empty.jpeg memfs/public_html/feed0.jpeg");
 if ( i != 0 ) fprintf(stderr,"Error cleaning web interface :P \n");
 i=system((const char *)"cp empty.jpeg memfs/public_html/feed1.jpeg");
 if ( i != 0 ) fprintf(stderr,"Error cleaning web interface :P \n");
 i=system((const char *)"cp empty.jpeg memfs/public_html/feed2.jpeg");
 if ( i != 0 ) fprintf(stderr,"Error cleaning web interface :P \n");
 i=system((const char *)"cp empty.jpeg memfs/public_html/feed3.jpeg");
 if ( i != 0 ) fprintf(stderr,"Error cleaning web interface :P \n");
 EraseConsoleOutput();
 return 1;
}


int WebIntNeedsNewSnapshot()
{

  FILE * pFile=0;
  pFile = fopen ("memfs/public_html/viewers.dat","r");
  if (pFile!=NULL )
    {
     fclose(pFile);
      if( remove( "memfs/public_html/viewers.dat" ) != 0 )
       {
        fprintf(stderr,"Could not clear viewers data\n");
        return 0;
       }
       fprintf(stderr,"Stream to viewers\n");

       return 1;
    }
 return 0;
}

int WebIntHasNewCommand()
{

  FILE * pFile=0;
  pFile = fopen ("memfs/public_html/commands.dat","r");
  if (pFile!=NULL )
    {

      /*
         READ COMMANDS HERE , USING AFSCRIPT!
      */
         char line[256]={0};
         while( fgets(line,sizeof(line),pFile) )
         {
            fprintf(stderr,"Command Received : %s\n",line);
            IssueCommand(line);
         }
      /*
         READ COMMANDS HERE!
      */

      fclose(pFile);

      if( remove( "memfs/public_html/commands.dat" ) != 0 )
       {
        fprintf(stderr,"Could not clear viewers data\n");
        return 0;
       }
       fprintf(stderr,"Stream to command\n");

       return 1;
    }
 return 0;
}



int UpdateNetworkInterface(wxBitmap * pica,wxBitmap * picb,wxBitmap * picc,wxBitmap * picd,unsigned int xsize,unsigned int ysize)
{
 if (WebIntNeedsNewSnapshot()==1)
  {
    ++count_snapshot;
    fprintf(stderr,"Snapshot #%u taken \n",count_snapshot);
    pica->SaveFile(wxT("memfs/public_html/feed0.jpeg"),wxBITMAP_TYPE_JPEG);
    picb->SaveFile(wxT("memfs/public_html/feed1.jpeg"),wxBITMAP_TYPE_JPEG);
    picc->SaveFile(wxT("memfs/public_html/feed2.jpeg"),wxBITMAP_TYPE_JPEG);
    picd->SaveFile(wxT("memfs/public_html/feed3.jpeg"),wxBITMAP_TYPE_JPEG);
  }

  WebIntHasNewCommand();

  return 1;
}
