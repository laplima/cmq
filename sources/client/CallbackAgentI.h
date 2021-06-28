#ifndef CALLBACKAGENTI_8BNWVC_H_
#define CALLBACKAGENTI_8BNWVC_H_

#include "CallbackAgentS.h"
#include <string>

namespace CMQ {

	using Handler = void (*)(CMQ::Channel_ptr, const std::string&);

	class  CallbackAgent_i : public virtual POA_CMQ::CallbackAgent {
	public:
		CallbackAgent_i (Handler h);
		virtual ~CallbackAgent_i ();
		virtual void callback (CMQ::Channel_ptr ch, const Message_t& msg);
	private:
		static const char* s_message;		// temporary to pass to handler
		static CMQ::Channel_ptr s_channel;	// idem
		static void lowlevel_handler(int ns);
		static Handler s_handler;
	};

};

#endif
