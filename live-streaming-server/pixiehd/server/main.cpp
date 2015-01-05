#include "include/globals.hpp"
#include "include/Session.hpp"

int sockfd;
std::vector<cfa::Session *> sessions;

int main(int argc, char**argv)
{
   struct sockaddr_in servaddr,cliaddr;
   socklen_t len;
   char mesg[512];
   int n;

   sockfd=socket(AF_INET,SOCK_DGRAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
   servaddr.sin_port=htons(4444);
   bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

   for (;;)
   {
      len = sizeof(cliaddr);
      n = recvfrom(sockfd,mesg,512,0,(struct sockaddr *)&cliaddr,&len);
      //sendto(sockfd,mesg,n,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
      printf("-------------------------------------------------------\n");
      mesg[n] = 0;
      printf("Received the following:\n");
      printf("%s",mesg);
      cfa::dumpHex(mesg, n);
      cfa::parseClientPacket(mesg, n, cliaddr);
      printf("\n-------------------------------------------------------\n");
   }
}
