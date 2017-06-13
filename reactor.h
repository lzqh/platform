#ifndef __REACTOR_H
#define __REACTOR_H

#include <map>

namespace reactor{

enum{
	EV_READ =1,
	EV_WRITE,
};

typedef unsigned int handle_t;
typedef unsigned int event_t;

class event_handler{
public:
	event_handler():{};
	virtual ~event_handler():{};

	virtual handle_t get_handle()const =0;
	virtual void read_event();
	virtual void write_event();

};
class reactor{
public:
	reactor():{}
	virtual ~reactor():{}

	int register_handler(event_t evt, event_handler*);
	int remove_handler(event_t evt);
	void handle_events();
private:
	std:map<handle_t,event_handler *> handlers_map;
	demultiplexer *m_demultiplexer;
};

}

#endif
