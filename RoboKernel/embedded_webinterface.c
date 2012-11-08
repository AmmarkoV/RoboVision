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
#include "visual_system.h"
#include "configuration.h"

#define DISABLE_EMBEDDED_WEB_INTERFACE 0 //Until it is in working order..
#define MAX_WEB_COMMAND_SIZE 512


char webserver_root[512]="../robot/permfs/public_html";
char templates_root[512]="../AmmarServer/public_html/templates";


/*! Dynamic content code ..! START!*/
/* This is the dynamic Hello World example , as shown here : https://github.com/AmmarkoV/AmmarServer/wiki/Howto */

//The decleration of hello world dynamic content resources..
struct AmmServer_RH_Context execute_web_command={0};
struct AmmServer_RH_Context camera_feed_image={0};
struct AmmServer_RH_Context camera_feed_page={0};

unsigned int helloworld_times_shown=0;
unsigned long JPEG_MAX_FILE_SIZE_IN_BYTES = 100 /*KB*/ * 1024;


//This function prepares the content of  execute_web_command context , ( execute_web_command.content ) whenever the index page is requested
void * prepare_execute_web_command_content_callback(unsigned int associated_vars)
{
  char command[MAX_WEB_COMMAND_SIZE]={0};
  char output_string[512]={0};
  unsigned int html_output = 1 ;

  //If we have the console argument set this means we dont want the html output enabled so we switch it off
  if ( _GET(&execute_web_command,"console",command,MAX_WEB_COMMAND_SIZE) ) { html_output = 0; }

  execute_web_command.content[0]=0;

  //After receiving the command and if we want html output we just want to redirect back to control.html
  if (html_output) { strcpy(execute_web_command.content,"<html><meta http-equiv=\"refresh\" content=\"0;URL='control.html'\"><body>Executed<br>"); }


  if ( _GET(&execute_web_command,"go",command,MAX_WEB_COMMAND_SIZE) )
             {
                  fprintf(stderr,"Executing command %s from webinterface\n",command);
                  IssueCommandInternal(command,"WEBINTERFACE",output_string,512);
                  strcat(execute_web_command.content,output_string); // Append output
             } else
  if ( _GET(&execute_web_command,"do",command,MAX_WEB_COMMAND_SIZE) )
             {
                  fprintf(stderr,"Executing command %s from webinterface\n",command);
                  IssueCommandInternal(command,"WEBINTERFACE",output_string,512);
                  strcat(execute_web_command.content,output_string); // Append output
             }


  if (html_output) {  strcat(execute_web_command.content,"</body></html>"); }


  // We signal the size of execute_web_command.content
  execute_web_command.content_size=strlen(execute_web_command.content);
  return 0;
}

//This function prepares the content of  camera_feed_image context , ( camera_feed_image.content ) whenever a camera image is requested
void * prepare_camera_feed_content_callback(unsigned int associated_vars)
{
  char feed[123]={0};
  if ( _GET(&camera_feed_image,"feed",feed,123) )
             {
                 camera_feed_image.content_size = JPEG_MAX_FILE_SIZE_IN_BYTES; // This to indicate what is the maximum size..!
                 if ( strcmp(feed,"left")==0 ) {  VisCortX_SaveVideoRegisterToJPEGMemory(LEFT_EYE,camera_feed_image.content,&camera_feed_image.content_size); } else
                 if ( strcmp(feed,"right")==0 ) {  VisCortX_SaveVideoRegisterToJPEGMemory(RIGHT_EYE,camera_feed_image.content,&camera_feed_image.content_size); } else
                 if ( strcmp(feed,"depth_left")==0 ) {  VisCortX_SaveVideoRegisterToJPEGMemory(DEPTH_LEFT_VIDEO,camera_feed_image.content,&camera_feed_image.content_size); } else
                 if ( strcmp(feed,"depth_right")==0 ) {  VisCortX_SaveVideoRegisterToJPEGMemory(DEPTH_RIGHT_VIDEO,camera_feed_image.content,&camera_feed_image.content_size); } else
                                                  {
                                                    //Incorrect feed id
                                                    fprintf(stderr,"Error sending feed.jpg , incorrect feed id %s \n",feed);
                                                    camera_feed_image.content_size =  0;
                                                  }

             } else
             {
                  camera_feed_image.content[0] = 0;
                  camera_feed_image.content_size =  0;
             }
  return 0;
}

//This function prepares the content of  camera_feed_page context , ( execute_web_command.content ) whenever the index page is requested
void * prepare_camera_page_content_callback(unsigned int associated_vars)
{
  //After receiving the command we just want to redirect back to control.html
  strcpy(camera_feed_page.content,"<html><meta http-equiv=\"refresh\" content=\"1\"><body>");


  char feed[123]={0};
  if ( _GET(&camera_feed_page,"feed",feed,123) )
             {
               strcat(camera_feed_page.content,"<div style=\"vertical-align: center; text-align: center; width: 100%%; height: 100%%;\"><img src=\"feed.jpg?feed=");
               strcat(camera_feed_page.content,feed); // This is not very safe to be dropped raw on the html page returned.. :P On the other hand it will only be visible on the person who sent it so who cares..
               strcat(camera_feed_page.content,"\"></div>");
             }  else
             {
               strcat(camera_feed_page.content,"<div style=\"vertical-align: center; text-align: center; width: 100%%; height: 100%%;\"><img src=\"empty_feed.jpeg\"></div>");
             }

  strcat(camera_feed_page.content,"</body></html>");

  // We signal the size of camera_feed_page.content
  camera_feed_page.content_size=strlen(camera_feed_page.content);
  return 0;
}

//This function adds a Resource Handler for the page index.html and its callback function
void init_dynamic_content()
{
  //We create a virtual file called "index.html" , when this gets requested our prepare_helloworld_content_callback gets called!
  if (! AmmServer_AddResourceHandler(&execute_web_command,"/execute.html",webserver_root,4096,0,&prepare_execute_web_command_content_callback) ) { fprintf(stderr,"Failed adding execute page\n"); }
  AmmServer_DoNOTCacheResourceHandler(&execute_web_command);

  if (! AmmServer_AddResourceHandler(&camera_feed_image,"/feed.jpg",webserver_root,JPEG_MAX_FILE_SIZE_IN_BYTES,100 /*MS second cooldown*/,&prepare_camera_feed_content_callback) ) { fprintf(stderr,"Failed adding execute page\n"); }
  AmmServer_DoNOTCacheResourceHandler(&camera_feed_image);

  if (! AmmServer_AddResourceHandler(&camera_feed_page,"/feed.html",webserver_root,4096,0,&prepare_camera_page_content_callback) ) { fprintf(stderr,"Failed adding execute page\n"); }
  AmmServer_DoNOTCacheResourceHandler(&camera_feed_page);

}

//This function destroys all Resource Handlers and free's all allocated memory..!
void close_dynamic_content()
{
   // AmmServer_RemoveResourceHandler(&helloworld,1);
    AmmServer_RemoveResourceHandler(&execute_web_command,1);
    AmmServer_RemoveResourceHandler(&camera_feed_image,1);
}
/*! Dynamic content code ..! END ------------------------*/


int StartEmbeddedWebInterface()
{
   if (DISABLE_EMBEDDED_WEB_INTERFACE)
   {
    printf("Ammar Server binding is currently disabled..\n");
    return 0;
   }

  char * env_directory = get_environment_robot_directory();
  strcpy(webserver_root,env_directory);
  strcat(webserver_root,"permfs/public_html");

  strcpy(templates_root,env_directory);
  strcat(templates_root,"permfs/public_html/templates");


    printf("Ammar Server starting up\n");

   //Kick start AmmarServer , bind the ports , create the threads and get things going..!
   
   // If i ever change the port from 8080 to 80 I will have to also change the guarddog Script to redirect wget requests to the correct port..!
   AmmServer_Start("0.0.0.0",8080,0,webserver_root,templates_root);

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
   if (DISABLE_EMBEDDED_WEB_INTERFACE)
   {
    printf("Ammar Server binding is currently disabled..\n");
    return 0;
   }
    //Delete dynamic content allocations and remove stats.html and formtest.html from the server
    close_dynamic_content();

    //Stop the server and clean state
    AmmServer_Stop();

    return 1;
}
