#pragma once
// ����iMessageType��ֵ
#define LOGIN 1
#define LOGOUT 2
#define P2PTRANS 3
#define GETUSER  4

// �������˿�
#define SERVER_PORT 2280

// Client��¼ʱ����������͵���Ϣ
//struct stLoginMessage
//{
	//char userName[10];
	//char password[10];
//};

// Clientע��ʱ���͵���Ϣ
//struct stLogoutMessage
//{
	//char userName[10];
//};


// �ͻ��ڵ���Ϣ
struct stUserListNode
{
	//char userName[10];
	unsigned int ip;
	unsigned short port;
};

// Client���������������һ��Client(userName)���Լ�������UDP����Ϣ
//struct stP2PTranslate
//{
	//char userName[10];
//	unsigned int ip;
//	unsigned short port;
//};

// Client����������͵���Ϣ��ʽ
struct stMessage
{
	int iMessageType;
	union _message
	{
		//stLoginMessage loginmember;
		//stLogoutMessage logoutmember;
		stUserListNode translatemessage;
	}message;
};

// Server��Client���͵���Ϣ
struct stServerToClient
{
	int iMessageType;
	union _message
	{
		stUserListNode user;
	}message;

};

//======================================
// �����Э�����ڿͻ���֮���ͨ��
//======================================
#define P2PMESSAGE				100				// ������Ϣ
#define P2PMESSAGEACK			101				// �յ���Ϣ��Ӧ��
#define P2PSOMEONEWANTTOCALLYOU 102				// ��������ͻ��˷��͵���Ϣ
// ϣ���˿ͻ��˷���һ��UDP�򶴰�
#define P2PTRASH				103				// �ͻ��˷��͵Ĵ򶴰������ն�Ӧ�ú��Դ���Ϣ
#define P2PRETURNPEER			104				// ��������ͻ��˷��͵���Ϣ

// �ͻ���֮�䷢����Ϣ��ʽ
struct stP2PMessage
{
	int iMessageType;
	int iStringLen;         // or IP address
	unsigned short Port;
};


