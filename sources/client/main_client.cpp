//
// Client is both a publisher and a consumer
// (C) 2021 by LAPLJ. All rights reserved.
//

#include <iostream>
#include <string>
#include <CMQC.h>
#include <colibry/ORBManager.h>
#include <colibry/NameServer.h>
#include <colibry/InteractiveShell.h>
#include "CMQ.h"
#include "Callback.h"

using namespace std;
using colibry::ORBManager;
using colibry::NameServer;
using colibry::InteractiveShell;
CMQ::Channel_var channel = CMQ::Channel::_nil();
CMQ::CallbackAgent_var gCallback = CMQ::CallbackAgent::_nil();

namespace Cmd {
	DECLARE_ISHELL_FUNC(publish);
	DECLARE_ISHELL_FUNC(consume);
	DECLARE_ISHELL_FUNC(exit);
};

// called asynchronously
void handler(CMQ::Channel_ptr ch, const string &msg) {
  cout << "  MSG = \"" << msg << "\"" << endl;
}

int main(int argc, char* argv[])
{
	cout << "CMQ CLIENT" << endl;

	try {

		ORBManager orb{argc,argv};
		NameServer ns{orb};

        CMQ::Connection_var connection = blocking_connection(ns);
        channel = connection->get_channel("channel");

		// set up server to receive callbacks
		Callback cb{orb, handler};
        gCallback = CMQ::CallbackAgent::_duplicate(cb.ref());

        InteractiveShell* ish = InteractiveShell::Instance();
		ish->RegisterCmds()
			("publish", Cmd::publish,"publish <queue> <body>")
			("consume",Cmd::consume,"consume <queue>")
			("exit",Cmd::exit,"exit the program");
		ish->Help();
		cout << endl;

		string cmd;

		do {
			try {
				cmd = ish->ReadExec("> ");
			} catch (const colibry::EmptyLine&) {
			} catch (const colibry::UnknownCmd& uc) {
				cerr << "\tUnknown command: " << uc.what() << endl;
			} catch (const colibry::SyntaxError& s) {
				cerr << "\tUsage: " << ish->Doc(s.what()) << endl;
			}
		} while (cmd != "exit");

		cout << "    Terminating..." << flush;
		orb.shutdown();
		cout << "ok" << endl;
	} catch (CORBA::Exception& e) {
		cerr << "CORBA exception: " << e << endl;
		return 1;
	}
}

ISHELL_FUNC(Cmd::publish,args)
{
	if (args.size()<3)
		throw colibry::SyntaxError{args[0]};

	// build message
	string msg;
	bool first{true};
	for (int i=2; i<args.size(); ++i) {
		if (first) {
			msg += args[i];
			first = false;
		} else
		msg += " " + args[i];
	}

	channel->queue_declare(args[1].c_str());	// no effect if already created
        CMQ::Message_t m;
        m <<= msg.c_str();
	channel->basic_publish("", args[1].c_str(), m);
}

ISHELL_FUNC(Cmd::consume, args)
{
	if (args.size()<2)
		throw colibry::SyntaxError{args[0]};

	const char* queue_name = args[1].c_str();

	channel->queue_declare(queue_name);
	channel->basic_consume(gCallback.in(), queue_name);
}

ISHELL_FUNC_NOARGS(Cmd::exit)
{
}
