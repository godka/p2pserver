#include "SimpleP2PServer.hh"


SimpleP2PServer::SimpleP2PServer()
	:BasicP2PServer()
{
}


int SimpleP2PServer::GetPeer(unsigned int _id, sockaddr_in* sender, stUserListNode* stlist)
{
	unsigned int ip = sender->sin_addr.s_addr;
	unsigned int port = sender->sin_port;
	unsigned int mip = 0;
	unsigned int mport = 0;
	if (context){
		auto reply = (redisReply*) redisCommand(context, "lrange p2p:%ud 0 -1", _id);
		if (reply){
			int len = reply->elements;
			for (int i = 0; i < len; i++){
				auto peerstr = reply->element[i]->str;
				auto peerreply = (redisReply*) redisCommand(context, "get p2p:peernum:%s", peerstr);
				if (peerreply){
					auto peernum = peerreply->integer;
					if (peernum == 0){
						sscanf(peerstr, "%ud:%ud", &mip, &port);
						if (mip == ip && mport == port){
							continue;
						}
						else{
							redisCommand(context, "incr p2p:peernum:%s", peerstr);
							redisCommand(context, "set p2p:connection:%ud:%ud %s", ip, port, peerstr);
							stlist->ip = mip; stlist->port = port;
							break;
						}
					}
				}
			}
		}
	}
	return 0;
}



int SimpleP2PServer::LogoutCallback(sockaddr_in* sender, stMessage* Message)
{
	auto ip = sender->sin_addr.s_addr;
	auto port = sender->sin_port;
	printf("%ud,%ud has logout\n", ip, port);
	RemoveUser(sender);
	if (context){
		auto reply = (redisReply*)redisCommand(context, "get p2p:connection:%ud:%ud %s", ip, port);
		if (reply){
			auto peerstr = reply->str;
			if (peerstr){
				redisCommand(context, "decr p2p:peernum:%s", peerstr);
				redisCommand(context, "del p2p:connection:%ud:%ud", ip, port);
			}

		}
	}
	return 0;
}

SimpleP2PServer::~SimpleP2PServer()
{
}
