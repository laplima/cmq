//
// (C) 2019 by Luiz Lima Jr.
//

#include <iostream>
#include "CMQI.h"
#include <colibry/ORBManager.h>
#include <colibry/NameServer.h>
#include <colibry/TextTools.h>
#include <string>
#include <thread>

using namespace std;
using namespace colibry;
using namespace CMQ;

const char* dflt_serv_name = "cmq";

int main(int argc, char* argv[])
{
	cout << "Simplified CORBA Message Queue Server" << endl;
	cout << "(C) 2019-21 by Luiz Lima Jr." << endl;

	string connection_name{dflt_serv_name};
	if (argv[1]!=nullptr)
		connection_name = argv[1];

	try {

		ORBManager om(argc,argv);
		om.activate_rootpoa();

		Connection_i cmqi{om};
		Connection_var cmq = om.activate_object<Connection>(cmqi);

		NameServer ns{om};
		ns.rebind(connection_name, cmq.in());

		cout << "* Connnection registered in ths NS: \"" << connection_name << "\"" << endl;

		cout << "* Waiting for requests" << endl;
		thread oth{[&om]() { om.run(); }};
		cout << "Type ENTER to quit" << endl;
		cin.get();

		ns.unbind(connection_name);
		om.shutdown();
		oth.join();

		cout << "Terminating" << endl;

	} catch (CORBA::Exception& e) {
		cerr << "CORBA execption: " << e << endl;
		return 1;
	}
}

