#include <iostream>
#include <print>
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

template<typename T>
string streamed(const T& t) { std::ostringstream ss; ss << t; return ss.str(); }

int main(int argc, char* argv[])
{
	print("CMQ Publisher\n");

	try {

		ORBManager om{argc,argv};

		span args(argv, argc);
		if (args.size() < 3) {
			println(stderr,"USAGE: {} <queue> <msg1> [<msg2> ...]", args[0]);
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

		println("\"{}\" published on queue \"{}\"", msg, qname);

	} catch (CORBA::Exception& e) {
		print(stderr, "CORBA exception: {}\n", streamed(e));
		return 1;
	}
}
