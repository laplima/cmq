#include <iostream>
#include <print>
#include <string>
#include <span>
#include "CMQ.h"
#include "Callback.h"
#include <colibry/ORBManager.h>
#include <colibry/NameServer.h>

using namespace std;
using namespace colibry;
using namespace CMQ;

template<typename T>
string streamed(const T& t) { std::ostringstream ss; ss << t; return ss.str(); }

void handler(CMQ::Channel_ptr ch, const string& msg)
{
	println("  MSG = \"{}\"", msg);
}

int main(int argc, char* argv[])
{
	println("CMQ Consumer");

	try {

		ORBManager om{argc,argv};

		span args(argv, argc);
		if (args.size() < 2) {
			println(stderr,"USAGE: {} <queue>", args[1]);
			return 1;
		}
		const char* qname = args[1];

		NameServer ns{om};

		Connection_var connection = blocking_connection(ns);
		Channel_var channel = connection->get_channel("channel");

		channel->queue_declare(qname);
		Callback cb{om, handler};
		channel->basic_consume(cb, qname);

		println("* Type ENTER to quit");
		cin.get();

		om.shutdown();
		println("* Terminating");
	} catch (CORBA::Exception& e) {
		println(stderr,"CORBA exception: {}", streamed(e));
		return 1;
	}
}
