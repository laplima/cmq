#ifndef CMQI_8NKMGW_H_
#define CMQI_8NKMGW_H_

#include <CMQS.h>
#include <string>
#include <map>
#include <colibry/ORBManager.h>
#include "Queue.h"
#include "Exchange.h"

namespace CMQ {

	// Connection: Channel Factory

	class  Connection_i : public virtual POA_CMQ::Connection {
	public:
		explicit Connection_i (colibry::ORBManager& om);
		::CMQ::Channel_ptr get_channel (const char * id) override;
	private:
		colibry::MPOA poa_;
	};

	// Channel

	class  Channel_i : public virtual POA_CMQ::Channel {
	public:
		explicit Channel_i (std::string id);

		// IDL
		char * id () override { return CORBA::string_dup(id_.c_str()); }
		void queue_declare(const char *qid) override;
		void exchange_declare(const char *exchange_id, ExchangeType t) override;
		void basic_publish (const char *exchange_id, const char *routing_key,
			const Message_t& body) override;
		void basic_consume (::CMQ::CallbackAgent_ptr cb,
			const char *routing_key) override;
		void bind(const char* exchange_id, const char* queue_id) override;

		virtual void set_auto_ref(Channel_ptr ref);
	protected:
		[[nodiscard]] bool queue_exists(const std::string& qid) const;
		[[nodiscard]] bool exchange_exists(const std::string& exid) const;
		std::string id_;
		std::map<std::string,Queue> qmap_;		// queues: queue_name => Queue
		std::map<std::string,Exchange> exmap_;	// exchanges: exchange_name => Exchange
		Channel_var ref_;
	};

};

#endif
