#include "VirtualP2PServer.hh"

VirtualP2PServer::VirtualP2PServer()
{
	InitSock();
	mrunning = false;
}


VirtualP2PServer::~VirtualP2PServer()
{
}

int VirtualP2PServer::InitSock()
{
#ifdef WIN32
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
		printf("Windows sockets 2.2 startup");
		return 1;
	}
	else{
		printf("init winsock32 success\n");
	}
#endif
	return 0;
}

int VirtualP2PServer::Stop(){
	mrunning = false;
	return 0;
}
int VirtualP2PServer::Start()
{
	PrimaryUDP = socket(AF_INET, SOCK_DGRAM, 0);
	if (PrimaryUDP < 0){
		printf("create socket error");
		return 1;
	}
	sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(SERVER_PORT);
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	int nResult = bind(PrimaryUDP, (sockaddr*) &local, sizeof(sockaddr));

	sockaddr_in sender;
	stMessage recvbuf = {0};//client向Sever发送的消息格式
	mrunning = true;
	printf("Start Loop! Port:%d\n",SERVER_PORT);
	while (mrunning){
		int dwSender = sizeof(sender);
		int ret = recvfrom(PrimaryUDP, (char *) &recvbuf, sizeof(stMessage), 0, (sockaddr *) &sender, (socklen_t*)&dwSender);
		if (ret <= 0){
			printf("recv error");
			continue;
		}
		else{
			switch (recvbuf.iMessageType){
			case LOGIN:
				LoginCallback(&sender,&recvbuf);
				break;
			case LOGOUT:
				LogoutCallback(&sender,&recvbuf);
				break;
			case P2PTRANS:
				P2PTransCallback(&sender,&recvbuf);
				break;
			case GETUSER:
				GetUserCallback(&sender, &recvbuf);
				break;
			}
		}
	}

	return 0;
}

int VirtualP2PServer::LoginCallback(sockaddr_in* sender, stMessage* Message){ return 0; }
int VirtualP2PServer::LogoutCallback(sockaddr_in* sender, stMessage* Message){ return 0; }
int VirtualP2PServer::P2PTransCallback(sockaddr_in* sender, stMessage* Message){ return 0; }
int VirtualP2PServer::GetUserCallback(sockaddr_in* sender, stMessage* Message){ return 0; }
