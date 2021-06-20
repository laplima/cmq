#ifndef EXCHANGE_H
#define EXCHANGE_H

#include "Queue.h"
#include <string>
#include <stdexcept>
#include <map>

class Exchange {
public:
	// Exchange() : etype{CMQ::DIRECT} {}
	Exchange(CMQ::ExchangeType t=CMQ::DIRECT) : etype{t} {}
	void bind(const std::string& rk, Queue* q);
	void publish(const std::string& rk, const CMQ::Message_t& m);
public:
	class NoRoute : public std::runtime_error {
	public:
		NoRoute(const std::string& w) : std::runtime_error{w} {}
	};
protected:
	CMQ::ExchangeType etype;
	std::map<std::string,Queue*> routemap;
};

#endif
