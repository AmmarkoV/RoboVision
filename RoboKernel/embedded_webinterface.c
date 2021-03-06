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
#include "configuration.h"

#include "../AmmarServer/src/AmmServerlib/AmmServerlib.h"
#include "embedded_webinterface.h"
#include "visual_system.h"
#include "command_hal.h"

#define ENABLE_PASSWORD_PROTECTION 1
#define MAX_WEB_COMMAND_SIZE 512


char webserver_root[512]="../robot/permfs/public_html";
char templates_root[512]="../AmmarServer/public_html/templates/";


/*! Dynamic content code ..! START!*/
/* This is the dynamic Hello World example , as shown here : https://github.com/AmmarkoV/AmmarServer/wiki/Howto */

struct AmmServer_Instance * guarddog_web_intf= 0 ;

//The decleration of hello world dynamic content resources..
struct AmmServer_RH_Context execute_web_command={0};

struct AmmServer_RH_Context camera_feed_image_LEFT={0};
struct AmmServer_RH_Context camera_feed_image_RIGHT={0};
struct AmmServer_RH_Context camera_feed_image_PROC1={0};
struct AmmServer_RH_Context camera_feed_image_PROC2={0};


unsigned int helloworld_times_shown=0;
unsigned long JPEG_MAX_FILE_SIZE_IN_BYTES = 100 /*KB*/ * 1024;


//This function prepares the content of  execute_web_command context , ( execute_web_command.content ) whenever the index page is requested
void * prepare_execute_web_command_content_callback(struct AmmServer_DynamicRequest * rqst)
{
  char command[MAX_WEB_COMMAND_SIZE]={0};
  char output_string[512]={0};
  unsigned int html_output = 1 ;

  //If we have the console argument set this means we dont want the html output enabled so we switch it off
  if ( _GET(guarddog_web_intf,rqst,"console",command,MAX_WEB_COMMAND_SIZE) ) { html_output = 0; }

  rqst->content[0]=0;

  //After receiving the command and if we want html output we just want to redirect back to control.html
  if (html_output) { strcpy(rqst->content,"<html><meta http-equiv=\"refresh\" content=\"0;URL='control.html'\"><body>Executed<br>"); }


  if ( _GET(guarddog_web_intf,rqst,"go",command,MAX_WEB_COMMAND_SIZE) )
             {
                  fprintf(stderr,"Executing command %s from webinterface\n",command);
                  IssueCommandInternal(command,"WEBINTERFACE",output_string,512);
                  strcat(rqst->content,output_string); // Append output
             } else
  if ( _GET(guarddog_web_intf,rqst,"do",command,MAX_WEB_COMMAND_SIZE) )
             {
                  fprintf(stderr,"Executing command %s from webinterface\n",command);
                  IssueCommandInternal(command,"WEBINTERFACE",output_string,512);
                  strcat(rqst->content,output_string); // Append output
             }


  if (html_output) {  strcat(rqst->content,"</body></html>"); }


  // We signal the size of execute_web_command.content
  rqst->contentSize=strlen(rqst->content);
  return 0;
}


void * prepare_camera_feed_content_LEFT(struct AmmServer_DynamicRequest * rqst)
{
  rqst->contentSize = JPEG_MAX_FILE_SIZE_IN_BYTES; // This to indicate what is the maximum size..!
  VisCortX_SaveVideoRegisterToJPEGMemory(LEFT_EYE,rqst->content,&rqst->contentSize);
  return 0;
}

void * prepare_camera_feed_content_RIGHT(struct AmmServer_DynamicRequest * rqst)
{
  rqst->contentSize = JPEG_MAX_FILE_SIZE_IN_BYTES; // This to indicate what is the maximum size..!
  VisCortX_SaveVideoRegisterToJPEGMemory(RIGHT_EYE,rqst->content,&rqst->contentSize);
  return 0;
}

void * prepare_camera_feed_content_PROC1(struct AmmServer_DynamicRequest * rqst)
{
  rqst->contentSize = JPEG_MAX_FILE_SIZE_IN_BYTES; // This to indicate what is the maximum size..!
  VisCortX_SaveVideoRegisterToJPEGMemory(DEPTH_LEFT_VIDEO,rqst->content,&rqst->contentSize);
  return 0;
}

void * prepare_camera_feed_content_PROC2(struct AmmServer_DynamicRequest * rqst)
{
  rqst->contentSize = JPEG_MAX_FILE_SIZE_IN_BYTES; // This to indicate what is the maximum size..!
  VisCortX_SaveVideoRegisterToJPEGMemory(DEPTH_RIGHT_VIDEO,rqst->content,&rqst->contentSize);
  return 0;
}



//This function adds a Resource Handler for the page index.html and its callback function
void init_dynamic_content()
{
  //We create a virtual file called "index.html" , when this gets requested our prepare_helloworld_content_callback gets called!
  if (! AmmServer_AddResourceHandler(guarddog_web_intf,&execute_web_command,"/execute.html",webserver_root,4096,0,&prepare_execute_web_command_content_callback,SAME_PAGE_FOR_ALL_CLIENTS) ) { fprintf(stderr,"Failed adding execute page\n"); }
  AmmServer_DoNOTCacheResourceHandler(guarddog_web_intf,&execute_web_command);

    if (! AmmServer_AddResourceHandler(guarddog_web_intf,&camera_feed_image_LEFT,"/feed_left.jpg",webserver_root,JPEG_MAX_FILE_SIZE_IN_BYTES,100 /*MS second cooldown*/,&prepare_camera_feed_content_LEFT,SAME_PAGE_FOR_ALL_CLIENTS) ) { fprintf(stderr,"Failed adding execute page\n"); }
   AmmServer_DoNOTCacheResourceHandler(guarddog_web_intf,&camera_feed_image_LEFT);

    if (! AmmServer_AddResourceHandler(guarddog_web_intf,&camera_feed_image_RIGHT,"/feed_right.jpg",webserver_root,JPEG_MAX_FILE_SIZE_IN_BYTES,100 /*MS second cooldown*/,&prepare_camera_feed_content_RIGHT,SAME_PAGE_FOR_ALL_CLIENTS) ) { fprintf(stderr,"Failed adding execute page\n"); }
   AmmServer_DoNOTCacheResourceHandler(guarddog_web_intf,&camera_feed_image_RIGHT);

    if (! AmmServer_AddResourceHandler(guarddog_web_intf,&camera_feed_image_PROC1,"/feed_proc1.jpg",webserver_root,JPEG_MAX_FILE_SIZE_IN_BYTES,100 /*MS second cooldown*/,&prepare_camera_feed_content_PROC1,SAME_PAGE_FOR_ALL_CLIENTS) ) { fprintf(stderr,"Failed adding execute page\n"); }
   AmmServer_DoNOTCacheResourceHandler(guarddog_web_intf,&camera_feed_image_PROC1);

    if (! AmmServer_AddResourceHandler(guarddog_web_intf,&camera_feed_image_PROC2,"/feed_proc2.jpg",webserver_root,JPEG_MAX_FILE_SIZE_IN_BYTES,100 /*MS second cooldown*/,&prepare_camera_feed_content_PROC2,SAME_PAGE_FOR_ALL_CLIENTS) ) { fprintf(stderr,"Failed adding execute page\n"); }
   AmmServer_DoNOTCacheResourceHandler(guarddog_web_intf,&camera_feed_image_PROC2);



}

//This function destroys all Resource Handlers and free's all allocated memory..!
void close_dynamic_content()
{
   // AmmServer_RemoveResourceHandler(&helloworld,1);
    AmmServer_RemoveResourceHandler(guarddog_web_intf,&execute_web_command,1);
    AmmServer_RemoveResourceHandler(guarddog_web_intf,&camera_feed_image_LEFT,1);
    AmmServer_RemoveResourceHandler(guarddog_web_intf,&camera_feed_image_RIGHT,1);
    AmmServer_RemoveResourceHandler(guarddog_web_intf,&camera_feed_image_PROC1,1);
    AmmServer_RemoveResourceHandler(guarddog_web_intf,&camera_feed_image_PROC2,1);
}
/*! Dynamic content code ..! END ------------------------*/


int StartEmbeddedWebInterface()
{
  if ( ! Web_Interface_Enabled ) { fprintf(stderr,"Embedded Web Interface is disabled.. ( this means console as well.. )");  return 0; }

  char * env_directory = get_environment_robot_directory();
  strcpy(webserver_root,env_directory);
  strcat(webserver_root,"permfs/public_html");

  strcpy(templates_root,env_directory);
  strcat(templates_root,"permfs/public_html/templates");


    printf("Ammar Server starting up\n");

   //Kick start AmmarServer , bind the ports , create the threads and get things going..!

   // If i ever change the port from 8080 to 80 I will have to also change the guarddog Script to redirect wget requests to the correct port..!
   guarddog_web_intf = AmmServer_Start("robovision","0.0.0.0",8080,0,webserver_root,templates_root);
   if (!guarddog_web_intf) { fprintf(stderr,"Could not start WebInterface .. \n"); return 0; }

    //Create dynamic content allocations and associate context to the correct files
    init_dynamic_content();

    //If we want password protection ( variable defined in the start of this file ) we will have to set a username and a password
    //and then enable password protection
    if (ENABLE_PASSWORD_PROTECTION)
    {
      fprintf(stderr,"\nEnabling password protection\n");
      AmmServer_SetStrSettingValue(guarddog_web_intf,AMMSET_USERNAME_STR,"admin");
      AmmServer_SetStrSettingValue(guarddog_web_intf,AMMSET_PASSWORD_STR,"admin"); //these 2 calls should change BASE64PASSWORD in configuration.c to YWRtaW46YW1tYXI= (or something else)
      /* To avoid the rare race condition of logging only with username and keep a proper state ( i.e. when password hasn't been declared )
         It is best to enable password protection after correctly setting both username and password */
      AmmServer_SetIntSettingValue(guarddog_web_intf,AMMSET_PASSWORD_PROTECTION,1);
    }

    return 1;
}

int EmbeddedWebInterfaceRunning()
{
    if (!Web_Interface_Enabled) { return 0; }
    return AmmServer_Running(guarddog_web_intf);
}

int StopEmbeddedWebInterface()
{
    if (!Web_Interface_Enabled) { return 0; }

    //Delete dynamic content allocations and remove stats.html and formtest.html from the server
    close_dynamic_content();

    //Stop the server and clean state
    AmmServer_Stop(guarddog_web_intf);

    return 1;
}
