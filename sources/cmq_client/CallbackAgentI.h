#ifndef CALLBACKAGENTI_8BNWVC_H_
#define CALLBACKAGENTI_8BNWVC_H_

#include "CallbackAgentS.h"
#include <string>

// #if !defined (ACE_LACKS_PRAGMA_ONCE)
// #pragma once
// #endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CMQ {

	using Handler = void (*)(const std::string&);

	class  CallbackAgent_i : public virtual POA_CMQ::CallbackAgent {
	public:
		CallbackAgent_i (Handler h);
		virtual ~CallbackAgent_i ();
		virtual void callback (CMQ::Channel_ptr ch, const Message_t& msg);
	private:
		static std::string s_message;
		static void lowlevel_handler(int ns);
		static Handler s_handler;
	};

};

#endif /* CALLBACKAGENTI_H_  */
