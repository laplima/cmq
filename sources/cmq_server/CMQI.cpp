#include "CMQI.h"
#include <iostream>
#include <stdexcept>
#include <colibry/ORBManager.h>

using namespace std;
using namespace CMQ;

Connection_i::Connection_i (const PortableServer::POA_ptr poa)
	: m_poa{PortableServer::POA::_duplicate(poa)}
{
}

Connection_i::~Connection_i ()
{
}

::CMQ::Channel_ptr Connection_i::get_channel (const char * channel_id)
{
	// factory
	PortableServer::ObjectId_var oid = PortableServer::string_to_ObjectId(channel_id);

	Channel_ptr channel;

	try {
		// tries to find channel in the Active Object Map of m_poa
		CORBA::Object_ptr tmp_ref = m_poa->id_to_reference(oid.in());
		channel = Channel::_narrow(tmp_ref);
		// cout << "Channel found" << endl;
	} catch (PortableServer::POA::ObjectNotActive&) {
		// channel not found - create a new one
		Channel_i *c = new Channel_i(channel_id);
		m_poa->activate_object_with_id(oid.in(),c);
		CORBA::Object_ptr tmp_ref = m_poa->id_to_reference(oid.in());
		channel = Channel::_narrow(tmp_ref);
		cout << "   New channel \"" << channel_id << "\" created" << endl;
		c->set_auto_ref(channel);
	}

	return Channel::_duplicate(channel);
}

void Connection_i::close()
{
	colibry::ORBManager::global->shutdown();
}

// ------------------------------------------------------------------
// CHANNEL
// ------------------------------------------------------------------

Channel_i::Channel_i (const std::string& id)
	: m_id{id}, m_myref{CMQ::Channel::_nil()}
{
}

Channel_i::~Channel_i ()
{
}

void Channel_i::set_auto_ref(Channel_ptr ref)
{
	m_myref = CMQ::Channel::_duplicate(ref);
}

void Channel_i::queue_declare(const char *qid)
{
	if (m_mqmap.count(qid) == 0) {
		cout << "     New queue \"" << qid << "\" created." << endl;
		m_mqmap[qid].count++;
	} else {
		cerr << "     Queue \"" << qid << "\" already declared." << endl;
	}
}

void Channel_i::exchange_declare(const char *eid, ExchangeType t)
{
	// to do
}

void Channel_i::basic_publish (const char *exchange_id, const char *routing_key, const Message_t& msg)
{
	// to do: use exchange_id
	try {
		check_queue(routing_key);
		m_mqmap[routing_key].q.push(msg);
		flush(routing_key);
	} catch (runtime_error& e) {
		cerr << "Invalid routing key: \"" << routing_key << "\"" << endl;
	}
}

void Channel_i::basic_consume (::CMQ::CallbackAgent_ptr cb, const char* queue_id)
{
	try {
		check_queue(queue_id);
		m_mqmap[queue_id].callbacks.push(CallbackAgent::_duplicate(cb));
		flush(queue_id);
	} catch (runtime_error& e) {
		cerr << "Invalid queue: \"" << queue_id << "\"" << endl;
	}
}

void Channel_i::check_queue(const string& qid) const
{
	if (m_mqmap.count(qid) == 0)
		throw runtime_error{"invalid queue"};
}


void Channel_i::flush(const char* qid)
{
	try {
		check_queue(qid);
		auto& q = m_mqmap[qid].q;
		auto& cb = m_mqmap[qid].callbacks;
		while (!q.empty() && !cb.empty()) {
			try {
				cb.front()->callback(m_myref.in(), q.front());
				q.pop();	// message delivered
				// callback round-robin
				cb.push(cb.front());
				cb.pop();
			} catch(CORBA::Exception&) {
				cb.pop();
			}
		}
	} catch (runtime_error& e) {
		cerr << "Invalid queue: \"" << qid << "\"" << endl;
	}
}
