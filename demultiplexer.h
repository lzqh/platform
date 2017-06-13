#ifdef __DEMULTIPLEXER_H
#define __DEMULTIPLEXER_H

namespace{
class demultiplexer{
public:
	demultiplexer():{}
	virtual ~demultiplexer():{}

	int request_event(handle_t handle, event_t evt);
	int unrequest_event(handle_t handle);
	int wait_event(std:map<handle_t,event_handler *> *event_map, int timeout =0);
private:
	 int  m_epoll_fd;
	 int  m_fd_num;
};
}
#endif
