#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/uio.h>
#include <regex.h>
#include <pthread.h>

#include "network_framework.h"
#include "../VideoInput.h"

char * peer_feed;

int network_receive_stop=0;
pthread_t network_receive_loop_id=0;
void * NetworkReceiveLoop(void *ptr );

int network_transmit_stop=0;
pthread_t network_transmit_loop_id=0;
void * NetworkTransmitLoop(void *ptr );

char peer_ip[200]={0};


struct TransmitThreadPassParam
{
  char ip[123];
  unsigned int port;
};



/******** TRANSMIT_MY_IMAGE() *********************
 There is a separate instance of this function
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void TransmitMyImage (int sock)
{

   unsigned int remaining_file = 320*240*3;
   void *frame=GetFrame(0);
   while (!network_transmit_stop)
    {
      int n = write(sock,frame,remaining_file);
      if ( n <0 )
        {
            fprintf(stderr,"Transmitting socket died apparently\n ");
            return ;
        } else
      if ( frame+n >= GetFrame(0)+remaining_file )
        {
            frame=GetFrame(0);
            remaining_file = 320*240*3;
            sleep(100);
        } else
        {
            frame+=n;
            remaining_file-=n;
        }

    }


}


void ReceivePeerImage (int sock)
{
   unsigned int remaining_file = 320*240*3;
   while (!network_receive_stop)
    {
      int n = read(sock,peer_feed,remaining_file);
      sleep(100);
      if ( n <0 )
        {
          fprintf(stderr,"Transmitting socket died apparently\n ");
          return ;
        } else
       if ( n < remaining_file )
        {

        }
    }


   /*
   int n;
   char buffer[256];

   bzero(buffer,256);
   n = read(sock,buffer,255);
   if (n < 0) error("ERROR reading from socket");
   printf("Here is the message: %s\n",buffer);
   n = write(sock,"I got your message",18);
   if (n < 0) error("ERROR writing to socket");*/
}



void error(char * msg )
{
  fprintf(stderr,"%s\n",msg);
}





void * NetworkTransmitLoop(void *ptr )
{
    struct TransmitThreadPassParam *param;
    param = (struct TransmitThreadPassParam *) ptr;


    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;


    portno = 1234;// (int) param->port;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { error("ERROR opening socket"); }

    server = gethostbyname(peer_ip);
    if (server == NULL) {
                               fprintf(stderr,"ERROR, no such host (%s) \n",peer_ip);
                               exit(0);
                        }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
     { error("ERROR connecting"); } else
     {
        TransmitMyImage (sockfd);
     }

    close(sockfd);
   return 0;
}





void * NetworkReceiveLoop(void *ptr )
{

    int sockfd, newsockfd, portno, pid;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;

     portno = 1234;

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) { return 0; error("ERROR opening socket"); }
     bzero((char *) &serv_addr, sizeof(serv_addr));


     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) { return 0; error("ERROR on binding");  }
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     while (1) {
         newsockfd = accept(sockfd,
               (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0) { return 0; error("ERROR on accept");  }

         pid = fork();
         if (pid < 0) { return 0; error("ERROR on fork"); }


         if (pid == 0)
         {
             close(sockfd);
             TransmitMyImage(newsockfd);
             exit(0);
         }
         else close(newsockfd);
     }
     close(sockfd);

return 0;
}



int StartupNetworkServer()
{
     network_receive_stop=0;
     if ( pthread_create( &network_receive_loop_id , NULL,  NetworkReceiveLoop ,0) != 0 )
     {
         fprintf(stderr,"Error creating network receive loop \n");
         return 0;
     }
     return 1;
}



int StartupNetworkClient(char * ip,unsigned int port)
{
     network_transmit_stop=0;

     struct TransmitThreadPassParam param={0};
     strcpy(param.ip,ip);
     strcpy(peer_ip,ip);
     param.port = port;

     fprintf(stderr,"Starting NetClient %s:%u\n",ip,port);

     if ( pthread_create( &network_transmit_loop_id , NULL,  NetworkTransmitLoop ,(void*) &param) != 0 )
     {
         fprintf(stderr,"Error creating network transmit loop \n");
         return 0;
     }
     return 1;
}
