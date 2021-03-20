#ifndef CMQI_8NKMGW_H_
#define CMQI_8NKMGW_H_

#include <CMQS.h>
#include <string>
#include <queue>
#include <map>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif

namespace CMQ {

	// Channel Factory
	class  Connection_i : public virtual POA_CMQ::Connection {
	public:
		Connection_i (const PortableServer::POA_ptr poa);
		virtual ~Connection_i ();
		virtual ::CMQ::Channel_ptr get_channel (const char * id);
		virtual void close ();
	private:
		PortableServer::POA_var m_poa;
	};

	class  Channel_i : public virtual POA_CMQ::Channel {
	public:
		Channel_i (const std::string& id);
		virtual ~Channel_i ();

		virtual	char * id (void) { return CORBA::string_dup(m_id.c_str()); }
		virtual void queue_declare(const char *routing_key);
		virtual void exchange_declare(const char *exchange_id, ExchangeType t);
		virtual void basic_publish (const char *exchange_id, const char *routing_key, const Message_t& body);
		virtual void basic_consume (::CMQ::CallbackAgent_ptr cb, const char *routing_key);
	public:
		virtual void set_auto_ref(Channel_ptr ref);
	protected:
		virtual void check_queue(const std::string& qid) const;
		virtual void flush(const char* qid);
	protected:
		std::string m_id;
		struct QueueData {
			QueueData() : count{0} {}
			unsigned short count;
			std::queue<::CMQ::CallbackAgent_var> callbacks;
			std::queue<CMQ::Message_t> q;
		};
		std::map<std::string,QueueData> m_mqmap;		// routing key => QueueData
		Channel_var m_myref;
	};

};

#endif /* CMQI_H_  */
