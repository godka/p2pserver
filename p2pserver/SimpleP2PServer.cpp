#include "SimpleP2PServer.hh"


SimpleP2PServer::SimpleP2PServer()
	:BasicP2PServer()
{
}


int SimpleP2PServer::GetPeer(unsigned int _id, sockaddr_in* sender, stUserListNode* stlist)
{
	auto ip = sender->sin_addr.s_addr;
	auto port = sender->sin_port;
	if (context){
		auto reply = (redisReply*) redisCommand(context, "lrange p2p:%d 0 -1", _id);
		if (reply){
			int len = reply->elements;
			for (int i = 0; i < len; i++){
				auto peerstr = reply->element[i]->str;
				auto peerreply = (redisReply*)redisCommand(context, "get p2p:peernum:%s", peerstr);
				if (peerreply){
					auto peernum = peerreply->integer;
					if (peernum == 0){
						redisCommand(context, "incr p2p:peernum:%s", peerstr);
						redisCommand(context, "set p2p:connection:%d:%d %s", ip,port,peerstr);
						sscanf(peerstr,"%d:%d", &stlist->ip, &stlist->port);
						
						return 0;
					}
				}
			}
		}
	}
	return -1;
}



int SimpleP2PServer::LogoutCallback(sockaddr_in* sender, stMessage* Message)
{
	auto ip = sender->sin_addr.s_addr;
	auto port = sender->sin_port;
	RemoveUser(sender);
	if (context){
		auto reply = (redisReply*)redisCommand(context, "get p2p:connection:%d:%d %s", ip, port);
		if (reply){
			auto peerstr = reply->str;
			if (peerstr){
				redisCommand(context, "decr p2p:peernum:%s", peerstr);
				redisCommand(context, "del p2p:connection:%d:%d", ip, port);
			}

		}
	}
	return 0;
}

SimpleP2PServer::~SimpleP2PServer()
{
}