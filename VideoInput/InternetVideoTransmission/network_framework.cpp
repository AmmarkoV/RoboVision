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


pthread_t network_loop_id=0;
void * NetworkLoop(void *ptr );

void * KernelLoop(void *ptr )
{

    int sockfd, newsockfd, portno, pid;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;

     int PORT = 1010;

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) { return 0; /*error("ERROR opening socket");*/ }
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(PORT);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) { return 0; /*error("ERROR on binding"); */ }
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     while (1) {
         newsockfd = accept(sockfd,
               (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0) { return 0; /*error("ERROR on accept"); */ }

         pid = fork();
         if (pid < 0) { return 0; /*error("ERROR on fork");*/ }


         if (pid == 0)
         {
             close(sockfd);
             TransmitMyImage(newsockfd);
             exit(0);
         }
         else close(newsockfd);
     }
     close(sockfd);


}


/******** TRANSMIT_MY_IMAGE() *********************
 There is a separate instance of this function
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void TransmitMyImage (int sock)
{
   while (1)
    {
      int n = write(sock,GetFrame(0),320*240*3);
      sleep(100);
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



int StartupNetworkServer()
{
     if ( pthread_create( &network_loop_id , NULL,  NetworkLoop ,(void*) &param) != 0 )
     {
         fprintf(stderr,"Error creating network loop \n");
         return 0;
     }




     return 1;
}
