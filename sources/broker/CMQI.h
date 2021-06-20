#ifndef CMQI_8NKMGW_H_
#define CMQI_8NKMGW_H_

#include <CMQS.h>
#include <string>
#include <map>
#include <colibry/ORBManager.h>
#include "Queue.h"
#include "Exchange.h"

namespace CMQ {

	// Channel Factory
	class  Connection_i : public virtual POA_CMQ::Connection {
	public:
		Connection_i (colibry::ORBManager& om);
		::CMQ::Channel_ptr get_channel (const char * id) override;
	private:
		PortableServer::POA_var poa_;
	};

	class  Channel_i : public virtual POA_CMQ::Channel {
	public:
		Channel_i (const std::string& id);

		char * id (void) override { return CORBA::string_dup(id_.c_str()); }
		void queue_declare(const char *routing_key) override;
		void exchange_declare(const char *exchange_id, ExchangeType t) override;
		void basic_publish (const char *exchange_id, const char *routing_key, const Message_t& body) override;
		void basic_consume (::CMQ::CallbackAgent_ptr cb, const char *routing_key) override;
		void bind(const char* exchange_id, const char* queue_id) override;
	public:
		virtual void set_auto_ref(Channel_ptr ref);
	protected:
		bool queue_exists(const std::string& qid) const;
		bool exchange_exists(const std::string& exid) const;
		// virtual void flush(const char* qid);
	protected:
		std::string id_;
		std::map<std::string,Queue> qmap_;		// queue_name => Queue
		std::map<std::string,Exchange> exmap_;	// exchange_name => Exchange
		Channel_var ref_;
	};

};

#endif /* CMQI_H_  */
