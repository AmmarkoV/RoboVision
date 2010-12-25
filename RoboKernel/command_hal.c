#include "command_hal.h"
#include "command_implementation.h"

#include "../InputParser/InputParser_C.h"

#include "visual_system.h"
#include "motor_system.h"
#include "mapping_system.h"
#include "configuration.h"
#include "webinterface.h"

int keep_snapshots=0;
int last_snapshot_activation=0;
int snapshot_activation_interval=5000;


enum command_id_consts
{
  CMD_UNKNOWN=0,
 /* ------------------ */

  CMD_PANORAMIC,
  CMD_KEEPCOLOR,
  CMD_MOTION_ALARM,
  CMD_SAVE_REGISTER,
  CMD_SWAP_FEEDS,
  CMD_WEB_INTERFACE,
  CMD_REFRESH_MAP_AT_WEB_INTERFACE,
  CMD_DRAW_MOVEMENT,
  CMD_DRAW_FEATURES,
  CMD_FIND_FEATURES,
  CMD_PLAYSOUND,
  CMD_SAY,
  CMD_DEPTHMAP,
  CMD_FORWARD,
  CMD_BACKWARD,
  CMD_LEFT,
  CMD_RIGHT,
  CMD_TOGGLE_AUTO_RECORD_SNAPSHOTS,
  CMD_TOGGLE_AUTO_PLAYBACK_SNAPSHOTS,
  CMD_RECORD_SNAPSHOT,
  CMD_RECORD_COMPRESSED,
  CMD_PLAYBACK_SNAPSHOT,
  CMD_PLAYBACK_LIVE,
  CMD_SENSORS,
  CMD_DEPTHMAP_TO_FILE,
  CMD_DEPTHMAP_IMPORT_TO_MAP,

 /* ------------------ */
  CMD_TOTAL_CONSTS
};



int ExecuteCommandInternal(unsigned int opcode,unsigned int words_count,struct InputParserC * ipc,char * from)
{
  if ( ( opcode == CMD_UNKNOWN ) || (opcode >= CMD_TOTAL_CONSTS) ) { return 0;}

  char outptstr[512]={0};
  char cmds_1[512]={0},cmds_2[512]={0},cmds_3[512]={0},cmds_4[512]={0};
  unsigned int cmdi_1=0,cmdi_2=0,cmdi_3=0,cmdi_4=0;

  InputParser_GetWord(ipc,1,cmds_1,512); cmdi_1=InputParser_GetWordInt(ipc,1);
  if ( words_count > 1 ) InputParser_GetWord(ipc,2,cmds_2,512); cmdi_2=InputParser_GetWordInt(ipc,2);
  if ( words_count > 2 ) InputParser_GetWord(ipc,3,cmds_3,512); cmdi_3=InputParser_GetWordInt(ipc,3);
  if ( words_count > 3 ) InputParser_GetWord(ipc,4,cmds_4,512); cmdi_4=InputParser_GetWordInt(ipc,4);


  switch (opcode)
   { case CMD_UNKNOWN :
       return 0;
     break;
     case CMD_PANORAMIC :
                sprintf(outptstr,"From %s : Panoramic \n",from);
                Panoramic();
     break;
     case CMD_KEEPCOLOR :
                sprintf(outptstr,"From %s : Command Executing Color keeping ( %u , %u , %u ) with threshold %u \n",from,cmdi_1,cmdi_2,cmdi_3,cmdi_4);
                DrawNewPalette(cmdi_1,cmdi_2,cmdi_3,cmdi_4);
     break;
     case CMD_MOTION_ALARM :
                sprintf(outptstr,"From %s : Command Switching Motion Alarm \n",from);
                if (motion_lock_on==1) {motion_lock_on=0; } else
                                       {motion_lock_on=1; }
     break;
     case CMD_SAVE_REGISTER :
                 sprintf(outptstr,"From %s : Command Parser Save Register %u to file : %s\n",from,cmdi_2,cmds_1);
                 VisCortX_SaveVideoRegisterToFile(cmdi_2,cmds_1);
     break;
     case CMD_SWAP_FEEDS :
                 sprintf(outptstr,"From %s : Command Swapping Video Feeds \n",from);
                 if (swap_inputs==1) {swap_inputs=0; } else
                                     {swap_inputs=1; }
     break;
     case CMD_WEB_INTERFACE :
                 sprintf(outptstr,"From %s : Command Switching WebInterface \n",from);
                 if (web_interface==1) {web_interface=0; CloseWebInterface(); } else
                                       {web_interface=1; OpenWebInterface(); }
     break;
     case CMD_DRAW_MOVEMENT :
                 sprintf(outptstr,"From %s : Command Drawing Movement\n",from);
                 DrawMovement();
     break;
     case CMD_DRAW_FEATURES :
                 sprintf(outptstr,"From %s : Command Drawing Features\n",from);
                 DrawFeatures();
     break;
     case CMD_FIND_FEATURES :
                 sprintf(outptstr,"From %s : Command Finding Features\n",from);
                 FindFeatures();
     break;
     case CMD_PLAYSOUND :
                 sprintf(outptstr,"From %s : Command Parser Playing sound : %s\n",from,cmds_1);
                 PlaySound(cmds_1);
     break;
     case CMD_SAY :
                 sprintf(outptstr,"From %s : Command Parser TTS Saying : %s\n",from,cmds_1);
                 Say(cmds_1);
     break;
     case CMD_DEPTHMAP :
                 sprintf(outptstr,"From %s : Command Parser , Performing DepthMap \n",from);
                 FullDepthMap(0);
     break;
     case CMD_FORWARD :
                 sprintf(outptstr,"From %s : Command Parser , Going Up \n",from);
                 RobotMove(20,5000);
     break;
     case CMD_BACKWARD :
                 sprintf(outptstr,"From %s : Command Parser , Going DOWN \n",from);
                 RobotMove(20,-5000);
     break;
     case CMD_LEFT :
                 sprintf(outptstr,"From %s : Command Parser , Going LEFT \n",from);
                 RobotRotate(20,10);
     break;
     case CMD_RIGHT :
                 sprintf(outptstr,"From %s : Command Parser , Going RIGHT \n",from);
                 RobotRotate(20,(-1)*10);
     break;
     case CMD_TOGGLE_AUTO_RECORD_SNAPSHOTS :
                  if ( ( cmdi_1 == 0 ) && ( keep_snapshots == 0 ) )
                   {
                     sprintf(outptstr,"From %s : Snapshots stream already disabled \n",from);
                   } else
                  if ( ( cmdi_1 == 0 ) && ( keep_snapshots != 0 ) )
                   {
                      sprintf(outptstr,"From %s : Switching off snapshots due to 0 command \n",from);
                      keep_snapshots=0;
                   }
                      else
                  if ( keep_snapshots == 0 )
                    { sprintf(outptstr,"From %s : Toggle auto timestamped snapshots is enabled ( %u milliseconds ) \nSHOULD BE IMPLEMENTED ON VIDEO INPUT AND NOT ROBOKERNEL\n",from,cmdi_1);
                      keep_snapshots = 1;
                      if ( cmdi_1 >= 500 ) { snapshot_activation_interval=cmdi_1; }
                    } else
                    { sprintf(outptstr,"From %s : Toggle auto timestamped snapshots is disabled \n",from); keep_snapshots = 0; }
     break;
     case CMD_TOGGLE_AUTO_PLAYBACK_SNAPSHOTS :
         sprintf(outptstr,"From %s : Auto Playback not implemented yet \n",from);
     break;

     case CMD_RECORD_COMPRESSED :
                 sprintf(outptstr,"From %s : Changing Recording Compression setting to %u \n",from,cmdi_1);
                 CompressRecordWithImageMagick(cmdi_1);
     break;

     case CMD_RECORD_SNAPSHOT :
                 if ( cmdi_1 == 0 )
                 {
                   sprintf(outptstr,"From %s : Capturing VideoInput Snapshot without a timestamp in the filename (snapshot.ppm) \n",from);
                   RecordOne((char *)"snapshot");
                 } else
                 {
                   sprintf(outptstr,"From %s : Capturing VideoInput Snapshot (param , %u)\n",from,cmdi_1);
                   SnapshotWithTimeStamp();
                 }
     break;
     case CMD_PLAYBACK_SNAPSHOT :
                 IssueCommandInternal((char *) "TOGGLE AUTO RECORD SNAPSHOTS(0)",from); /*Internal message to stop recording of Streams*/

                 sprintf(outptstr,"From %s : PlayingBack VideoInput Snapshot \n",from);
                 Play((char *)"memfs/snapshot");
     break;
     case CMD_PLAYBACK_LIVE :
                 IssueCommandInternal((char *) "TOGGLE AUTO RECORD SNAPSHOTS(0)",from); /*Internal message to stop recording of Streams*/

                 sprintf(outptstr,"From %s : PlayingBack VideoInput Cam Input \n",from);
                 Stop();
     break;

     case CMD_SENSORS :
                 sprintf(outptstr,"From %s : Ultrasonic(%u,%u) Accelerometers(%u,%u) \n",from,RobotGetUltrasonic(0),RobotGetUltrasonic(1),RobotGetAccelerometerX(0),RobotGetAccelerometerY(0));
     break;

     case CMD_DEPTHMAP_TO_FILE :
                 sprintf(outptstr,"From %s : Depth map is stored to File for 3D viewing \n",from);
                 VisCortX_SaveVideoRegisterToFile(DEPTH_LEFT_VIDEO,"DEPTH0");
                 VisCortX_SaveVideoRegisterToFile(LEFT_EYE,"COLOR0");
     break;


     case CMD_REFRESH_MAP_AT_WEB_INTERFACE :
                 sprintf(outptstr,"From %s : Refreshing HTML map! \n",from);
                 RobotPrintPosition();
     break;

     case CMD_DEPTHMAP_IMPORT_TO_MAP :
                 sprintf(outptstr,"From %s : Scanning depth to map! \n",from);
                 PassDepthToMap();
     break;


     default :
       return 0;
     break;
   };

   WriteConsoleOutput(outptstr);

  return 1;
}

int IssueCommandInternal(char * command,char * from)
{
  struct InputParserC * ipc=0;
  ipc = InputParser_Create(512,5);

  unsigned int chosen_command=CMD_UNKNOWN;

  unsigned int words_count = InputParser_SeperateWords(ipc,command,0);
    if ( words_count > 0 )
    {
      /* NEEDS CARE BECAUSE ITS STRING , MUST HAVE ITS LENGTH AS A PARAMETER :P*/
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"PANORAMIC",9)==1) { chosen_command=CMD_PANORAMIC; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"KEEPCOLOR",9)==1) { chosen_command=CMD_KEEPCOLOR; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"MOTION ALARM",12)==1) { chosen_command=CMD_MOTION_ALARM; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"SAVE REGISTER",13)==1) { chosen_command=CMD_SAVE_REGISTER; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"SWAP FEEDS",10)==1) { chosen_command=CMD_SWAP_FEEDS; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"WEB INTERFACE",13)==1) { chosen_command=CMD_WEB_INTERFACE; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"DRAW MOVEMENT",13)==1) { chosen_command=CMD_DRAW_MOVEMENT; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"DRAW FEATURES",13)==1) { chosen_command=CMD_DRAW_FEATURES; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"FIND FEATURES",13)==1) { chosen_command=CMD_FIND_FEATURES; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"PLAYSOUND",9)==1) { chosen_command=CMD_PLAYSOUND; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"SAY",3)==1) { chosen_command=CMD_SAY;} else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"DEPTH MAP",9)==1) { chosen_command=CMD_DEPTHMAP; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"FORWARD",7)==1) { chosen_command=CMD_FORWARD; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"BACKWARD",8)==1) { chosen_command=CMD_BACKWARD; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"LEFT",4)==1) { chosen_command=CMD_LEFT; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"RIGHT",5)==1) { chosen_command=CMD_RIGHT; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"TOGGLE AUTO RECORD SNAPSHOTS",28)==1) { chosen_command=CMD_TOGGLE_AUTO_RECORD_SNAPSHOTS; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"TOGGLE AUTO PLAYBACK SNAPSHOTS",30)==1) { chosen_command=CMD_TOGGLE_AUTO_PLAYBACK_SNAPSHOTS; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"RECORD SNAPSHOT",15)==1) { chosen_command=CMD_RECORD_SNAPSHOT; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"RECORD COMPRESSED",17)==1) { chosen_command=CMD_RECORD_COMPRESSED; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"PLAYBACK SNAPSHOT",17)==1) { chosen_command=CMD_PLAYBACK_SNAPSHOT; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"PLAYBACK LIVE",13)==1) { chosen_command=CMD_PLAYBACK_LIVE; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"SENSORS",7)==1) { chosen_command=CMD_SENSORS; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"DEPTH MAP TO FILE",17)==1) { chosen_command=CMD_DEPTHMAP_TO_FILE; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"REFRESH MAP",11)==1) { chosen_command=CMD_REFRESH_MAP_AT_WEB_INTERFACE; } else
      if (InputParser_WordCompareNoCase(ipc,0,(char*)"DEPTH MAP IMPORT TO MAP",23)==1) { chosen_command=CMD_DEPTHMAP_IMPORT_TO_MAP; } else


        /*
         * >>>>>>>>>>>>>>>>>>>>>>>>>>!!!WRONG COMMAND!!!<<<<<<<<<<<<<<<<<<<<<<<<
         */
        {
           char outptstr[512]={0};
           char cmds_1[512]={0};
           InputParser_GetWord(ipc,1,cmds_1,512);
           sprintf(outptstr,"Wrong command from %s \n",from);
           WriteConsoleOutput(outptstr);
           return 0;
        }
    }
    ExecuteCommandInternal(chosen_command,words_count,ipc,from);
    InputParser_Destroy(ipc);

return 1;
}





