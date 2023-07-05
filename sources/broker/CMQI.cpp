#include "CMQI.h"
#include "CMQC.h"
#include "tao/Exception.h"
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <stdexcept>

using namespace std;
using namespace CMQ;
using namespace colibry;

using fmt::print;
using fmt::streamed;

// -----------------------------------------------------------------------------
// CONNECTION
// -----------------------------------------------------------------------------

Connection_i::Connection_i (ORBManager& om)
{
	// Create child poa
	auto rpoa = om.rootpoa();
	poa_ = rpoa.create_child_poa("poachannel", {
		POAPolicy::USER_ID,
		POAPolicy::NO_IMPLICIT_ACTIVATION
	});
}

::CMQ::Channel_ptr Connection_i::get_channel (const char * channel_id)
{
	// channel factory

	Channel_ptr channel = poa_.get_reference<Channel>(channel_id);
	if (CORBA::is_nil(channel)) {
		// create a new channel
		auto *c = new Channel_i(channel_id);
		channel = poa_.activate_object_with_id<Channel>(channel_id,*c);
		print("    New channel \"{}\" created\n", channel_id);
		c->set_auto_ref(channel);
	}

	return Channel::_duplicate(channel);
}

// ------------------------------------------------------------------
// CHANNEL
// ------------------------------------------------------------------

Channel_i::Channel_i (std::string id)
	: id_{std::move(id)}, ref_{CMQ::Channel::_nil()}
{
	// create default exchange
	exmap_.emplace("", CMQ::DIRECT);
	print("    Default exchange \"\" created\n");
}

void Channel_i::set_auto_ref(Channel_ptr ref)
{
	ref_ = CMQ::Channel::_duplicate(ref);
}

void Channel_i::queue_declare(const char *qid)
{
	if (qmap_.count(qid) == 0) {
		print("    New queue \"{}\" created.\n", qid);
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
			exmap_.at(exchange_id).publish(routing_key, msg);
		} catch (const Exchange::NoRoute& e) {
			print(stderr, "No route to {}\n", routing_key);
		} catch (const CORBA::Exception& e) {
			print(stderr, "CORBA connection to {} is lost\n", routing_key);
		}
	}
	else
		print(stderr, "ERROR: No exchange called \"{}\"\n", exchange_id);
}

void Channel_i::basic_consume (::CMQ::CallbackAgent_ptr cb, const char* queue_id)
{
	if (queue_exists(queue_id)) {
		qmap_[queue_id].add_callback(cb);
		print("    New callback registered for queue \"{}\"\n", queue_id);
	} else
		print(stderr, "Invalid queue: \"{}\"\n", queue_id);
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
