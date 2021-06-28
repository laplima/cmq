#ifndef CALLBACK_H
#define CALLBACK_H

#include "CallbackAgentI.h"
#include <colibry/ORBManager.h>
#include <thread>

// this is a wrapper for CallbackAgent_i class that encapsulates
// the initializing process

class Callback {
public:
	Callback(colibry::ORBManager& om, CMQ::Handler h);
	~Callback();
	CMQ::CallbackAgent_ptr ref() { return cbref.in(); }
	operator CMQ::CallbackAgent_ptr() { return ref(); }
private:
	colibry::ORBManager& orb;
	CMQ::CallbackAgent_i cbi;
	CMQ::CallbackAgent_var cbref;
	std::thread orbt;
};

#endif
