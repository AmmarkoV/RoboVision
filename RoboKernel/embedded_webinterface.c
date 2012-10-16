/*
AmmarServer , Hello World main executable

URLs: http://ammar.gr
Written by Ammar Qammaz a.k.a. AmmarkoV 2012

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../AmmarServer/src/AmmServerlib/AmmServerlib.h"
#include "embedded_webinterface.h"

char webserver_root[512]="../robot/permfs/www";
char templates_root[512]="../AmmarServer/public_html/templates";


/*! Dynamic content code ..! START!*/
/* This is the dynamic Hello World example , as shown here : https://github.com/AmmarkoV/AmmarServer/wiki/Howto */

//The decleration of hello world dynamic content resources..
struct AmmServer_RH_Context helloworld={0};
unsigned int helloworld_times_shown=0;

//This function prepares the content of  helloworld context , ( helloworld.content ) whenever the index page is requested
void * prepare_helloworld_content_callback(unsigned int associated_vars)
{
  //Our Counter
  ++helloworld_times_shown;

  // We fill helloworld.content with the page
  sprintf(helloworld.content,"<html><body><center>Hello World<br>for the %u time</center></body></html>",helloworld_times_shown);

  // We signal the size of helloworld.content
  helloworld.content_size=strlen(helloworld.content);
  return 0;
}

//This function adds a Resource Handler for the page index.html and its callback function
void init_dynamic_content()
{
  //We create a virtual file called "index.html" , when this gets requested our prepare_helloworld_content_callback gets called!
  if (! AmmServer_AddResourceHandler(&helloworld,"/index.html",webserver_root,4096,0,&prepare_helloworld_content_callback) ) { fprintf(stderr,"Failed adding helloworld page\n"); }
}

//This function destroys all Resource Handlers and free's all allocated memory..!
void close_dynamic_content()
{
    AmmServer_RemoveResourceHandler(&helloworld,1);
}
/*! Dynamic content code ..! END ------------------------*/


int StartEmbeddedWebInterface()
{
    printf("Ammar Server starting up\n");

   //Kick start AmmarServer , bind the ports , create the threads and get things going..!
   AmmServer_Start("0.0.0.0",80,0,webserver_root,templates_root);

    //Create dynamic content allocations and associate context to the correct files
    init_dynamic_content();

    return 1;
}

int EmbeddedWebInterfaceRunning()
{
    return AmmServer_Running();
}

int StopEmbeddedWebInterface()
{
    //Delete dynamic content allocations and remove stats.html and formtest.html from the server
    close_dynamic_content();

    //Stop the server and clean state
    AmmServer_Stop();

    return 1;
}
