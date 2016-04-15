#include "SimpleP2PServer.hh"
int main(int argc, char** argv){
	SimpleP2PServer* simpleserver = SimpleP2PServer::CreateNew();
	simpleserver->Start();
	return 0;
}