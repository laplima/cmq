#include <iostream>
#include <string>
#include "CallbackAgentI.h"
#include <CMQC.h>
#include <thread>
#include <colibry/ORBManager.h>
#include <colibry/NameServer.h>
#include <colibry/InteractiveShell.h>

using namespace std;
using namespace colibry;
using namespace CMQ;

const char* SERVER_NAME = "cmq";

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

		ORBManager om{argc,argv};

		NameServer ns{om};
		gConnection = ns.resolve<Connection>(SERVER_NAME);

		string cid;
		cout << "* Enter channel id: ";
		cin >> cid;
		cout << endl;
		gChannel = gConnection->get_channel(cid.c_str());

		// set up server to receive callbacks
		om.activate_rootpoa();
		CallbackAgent_i cbi{handler};
		gCallback = om.activate_object<CallbackAgent>(cbi);
		thread mt{ [&om]() { om.run(); } };

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

		om.shutdown();
		mt.join();
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
	gChannel->basic_publish(nullptr,args[1].c_str(),m);
}

ISHELL_FUNC(Cmd::consume,args)
{
	if (args.size()<2)
		throw SyntaxError{args[0]};

	gChannel->queue_declare(args[1].c_str());
	gChannel->basic_consume(gCallback.in(),args[1].c_str());
}

ISHELL_FUNC_NOARGS(Cmd::exit)
{
}
