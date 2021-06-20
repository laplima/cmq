#include "Callback.h"
#include <iostream>

using namespace std;

Callback::Callback(colibry::ORBManager& om, CMQ::Handler h) : orb{om}, cbi{h}
{
	orb.activate_rootpoa();
	cbref = orb.activate_object<CMQ::CallbackAgent>(cbi);
	orbt = thread{ [this]() { this->orb.run(); } };
}

Callback::~Callback()
{
	cout << "    Wating for orb thread..." << flush;
	orbt.join();
	cout << "ok" << endl;
}
