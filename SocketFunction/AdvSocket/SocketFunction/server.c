#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#define MAX_HOSTNAME 255

void error(char *msg)
{
 perror(msg);
 exit(0);
}

int main(int argc, char *argv[])
{
 int ret, buffylen, sock, n;
 socklen_t length, fromlen;
 struct sockaddr_in server;
 struct sockaddr_in from;
 char buf[1024], buffy[1024], name[10] = "Message: ";
 char hostbuffer[MAX_HOSTNAME+1]; 

 sock=socket(AF_INET, SOCK_DGRAM, 0);
 if(sock < 0)
 {
  error("Opening socket");
 }

 strcpy(hostbuffer, "Syahrul");
 
 //sethostname function
 ret = sethostname(hostbuffer, strlen(hostbuffer));

 if(ret == 0)
 {
  printf("Host Name: %s\n", hostbuffer);
 }
 //End of sethostname

 length = sizeof(server);
 
 bzero(&server,length);
 server.sin_family=AF_INET;
 /*I am using INADDR_ANY to set its IP to localhost*/
 server.sin_addr.s_addr=INADDR_ANY;
 server.sin_port=htons(1030);
 if (bind(sock,(struct sockaddr *)&server,length)<0)
 {
  error("binding");
 }
 fromlen = sizeof(struct sockaddr_in);
 
 //getsockname Function
 ret = getsockname(sock, (struct sockaddr_in *)&server, &length);

 if (ret == 0)
 {
  printf("Local Address: %s\n", inet_ntop(server.sin_addr));
  printf("Local Port: %d\n", server.sin_port); 
 }
 //End of getsockname

// while(1)
// {
  n = recvfrom(sock, buf, 1024, 0,(struct sockaddr 
*)&from,&fromlen);
  if(n<0)
  {
   error("recvfrom");
  }
  
  
  //getpeername Function
  ret = getpeername(sock, (struct sockaddr *)&from, &fromlen);
  
  if(ret == 0)
  {
   printf("Peer Address: %s\n", inet_ntop(from.sin_addr));
   printf("Peer Port: %d\n", from.sin_port);
  }
  //End of getpeername  

 
  write(1,name, strlen(name));
  for(int i = 0; i < n; i++)
	buffy[i] = toupper(buf[i]);
  write(1,buf,n);
  write(1,"\n",1);
  n = sendto(sock,buffy,strlen(buffy),0,&from,fromlen);
  if(n<0)
  {
   error("sendto");
  }
// }
  
}
