#pragma once
#include "VirtualP2PServer.hh"
#include "hiredis.h"
class BasicP2PServer
	:public VirtualP2PServer
{
public:
	static BasicP2PServer* CreateNew(){
		return new BasicP2PServer();
	}
	~BasicP2PServer();
	virtual int LoginCallback(sockaddr_in* sender, stMessage* Message);
	virtual int LogoutCallback(sockaddr_in* sender, stMessage* Message);
	virtual int P2PTransCallback(sockaddr_in* sender, stMessage* Message);
	virtual int GetUserCallback(sockaddr_in* sender, stMessage* Message);
	virtual int GetPeer(unsigned int _id, sockaddr_in* sender, stUserListNode* stlist);
	virtual int SendPeerRequest(sockaddr_in* sender, stUserListNode* peerstr);
protected:
	BasicP2PServer();
	redisContext* context;
	int RemoveUser(sockaddr_in* sock);
	int AddUser(unsigned int id, sockaddr_in* sock);
	int SendToPeer(sockaddr_in* sender, stUserListNode* node);
private:
	//int AddGlobalUser(unsigned int ip,unsigned short port);
	int InitRedis();
};

