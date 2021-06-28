#include "CMQ.h"

CMQ::Connection_ptr blocking_connection(colibry::NameServer& ns,
	const std::string& broker_name)
{
	return ns.resolve<CMQ::Connection>(broker_name);
}
