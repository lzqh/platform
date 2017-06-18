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
#include "reactor.h"

 #include <unistd.h>


  #include <fcntl.h>
 #include <sys/shm.h>
 #include <netinet/in.h>
reactorer::reactor g_reactor;
//#define g_reactor (*(sGlobal->g_reactor_ptr))

const size_t kBufferSize = 1024;
char g_read_buffer[kBufferSize];
char g_write_buffer[kBufferSize];

class request_handler : public   reactorer::event_handler{
public:
	request_handler(reactorer:: handle_t handle):
		event_handler(),
		m_handle(handle)
	{
	}
	~request_handler()
	{
	}
	virtual reactorer::handle_t get_handle() const
	{
		return m_handle;
	}
	virtual void write_event()
	{

	}
	virtual void read_event()
	{
		memset(g_read_buffer, 0, sizeof(g_read_buffer));
		int len = recv(m_handle, g_read_buffer, kBufferSize, 0);
		 if (len > 0)
		        {
		            if (strncasecmp("time", g_read_buffer, 4) == 0)
		            {
		            	fprintf(stderr,"%s",g_read_buffer);
		            	memset(g_read_buffer, 0, sizeof(g_read_buffer));
		                //g_reactor.RegisterHandler(this, reactor::kWriteEvent);
		            }
		            else if (strncasecmp("exit", g_read_buffer, 4) == 0)
		            {
		                close(m_handle);
		                g_reactor.remove_handler(this);
		                delete this;
		            }
		            else
		            {
		                fprintf(stderr, "Invalid request: %s", g_read_buffer);
		                close(m_handle);
		                g_reactor.remove_handler(this);
		                delete this;
		            }
		        }
		 else
		 {
			 fprintf(stderr, "%s error: %s\n", "recv", strerror(errno));
		 }

		/*
		if (len <0)
		{
			printf("server recv err \n");
			close(m_handle);
			g_reactor.remove_handler(this);
			delete this;
			//sprintf(stderr,"%s",g_read_buffer);
		}
		else
		{
			fprintf(stderr,"%s",g_read_buffer);
			//memset(g_read_buffer, 0, sizeof(g_read_buffer));

		}*/
	}
private:
	reactorer::handle_t m_handle;

};
class server: public reactorer::event_handler{
public:
	server(const char * ip, unsigned short port):
		event_handler(),
		m_ip(ip),
		m_port(port)
	{}
	~server()
	{

	}
	bool start()
	{
		printf("in start \n ");
		m_handle = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(m_port);
		addr.sin_addr.s_addr = inet_addr(m_ip.c_str());
		if(bind(m_handle, (struct sockaddr *)&addr, sizeof(addr))<0)
		{
			printf("bind err \n");
			return false;
		}
		if(listen(m_handle,10)<0)
		{
			printf("listen err \n");
			return false;
		}
		return true;
	}
	virtual reactorer::handle_t get_handle() const
	{
		return m_handle;
	}
	virtual void read_event()
	{
		struct sockaddr addr;
		socklen_t addrlen = sizeof(addr);

		printf("in read_event \n");
		//reactorer::handle_t handle = accept(m_handle, (struct sockaddr * )&addr, &addrlen);
		reactorer::handle_t handle  = accept(m_handle, (struct sockaddr * )&addr, &addrlen);
		if(handle  ==-1 )
		{
			printf("server recv err \n");
			//close(handle);
		}
		else
		{
			request_handler  * request = new request_handler(handle);
			if(g_reactor.register_handler(reactorer::EV_READ, request) != 0);
			{
				 fprintf(stderr, "error: register handler failed\n");
				//delete request;
			}
		}

		/*
		int len = recv(handle, g_read_buffer, kBufferSize, 0);
		if (len <0)
		{
			printf("server recv err \n");
			close(handle);
			//sprintf(stderr,"%s",g_read_buffer);

		}
		else
		{
			fprintf(stderr,"%s",g_read_buffer);

		}*/

	}



private:
	reactorer::handle_t m_handle;
	std::string m_ip;
	unsigned short m_port;

};
int main(int argc, char **argv)
{

	 if (argc < 3)
	    {
	        fprintf(stderr, "usage: %s ip port\n", argv[0]);
	        return EXIT_FAILURE;
	    }
	 server m_server(argv[1], atoi(argv[2]));
	 if(!m_server.start())
	 {
		 fprintf(stderr, "start server failed\n");
		 return EXIT_FAILURE;
	 }
	 fprintf(stderr, "server started!\n");
	  //m_server.read_event();
	 while (1)
	{
		g_reactor.register_handler( reactorer::EV_READ,&m_server);
		g_reactor.handle_events();
	   sleep(1);
	 }
	    return EXIT_SUCCESS;
}
