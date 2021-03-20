//
// (C) 2019 by Luiz Lima Jr.
//

#include <iostream>
#include "CMQI.h"
#include <colibry/ORBManager.h>
#include <colibry/NameServer.h>
#include <colibry/TextTools.h>
#include <signal.h>
#include <string>

using namespace std;
using namespace colibry;
using namespace CMQ;

const char* DEFAULT_SERVER_NAME = "cmq";
void termination_handler(int ns);

int main(int argc, char* argv[])
{
	cout << "Simplified CORBA Message Queue Server" << endl;
	cout << "(C) 2019 by Luiz Lima Jr." << endl;

	string connection_name{DEFAULT_SERVER_NAME};
	if (argv[1]!=nullptr)
		connection_name = argv[1];

	try {

		ORBManager om(argc,argv);
		om.activate_rootpoa();

		// Create child poa
		PortableServer::POA_var childpoa = om.create_child_poa("poachannel",
			{ ORBManager::Policy::USER_ID, ORBManager::Policy::NO_IMPLICIT_ACTIVATION });

		Connection_i cmqi{childpoa};
		Connection_var cmq = om.activate_object<Connection>(cmqi);

		NameServer* ns = NameServer::Instance(om.orb());
		ns->rebind(connection_name,cmq.in());

		cout << "* Connnection registered in ths NS: \"" << connection_name << "\"" << endl;

		signal(SIGINT,termination_handler);
		signal(SIGTERM,termination_handler);

		cout << "* Waiting for requests" << endl;
		om.run();

		cout << "Terminating" << endl;

	} catch (CORBA::Exception& e) {
		cerr << "CORBA execption: " << e << endl;
		return 1;
	}
}

void termination_handler(int ns)
{
	ORBManager::global->shutdown();
	cout << cursor_back();
}
