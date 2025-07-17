#include "CallbackAgentI.h"
#include <print>
#include <signal.h>
#include <mutex>

using namespace std;
using namespace CMQ;

const char* CallbackAgent_i::s_message = "";
Handler CallbackAgent_i::s_handler = nullptr;

mutex gMUTEX;

void CallbackAgent_i::lowlevel_handler(int)
{
	// gMUTEX.lock();
	// string mcopy{s_message};
	// gMUTEX.unlock();

	// s_handler(mcopy);
}

CallbackAgent_i::CallbackAgent_i (Handler h)
{
	s_handler = h;
	signal(SIGUSR1,lowlevel_handler);
}

CallbackAgent_i::~CallbackAgent_i ()
{
	signal(SIGUSR1,SIG_DFL);
}

void CallbackAgent_i::callback (CMQ::Channel_ptr ch, const Message_t& msg)
{
	// gMUTEX.lock();
	// if (!(msg >>= s_message))
	// 	cerr << "Could not extract message..." << endl;
	// s_channel = ch;
	// gMUTEX.unlock();

	// kill(getpid(), SIGUSR1);	// signal main thread (do not use raise())

	const char* s;
	if (msg >>= s)
		s_handler(ch, s);
	else
		println(stderr,"Could not extract message");
}

