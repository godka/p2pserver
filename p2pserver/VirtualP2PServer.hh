#pragma once
#ifdef WIN32
#	include <WinSock2.h>
#	pragma comment(lib,"ws2_32")
typedef int socklen_t;
#else
#	include <wchar.h>
#	include <unistd.h>
#	include <assert.h>
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <arpa/inet.h>
#	include <netinet/in.h>
#	include <time.h>
#	include <unistd.h>
#	include <fcntl.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <map>
#include "proto.h"
using namespace std;
typedef map<string, stUserListNode *> UserList;

class VirtualP2PServer
{
public:
	static VirtualP2PServer* CreateNew(){
		return new VirtualP2PServer();
	}
	VirtualP2PServer();
	~VirtualP2PServer();
	int Start();
	int Stop();
	virtual int LoginCallback(sockaddr_in* sender,stMessage* Message);
	virtual int LogoutCallback(sockaddr_in* sender, stMessage* Message);
	virtual int P2PTransCallback(sockaddr_in* sender, stMessage* Message);
	virtual int GetUserCallback(sockaddr_in* sender, stMessage* Message);
private:
	int InitSock();
	//stUserListNode* GetUser(char *username);
protected:
#ifdef WIN32
	SOCKET PrimaryUDP;
#else
    int PrimaryUDP;
#endif
	UserList ClientList;
	bool mrunning;
};
