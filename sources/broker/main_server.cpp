//
// Simplified CORBA Message Queue Broker based on AMQP (RabbitMQ)
// (C) 2019-21 by Luiz Lima Jr.
//

#include <iostream>
#include "CMQI.h"
#include <colibry/ORBManager.h>
#include <colibry/NameServer.h>
#include <colibry/TextTools.h>
#include <string>
#include <thread>
#include <span>
#include <fmt/core.h>
#include <fmt/ostream.h>

using namespace std;
using colibry::ORBManager;
using colibry::NameServer;
using namespace CMQ;

using fmt::print;

const char* const dflt_serv_name = "cmq";

int main(int argc, char* argv[])
{
	print("CORBA Message Queue Broker\n");
	print("(C) 2019-23 Luiz Lima Jr.\n");

	try {

		ORBManager om{argc,argv};

		// parse command-line
		span args(argv, argc);
		string connection_name{dflt_serv_name};
		if (args.size() > 1)
			connection_name = args[1];

		om.activate_rootpoa();

		Connection_i cmqi{om};
		Connection_var cmq = om.activate_object<Connection>(cmqi);

		NameServer ns{om};
		ns.rebind(connection_name, cmq.in());

		print(R"(* Connnection registered in ths NS: "{}"{})",
			connection_name, "\n");

		print("* Waiting for requests\n");
		thread oth{[&om]() { om.run(); }};
		print("* Hit ENTER to quit\n");
		cin.get();

		ns.unbind(connection_name);
		om.shutdown();
		oth.join();

		print("* Terminating\n");

	} catch (CORBA::Exception& e) {
		print(stderr, "CORBA execption: {}\n", fmt::streamed(e));
		return 1;
	}
}
