#ifndef XTCP_H
#define XTCP_H

#include "XTcp.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <Windows.h>
#define socklen_t int
#define MAXBUFLEN 104857600
#define SLEEPTIMEms 10

#include <thread>

#define ending_str "[helloworld]"

using namespace std;

class XTcp {
public:
	int CreateSocket();
	bool Bind(unsigned short port);
	XTcp Accept();
	void Close();
	int Recv(char* buf, int bufsize);
	int Send(const char* buf, int sendsize);
	int SendEx(string buf);
	string RecvEx();
	bool Connect(const char *ip, unsigned short port, unsigned int timeoutms=1000);
	bool SetBlock(bool isblock);
//	XTcp();
//	virtual ~XTcp();
	
	unsigned short port = 0; // 用来建立连接的端口
	int sock = 0; // 用来通信的socket
	char ip[16];
	string data;
	bool online;
};

#endif //!XTCP_H
