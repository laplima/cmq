#include "Queue.h"
#include <iostream>

using namespace std;

Queue::Queue(CMQ::Channel_ptr ch) : channel{ch}
{
}

void Queue::push(const CMQ::Message_t& m)
{
	mqueue.push(m);
	flush();
}

void Queue::add_callback(CMQ::CallbackAgent_ptr cb)
{
	callbacks.push_back(CMQ::CallbackAgent::_duplicate(cb));
	if (next == callbacks.end())
		next = callbacks.begin();
	flush();
}

void Queue::flush()
{
	if (callbacks.empty())
		return;

	while (!mqueue.empty()) {
		try {			// cout << "Queue::flush() - calling back" << std::flush;
			(*next)->callback(channel, mqueue.front());
			mqueue.pop();
			++next;
			if (next == callbacks.end())
				next = callbacks.begin();
		} catch (const CORBA::Exception&) {
			// callback unavailable - remove it
			auto rem = next;
			++next;
			if (next == callbacks.end())
				next = callbacks.begin();
			callbacks.erase(rem);
		}
	}
}
