#include "reactor.h"
#include "demultiplexer.h"
#include <iostream>
namespace reactorer{
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
	std::cout <<"in register_handler"<<std::endl;
	if(it == handlers_map.end())
	{
		handlers_map[handle] = handler;
	}
	return m_demultiplexer->request_event(handle,evt);
}

int reactor::remove_handler(event_handler * handler)
{
	handle_t handle = handler->get_handle();
	handlers_map.erase(handle);
	return m_demultiplexer->unrequest_event(handle);
}

void reactor::handle_events()
{
	std::cout <<"in handle_events"<<std::endl;
	int timeout  = 0;
	m_demultiplexer->wait_event(&handlers_map,timeout);

}

event_handler::event_handler()
{
}
event_handler::~event_handler()
{
}
void event_handler::read_event()
{
}

void event_handler::write_event()
{
}
}
