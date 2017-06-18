#ifndef __REACTOR_H
#define __REACTOR_H

#include <map>
#include <stdint.h>
#include <unistd.h>
#include <sys/epoll.h>
//#include "demultiplexer.h"


namespace reactorer{

typedef int handle_t;
typedef unsigned int event_t;

class demultiplexer;
enum{
	EV_READ =1,
	EV_WRITE,
};



class event_handler{
public:
	event_handler();
	virtual ~event_handler();

	virtual handle_t get_handle()const =0;
	virtual void read_event();
	virtual void write_event();

};
class reactor{
public:
	//reactor():{}
	//virtual ~reactor():{}

	reactor();
	virtual ~reactor();

	int register_handler(event_t evt, event_handler*);
	int remove_handler(event_handler *);
	void handle_events();
private:
	std::map<handle_t,event_handler *> handlers_map;
	demultiplexer *m_demultiplexer;
};

}

#endif
