#include "reactor.h"

reactor::reactor()
{
	m_demultiplexer = new demultiplexer();
}
reactor::~reactor()
{
	delete m_demultiplexer;
}

int reactor::register_handler(event_t evt, event_handler* handler)
{
	handle_t handle=handler->get_handle();
	std::map<handle_t,event_handler*>::iterator it = handlers_map.find(handle);
	if(it = handlers_map.end())
	{
		handlers_map[handle] = handler;
	}
	return m_demultiplexer->request(handle,evt);
}

int reactor::remove_handler(event_handler * handler)
{
	handle_t handle = handler->get_handle();
	handlers_map.erase(handle);
	return m_demultiplexer->unrequest(handle);
}

void reactor::handle_events()
{
	int timeout  = 0;
	m_demultiplexer->wait_event(&handlers_map,timeout);

}
