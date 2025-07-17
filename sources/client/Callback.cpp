#include "Callback.h"
#include <print>
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
	print("    Wating for orb thread...");
	cout << flush;
	orbt.join();
	println("ok");
}
