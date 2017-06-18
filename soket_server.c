#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <string>

 #include <netinet/in.h>
 #include <unistd.h>
 #include <fcntl.h>
 #include <sys/shm.h>

 #define PORT  8887
 #define BUFFER_SIZE 1024

int main()
{
   //soket fd
	int  server_fd= socket(AF_INET, SOCK_STREAM, 0 );
	//定义sockaddr_in
	strcut sockaddr_in serv_addr_in;
	serv_addr_in.sin_family = AF_INET ;
	serv_addr_in.sin_port = htons(PORT);
	serv_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind
	if(!bind(server_fd, (struct sockaddr *)&serv_addr_in, sizeof(serv_addr_in) ))
	{
		perror("bind");
		exit(1);
	}
	if()



	return 0;
}
