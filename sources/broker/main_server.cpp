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
#include <sstream>
#include <span>
#include <print>

using namespace std;
using colibry::ORBManager;
using colibry::NameServer;
using namespace CMQ;

const char* const dflt_serv_name = "cmq";

template<typename T>
string streamed(const T& t) { std::ostringstream ss; ss << t; return ss.str(); }

int main(int argc, char* argv[])
{
	print("CORBA Message Queue Broker\n");
	print("(C) 2019-25 Luiz Lima Jr.\n");

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

		println(R"(* Connnection registered in ths NS: "{}")", connection_name);

		print("* Waiting for requests\n");
		thread oth{[&om]() { om.run(); }};
		print("* Hit ENTER to quit\n");
		cin.get();

		ns.unbind(connection_name);
		om.shutdown();
		oth.join();

		print("* Terminating\n");

	} catch (CORBA::Exception& e) {
		println("CORBA execption: {}", streamed(e));
		return 1;
	}
}
