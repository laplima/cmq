#include <iostream>
#include <string>
#include "CMQ.h"
#include "Callback.h"
#include <colibry/ORBManager.h>
#include <colibry/NameServer.h>

using namespace std;
using namespace colibry;
using namespace CMQ;

void handler(CMQ::Channel_ptr ch, const string& msg)
{
	cout << "  MSG = \"" << msg << "\"" << endl;
}

int main(int argc, char* argv[])
{
	cout << "CMQ CONSUMER" << endl;

	if (argc < 2) {
		cerr << "USAGE: " << argv[0] << " <queue>" << endl;
		return 1;
	}

	const char* qname = argv[1];

	try {

		ORBManager om{argc,argv};
		NameServer ns{om};

		Connection_var connection = blocking_connection(ns);
		Channel_var channel = connection->get_channel("channel");

		channel->queue_declare(qname);
		Callback cb{om, handler};
		channel->basic_consume(cb, qname);

		cout << "* Type ENTER to quit" << endl;
		cin.get();

		om.shutdown();
		cout << "* Terminating" << endl;
	} catch (CORBA::Exception& e) {
		cerr << "CORBA exception: " << e << endl;
		return 1;
	}
}
