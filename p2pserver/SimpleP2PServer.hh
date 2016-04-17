#pragma once
#include "BasicP2PServer.hh"
class SimpleP2PServer :
	public BasicP2PServer
{
public:
	static SimpleP2PServer* CreateNew(){
		return new SimpleP2PServer();
	}
	virtual int GetPeer(unsigned int _id, sockaddr_in*, stUserListNode* stlist);
	virtual int LogoutCallback(sockaddr_in* sender, stMessage* Message);
	~SimpleP2PServer();
protected:
	SimpleP2PServer();
};

