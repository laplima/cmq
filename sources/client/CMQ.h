#ifndef CMQ_H
#define CMQ_H

#include <string>
#include <colibry/NameServer.h>
#include <CMQC.h>

inline const char* BROKER_NAME = "cmq";

CMQ::Connection_ptr blocking_connection(colibry::NameServer& ns,
	const std::string& name=BROKER_NAME);

#endif
