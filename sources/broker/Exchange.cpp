#include "Exchange.h"

// #include <iostream>
// using namespace std;

void Exchange::bind(const std::string& rk, Queue* q)
{
	routemap[rk] = q;
}

void Exchange::publish(const std::string& rk, const CMQ::Message_t& m)
{
	switch (etype) {
	case CMQ::DIRECT:
		// map
		if (routemap.count(rk) > 0)
			routemap.at(rk)->push(m);
		else
			throw NoRoute(rk);
		break;
	case CMQ::FANOUT:
		// send to all queues, regardless the routing key
		for (auto& [k, q] : routemap)
			q->push(m);
		break;
	case CMQ::TOPIC:
		break;
	case CMQ::HEADERS:
		break;
	}
}
