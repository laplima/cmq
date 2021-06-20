#include <iostream>
#include <string>
#include "Callback.h"
#include <CMQC.h>
#include <colibry/ORBManager.h>
#include <colibry/NameServer.h>
#include <colibry/InteractiveShell.h>
#include <memory>

using namespace std;
using namespace colibry;
using namespace CMQ;

const char* SERVER_NAME = "cmq";
Connection_ptr blocking_connection(NameServer& ns, const string& name=SERVER_NAME);

Connection_var gConnection = Connection::_nil();
Channel_var gChannel = Channel::_nil();
CallbackAgent_var gCallback = CallbackAgent::_nil();

namespace Cmd {
	DECLARE_ISHELL_FUNC(publish);
	DECLARE_ISHELL_FUNC(consume);
	DECLARE_ISHELL_FUNC(exit);
};

// called asynchronously
void handler(const string& msg)
{
	cout << "  MSG = \"" << msg << "\"" << endl;
}

int main(int argc, char* argv[])
{
	cout << "CMQ CLIENT" << endl;

	try {

		ORBManager orb{argc,argv};
		NameServer ns{orb};

		gConnection = blocking_connection(ns);
		gChannel = gConnection->get_channel("channel");

		// set up server to receive callbacks
		Callback cb{orb, handler};
		gCallback = CallbackAgent::_duplicate(cb.ref());

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
			} catch (const EmptyLine&) {
			} catch (const UnknownCmd& uc) {
				cerr << "\tUnknown command: " << uc.what() << endl;
			} catch (const SyntaxError& s) {
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
		throw SyntaxError{args[0]};

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

	gChannel->queue_declare(args[1].c_str());	// no effect if already created
	Message_t m;
	m <<= msg.c_str();
	gChannel->basic_publish("", args[1].c_str(), m);
}

ISHELL_FUNC(Cmd::consume, args)
{
	if (args.size()<2)
		throw SyntaxError{args[0]};

	const char* queue_name = args[1].c_str();

	gChannel->queue_declare(queue_name);
	gChannel->basic_consume(gCallback.in(), queue_name);
}

ISHELL_FUNC_NOARGS(Cmd::exit)
{
}

Connection_ptr blocking_connection(NameServer& ns, const string& name)
{
	return ns.resolve<Connection>(name);
}
