#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>

#define MAX_HOSTNAME 255

void error(char *msg)
{
 perror(msg);
 exit(0); 
}

int main(int argc, char *argv[])
{
 int sock, n, ret;
 socklen_t length, length2;
 struct sockaddr_in server;
 struct sockaddr_in from;
 struct hostent *hp;
 char buffer[1024], hostbuffer[MAX_HOSTNAME+1];
 
 
 sock = socket(AF_INET, SOCK_DGRAM, 0);
 
 if(sock<0)
 {
  error("socket");
 }
 
 server.sin_family = AF_INET;
 
 hp = gethostbyname("localhost");

 if(hp==0)
 {
  error("Unknown Host");
 }

 bcopy((char *)hp->h_addr,(char *)&server.sin_addr,hp->h_length);
 
 server.sin_port = htons(1030);
 length = sizeof(server);
 length2 = sizeof(from);
 
 
 //gethostname Function
 ret = gethostname(hostbuffer, MAX_HOSTNAME);
 if(ret == 0)
 {
  printf("Host name is %s\n", hostbuffer);
 } 
 //End of gethostname
 
 
 //getsockname Function
 ret = getsockname(sock, (struct sockaddr_in *)&from, &length2);

 if(ret == 0)
 {
  printf("Local Address: %s\n", inet_ntoa(from.sin_addr));
  printf("Local Port: %d\n", from.sin_port);
 }
 //End of getsockname
 
 
 printf("Please enter the message: ");
 
 bzero(buffer,1024);
 fgets(buffer,1023,stdin);
 n = sendto(sock,buffer,strlen(buffer), 0, &server, length);
 if(n<0)
 {
  error("Sendto");
 }
 
 n = recvfrom(sock,buffer,1024,0,&from,&length);
 if(n<0)
 {
  error("recvfrom");
 }
 
 
 //getpeername Function
 
 ret = getpeername(sock, (struct sockaddr_in *)&server, &length);
 
 if(ret == 0)
 {
  printf("Peer Address: %s\n", inet_ntoa(server.sin_addr));
  printf("Peer Port: %d\n", server.sin_port);
 }
 
 //End of getpeername
 
 
 //gethostbyname Function
 
 hp = gethostbyname("localhost");
 
 if(hp)
  printf("h_name: %s", hp->h_name);
  printf("\n");
  printf("h_addrtype: %d", hp->h_addrtype);
  printf("\n");
  printf("Address: ");
  int i = 0;
  while(1)
  {
   if(hp->h_addr_list[i])
   {
    struct in_addr theAddr;
    memcpy(&theAddr.s_addr, hp->h_addr_list[i], sizeof(theAddr.s_addr));
    printf(" h_addr_list[%d} = %s\n",i, inet_ntoa(theAddr));
    i++;
   }
   else
    break;
  }
 
 //End of gethostbyname


 //gethostbyaddr function
 
 struct in_addr xy;
 struct hostent *yx;
 
 inet_aton("127.0.0.1", &xy);
 if(yx = gethostbyaddr((char*)&xy.s_addr, sizeof(xy.s_addr),AF_INET))
  printf("Host name is %s\n", yx->h_name);
 
 //end of gethostbyaddr


 //getservbyname Function
 
  struct servent *sp;
  sp = getservbyname("http","tcp");
  if(sp)
   printf("(By Name)Service %s is at port %d\n",sp->s_name, 
ntohs(sp->s_port));
  
 //
 
 //getservbyport Function
 
 struct servent *sd;
 
 sd = getservbyport(htons(80),NULL);
 if(sd)
  printf("(By Port)Service %s is at port %d\n",sd->s_name, 
ntohs(sd->s_port));
 
 //End of getservbyport

 write(1,"\nConverted: ",16);
 write(1,buffer,n);
 write(1,"\n",1); 


}
