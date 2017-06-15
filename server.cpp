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

reactor::reactor g_reactor;
//#define g_reactor (*(sGlobal->g_reactor_ptr))

const size_t kBufferSize = 1024;
char g_read_buffer[kBufferSize];
char g_write_buffer[kBufferSize];
class server: public reactor::event_handler{
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
	virtual reactor::handle_t get_handle() const
	{
		return m_handle;
	}
	virtual void reactor::read_event()
	{
		struct sockaddr addr;
		socklen_t addrlen = sizeof(addr);
		reactor::handle_t handle = accept(m_handle, &addr, &addrlen);
		memset(g_read_buffer, 0, sizeof(g_read_buffer));
		int len = recv(m_handle, g_read_buffer, kBufferSize, 0);
		if (len > 0)
		{
			sprintf(stderr,"%s",g_read_buffer);

		}
		else
		{
			printf("server recv err \n");
		}

	}



private:
	handle_t m_hanle;
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
	 server m_server(argv[1], argv[2]);
	 if(!m_server.start())
	 {
		 fprintf(stderr, "start server failed\n");
		 return EXIT_FAILURE;
	 }
	 fprintf(stderr, "server started!\n");
	 while (1)
	    {
	        g_reactor.register_handler(&m_server, reactor::EV_READ);
	        g_reactor.handle_events();
	    }
	    return EXIT_SUCCESS;
}
