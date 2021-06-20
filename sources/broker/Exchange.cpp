#include "Exchange.h"

void Exchange::bind(const std::string& rk, Queue* q)
{
	routemap[rk] = q;
}

void Exchange::publish(const std::string& rk, const CMQ::Message_t& m)
{
	switch (etype) {
	case CMQ::DIRECT:
		if (routemap.count(rk) > 0)
			routemap[rk]->push(m);
		else
			throw NoRoute(rk);
		break;
	case CMQ::FANOUT:
		break;
	case CMQ::TOPIC:
		break;
	case CMQ::HEADERS:
		break;
	}
}
