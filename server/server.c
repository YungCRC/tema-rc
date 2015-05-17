#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <stdio.h>
#include <signal.h>

void intHandler();

void error(const char *);

int sockfd, newsockfd;

int main(int argc, char *argv[])
{
   int servlen, n;
   socklen_t clilen;
   struct sockaddr_un  cli_addr, serv_addr;
   char buf[80];

   signal (SIGINT, intHandler);

   if ((sockfd = socket(AF_UNIX,SOCK_STREAM,0)) < 0)
       error("creating socket");
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sun_family = AF_UNIX;
   strcpy(serv_addr.sun_path, argv[1]);
   servlen=strlen(serv_addr.sun_path) + 
                     sizeof(serv_addr.sun_family);
   if(bind(sockfd,(struct sockaddr *)&serv_addr,servlen)<0)
       error("binding socket"); 

   listen(sockfd,5);
   clilen = sizeof(cli_addr);
   newsockfd = accept(
        sockfd,(struct sockaddr *)&cli_addr,&clilen);
   if (newsockfd < 0) 
        error("accepting");
   while (1)
     {
       n=read(newsockfd,buf,80);
       /* printf("A connection has been established\n"); */
       write(1, "Client>", 7);
       write(1,buf,n);
       write(1, "\n", 1);
       bzero (buf, 80);
       write(1, "Server>", 7);
       fgets (buf, 80, stdin);
       write(1, "\n", 1);
       write (newsockfd, buf, sizeof(buf));
       /* write(newsockfd,"I got your message\n",19); */
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
    printf("\nCaught interrupt\nExiting\n");
    close(newsockfd);
    close(sockfd);
    exit(0);
}
