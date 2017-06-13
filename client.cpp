#include <stdlib.h>
#include <stdio.h>

#include "reactor.h"



class client :public event_handler{
public:
	client();
	~client();

	int connet_server()
	{

	}
	void  read_event()
	{

	}
	void write_event()
	{

	}

	handle_t get_handle()
	{
		return handle;
	}


private:
	handle_t handle;
};

int main(arg,argv)
{

}



