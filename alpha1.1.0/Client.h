#ifndef CLIENT_H
#define CLIENT_H 1919810

#include <cstdio>
#include <string>
#include <thread>
#include <deque>
#include <winsock2.h>
#include <Windows.h>



using namespace std;

namespace Game {
	class Client{
	public:
		SOCKET sersok, cltsok;
		string uname;
		SOCKADDR cltad;
		string data;
		bool online = false;
		int id;
		deque<string> bag_db;
		
		void init(SOCKET sersok, int idx);
		void send_(string str);
		void UpdateDataThread();
		string recv_next_bag();
	};
}

#endif //!CLIENT_H
