#include <iostream>
#include <string>
#include <CMQC.h>
#include "CMQ.h"
#include <colibry/ORBManager.h>
#include <colibry/NameServer.h>

using namespace std;
using colibry::ORBManager;
using colibry::NameServer;
using namespace CMQ;

int main(int argc, char* argv[])
{
	cout << "CMQ PUBLISHER" << endl;

	if (argc < 3) {
		cerr << "USAGE: " << argv[0] << " <queue> <msg1> [<msg2> ...]" << endl;
		return 1;
	}

	const char* qname = argv[1];

	try {

		ORBManager om{argc,argv};
		NameServer ns{om};

		Connection_var cn = blocking_connection(ns);
		Channel_var ch = cn->get_channel("channel");

		ch->queue_declare(qname);

		string msg = argv[2];
		for (int i=3; i<argc; ++i)
			msg += string{" "} + argv[i];

		Message_t m;
		m <<= msg.c_str();
		ch->basic_publish("", qname, m);

	} catch (CORBA::Exception& e) {
		cerr << "CORBA exception: " << e << endl;
		return 1;
	}
}
