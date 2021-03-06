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
#define RECV_LOOP_COUNT 1
int VirtualP2PServer::recv_within_time(int fd, char *buf, size_t buf_n, struct sockaddr* addr, socklen_t *len, unsigned int sec, unsigned usec)
{
#if 1
	struct timeval tv;
	fd_set readfds;
	int i = 0;
	unsigned int n = 0;
	for (i = 0; i < RECV_LOOP_COUNT; i++)
	{
		FD_ZERO(&readfds);
		FD_SET(fd, &readfds);
		tv.tv_sec = sec;
		tv.tv_usec = usec;
		select(fd + 1, &readfds, NULL, NULL, &tv);
		if (FD_ISSET(fd, &readfds))
		{
			if ((n = recvfrom(fd, buf, buf_n, 0, addr, len)) >= 0)
			{
				return n;
			}
		}
	}
	return -1;
#else
	return recvfrom(fd, buf, buf_n, 0, addr, len);
#endif
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
	int dwSender = sizeof(sender);
	stMessage recvbuf = {0};//client向Sever发送的消息格式
	mrunning = true;
	printf("Start Loop! Port:%d\n",SERVER_PORT);
	while (mrunning){
		int ret = recv_within_time(PrimaryUDP, (char *) &recvbuf, sizeof(stMessage), (sockaddr *) &sender, (socklen_t*) &dwSender,0,100);
		if (ret <= 0){

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
			case P2PMESSAGEACK:
				ACKCallback(&sender, &recvbuf);
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
int VirtualP2PServer::ACKCallback(sockaddr_in* sender, stMessage* Message){ return 0; }