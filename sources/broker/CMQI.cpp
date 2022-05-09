#include "CMQI.h"
#include "CMQC.h"
#include <iostream>
#include <stdexcept>

using namespace std;
using namespace CMQ;
using namespace colibry;

// -----------------------------------------------------------------------------
// CONNECTION
// -----------------------------------------------------------------------------

Connection_i::Connection_i (ORBManager& om)
{
	// Create child poa
	poa_ = om.create_child_poa("poachannel", {
		POAPolicy::USER_ID,
		POAPolicy::NO_IMPLICIT_ACTIVATION
	});
}

::CMQ::Channel_ptr Connection_i::get_channel (const char * channel_id)
{
	// channel factory
	PortableServer::ObjectId_var oid = PortableServer::string_to_ObjectId(channel_id);

	Channel_ptr channel = Channel::_nil();

	try {
		// tries to find channel in the Active Object Map of poa_
		CORBA::Object_ptr tmp_ref = poa_->id_to_reference(oid.in());
		channel = Channel::_narrow(tmp_ref);
	} catch (PortableServer::POA::ObjectNotActive&) {
		// channel not found - create a new one
		Channel_i *c = new Channel_i(channel_id);
		poa_->activate_object_with_id(oid.in(),c);
		CORBA::Object_ptr tmp_ref = poa_->id_to_reference(oid.in());
		channel = Channel::_narrow(tmp_ref);
		cout << "   New channel \"" << channel_id << "\" created" << endl;
		c->set_auto_ref(channel);
	}
	return Channel::_duplicate(channel);
}

// ------------------------------------------------------------------
// CHANNEL
// ------------------------------------------------------------------

Channel_i::Channel_i (const std::string& id)
	: id_{id}, ref_{CMQ::Channel::_nil()}
{
	// create default exchange
	exmap_.emplace("", CMQ::DIRECT);
	cout << "    Default echange \"\" created" << endl;
}

void Channel_i::set_auto_ref(Channel_ptr ref)
{
	ref_ = CMQ::Channel::_duplicate(ref);
}

void Channel_i::queue_declare(const char *qid)
{
	if (qmap_.count(qid) == 0) {
		cout << "     New queue \"" << qid << "\" created." << endl;
		qmap_.emplace(qid,ref_.in());
		exmap_[""].bind(qid, &qmap_[qid]); // bind to default exchange
	}
	// else
	// 	cerr << "     Queue \"" << qid << "\" already declared." << endl;
}

void Channel_i::exchange_declare(const char *eid, ExchangeType t)
{
	exmap_.emplace(eid,t);
}

void Channel_i::basic_publish (const char *exchange_id,
	const char *routing_key,
	const Message_t& msg)
{
	if (exchange_exists(exchange_id)) {
		try {
			exmap_[exchange_id].publish(routing_key, msg);
		} catch (const Exchange::NoRoute& e) {
			cerr << "No route to " << routing_key << endl;
		}
	}
	else
		cerr << "ERROR: No exchange named " << exchange_id << endl;
}

void Channel_i::basic_consume (::CMQ::CallbackAgent_ptr cb, const char* queue_id)
{
	if (queue_exists(queue_id)) {
		qmap_[queue_id].add_callback(cb);
		cout << "\tNew callback registered for queue " << queue_id << endl;
	} else
		cerr << "Invalid queue: \"" << queue_id << "\"" << endl;
}

void Channel_i::bind(const char* exchange_id, const char* queue_id)
{
	if (exchange_exists(exchange_id) && queue_exists(queue_id)) {
		exmap_[exchange_id].bind(queue_id, &qmap_[queue_id]);
	}
}

bool Channel_i::queue_exists(const string& qid) const
{
	return (qmap_.count(qid) > 0);
}

bool Channel_i::exchange_exists(const std::string& exid) const
{
	return (exmap_.count(exid) > 0);
}
