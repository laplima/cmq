#ifndef QUEUE_H
#define QUEUE_H

#include <CMQS.h>
#include <queue>
#include <list>

class Queue {
public:
	Queue() = default;
	explicit Queue(CMQ::Channel_ptr ch);
	void push(const CMQ::Message_t& m);
	void add_callback(CMQ::CallbackAgent_ptr cb);
protected:
	void flush();
protected:
	std::queue<CMQ::Message_t> mqueue;
	std::list<CMQ::CallbackAgent_var> callbacks;
	std::list<CMQ::CallbackAgent_var>::iterator next = callbacks.end();
	CMQ::Channel_ptr channel = CMQ::Channel::_nil();
};

#endif
