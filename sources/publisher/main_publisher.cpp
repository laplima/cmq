#include <iostream>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <string>
#include <span>
#include <CMQC.h>
#include "CMQ.h"
#include <colibry/ORBManager.h>
#include <colibry/NameServer.h>

using namespace std;
using colibry::ORBManager;
using colibry::NameServer;
using namespace CMQ;

using fmt::print;

int main(int argc, char* argv[])
{
	print("CMQ Publisher\n");

	try {

		ORBManager om{argc,argv};

		span args(argv, argc);
		if (args.size() < 3) {
			cerr << "USAGE: " << args[0] << " <queue> <msg1> [<msg2> ...]" << endl;
			return 1;
		}
		const char* qname = args[1];

		NameServer ns{om};

		Connection_var cn = blocking_connection(ns);
		Channel_var ch = cn->get_channel("channel");

		ch->queue_declare(qname);

		string msg = args[2];
		for (string m : args.last(argc-3))
			msg += " "s + m;

		Message_t m;
		m <<= msg.c_str();
		ch->basic_publish("", qname, m);

		print("\"{}\" published on queue {}\n", msg, qname);

	} catch (CORBA::Exception& e) {
		print(stderr, "CORBA exception: {}\n", fmt::streamed(e));
		return 1;
	}
}
