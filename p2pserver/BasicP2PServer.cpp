#include "BasicP2PServer.hh"


BasicP2PServer::BasicP2PServer()
	:VirtualP2PServer(){
	InitRedis();
}


int BasicP2PServer::RemoveUser(sockaddr_in* sock){
	auto ip = sock->sin_addr.s_addr;
	auto port = sock->sin_port;
	auto reply = (redisReply*)redisCommand(context, "get p2p:id:%d:%d", ip, port);
	if (reply){
		auto id = reply->integer;
		redisCommand(context, "lrem p2p:%d 0 %d:%d", id, ip, port);
		redisCommand(context, "lrem p2p:list 0 %d:%d", ip, port);
		redisCommand(context, "del p2p:id:%d:%d", ip, port);
	}
	return 0;
}

int BasicP2PServer::AddUser(unsigned int id,sockaddr_in* sock){
	auto ip = sock->sin_addr.s_addr;
	auto port = sock->sin_port;
	if (context){
		redisCommand(context, "set p2p:id:%d:%d %d", ip, port, id);
		redisCommand(context, "lpushx p2p:%d %d:%d", id, ip, port);
	}
	return 0;
}


int BasicP2PServer::SendToPeer(sockaddr_in* sender, stUserListNode* node)
{
	sockaddr_in remote;//目的客户端地址信息
	remote.sin_family = AF_INET;
	remote.sin_port = htons(node->port);
	remote.sin_addr.s_addr = htonl(node->ip);
#ifdef _DEBUG
	in_addr tmp = { 0 };
	tmp.s_addr = htonl(node->ip);
	printf("the address is %s,and port is %d\n", inet_ntoa(tmp), node->port);//网络地址转换为字符串地址
#endif
	stP2PMessage transMessage;
	transMessage.iMessageType = P2PSOMEONEWANTTOCALLYOU;
	transMessage.iStringLen = ntohl(sender->sin_addr.s_addr);
	transMessage.Port = ntohs(sender->sin_port);

	sendto(PrimaryUDP, (const char*) &transMessage, sizeof(transMessage), 0, (const sockaddr *) &remote, sizeof(sockaddr_in));
	return 0;
}


int BasicP2PServer::InitRedis()
{
	timeval tv = { 0 };
	tv.tv_sec = 1; tv.tv_usec = 0;
	context = redisConnectWithTimeout("127.0.0.1", 6379, tv);
	if (context->err) {
		printf("SET:Redis:Connection error: %s\n", context->errstr);
		return 1;
	}
	return 0;
}

int BasicP2PServer::LoginCallback(sockaddr_in* sender, stMessage* Message){
	auto ip = sender->sin_addr.s_addr;
	auto port = sender->sin_port;
	printf("%d,%d has login\n", ip, port);
	if (context){
		redisCommand(context, "lpushx p2p:list %d:%d", ip, port);
	}
	return 0;
}

int BasicP2PServer::LogoutCallback(sockaddr_in* sender, stMessage* Message){
	RemoveUser(sender);
	return 0;
}

int BasicP2PServer::P2PTransCallback(sockaddr_in* sender, stMessage* Message){
	stUserListNode node = Message->message.translatemessage;
	return SendToPeer(sender,&node);
}

int BasicP2PServer::GetUserCallback(sockaddr_in* sender, stMessage* Message){
	stUserListNode mlist = { 0 };
	auto _id = Message->message.id;
	AddUser(_id, sender);
	if (GetPeer(_id, sender, &mlist) == 0){
		SendPeerRequest(sender, &mlist);
	}
	
	return 0;
}
int BasicP2PServer::SendPeerRequest(sockaddr_in* sender, stUserListNode* peerstr)
{
	stP2PMessage transMessage;
	transMessage.iMessageType = P2PRETURNPEER;
	transMessage.iStringLen = peerstr->ip;
	transMessage.Port = peerstr->port;
	sendto(PrimaryUDP, (const char*) &transMessage, sizeof(transMessage), 0, (const sockaddr *) &sender, sizeof(sockaddr_in));
	return 0;
}
int BasicP2PServer::GetPeer(unsigned int _id,sockaddr_in* sender,stUserListNode* stlist)
{
	return 0;
}

BasicP2PServer::~BasicP2PServer()
{
}
