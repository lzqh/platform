#ifndef __REACTOR_H
#define __REACTOR_H

class reactor{
public:
	reactor():{}
	~reactor():{}
	int register_event(int event_code, *event_handler);
	int remove_event(int event_code);
	int handle_event();
private:
	std:list<> event_list;
};

#endif
