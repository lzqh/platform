#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "test_common.h"


#include "reactor.h"

reactor::reactor g_reactor;
const size_t buffer_size = 1024;
char read_buff[buffer_size];
char write_buff[buffer_size];

class client :public event_handler{
public:
	client()
	{
		m_handle = socket(AF_INET, SOCK_STREAM, 0);
		//assert(is_valid_handle(m_handle));
	}
	~client()
	{
		close(m_handle);
	}

	int connet_server(const char *ip, unsigned short port)
	{
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(ip);
		if(connect(m_handle, (struct sockaddr *)&addr, sizeof(addr))<0)
		{
			printf("connent failed !\n");
			return false;
		}
		return true;
	}
	void  read_event()
	{
		memset(read_buff, 0, buffer_size);
		int len  = recv(m_handle, read_buff, buffer_size, 0);
		if(len >0)
		{
			fprintf(stderr, "%s", read_buff);
		}
		else
		{
			printf("recv err");

		}
	}
	void write_event()
	{
		memset(write_buff, 0, buffer_size);
		int len = sprintf(write_buff, "hello world! \r\n");
		len = send(m_handle, write_buff, len, 0);
		if(len >0)
		{
			fprintf(stderr, "%s", write_buff);
		}
		else
		{
			printf("send err");

		}
	}

	handle_t get_handle()
	{
		return handle;
	}


private:
	handle_t m_handle;
};

int main(int arg,char **argv)
{
	if(argc <3)
	{
		fprintf(stderr, "usage: %s ip port\n", argv[0]);
		return EXIT_FAILURE;
	}
	client m_client;
	if(!m_client.connet_server(argv[1], atoi(argv[2])))
	{
		 fprintf(stderr, "connect remote server failed\n");
		 return EXIT_FAILURE;
	}
	g_reactor.register_handler(reactor::EV_WRITE, &m_client);
	while(1)
	{
		g_reactor.handle_events();
		sleep(1);
	}
	g_reactor.remove_handler(&m_client);
	return EXIT_SUCCESS;
}



