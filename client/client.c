#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <signal.h>

void error(const char *);

void intHandler();

int sockfd;
int main(int argc, char *argv[])
{
   int servlen,n;
   struct sockaddr_un  serv_addr;
   char buffer[82];

   signal(SIGINT, intHandler);

   bzero((char *)&serv_addr,sizeof(serv_addr));
   serv_addr.sun_family = AF_UNIX;
   strcpy(serv_addr.sun_path, argv[1]);
   servlen = strlen(serv_addr.sun_path) + 
                 sizeof(serv_addr.sun_family);
   if ((sockfd = socket(AF_UNIX, SOCK_STREAM,0)) < 0)
       error("Creating socket");
   if (connect(sockfd, (struct sockaddr *) 
                         &serv_addr, servlen) < 0)
       error("Connecting");
   /* printf("Please enter your message: "); */
   bzero(buffer,82);
   while (1)
     {
       write(1, "Client>", 7);
       fgets(buffer,80,stdin);
       write(sockfd,buffer,strlen(buffer));
       write(1, "\n", 1);
       n=read(sockfd,buffer,80);
       /* printf("The return message was\n"); */
       write(1, "Server>", 7);
       write(1,buffer,n);
       write(1, "\n", 1);
     }
   return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void intHandler()
{
    printf("\nCaught interrupt\n Exiting\n");
    close(sockfd);
    exit(0);
}

