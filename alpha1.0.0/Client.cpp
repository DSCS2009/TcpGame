#include "Client.h"

using namespace std;

void Game::Client::init(SOCKET serverSock, int idx) {
	this->id = idx;
	this->online = true;
	this->sersok = serverSock;
	//https://blog.csdn.net/u013093948/article/details/53584461/
	thread t_update(Game::Client::UpdateDataThread, this);
	t_update.detach();
}

void Game::Client::send_(string str) {
	send(cltsok, str.c_str(), (int)str.length(), 0);
}

string Game::Client::recv_next_bag() {
	for(int times = 1; times <= 1000; times++) {
		if(times == 1000) {
			this->online = false;
			Sleep(50); //waiting update data thread
			return "[Offline]";
		}
		if(!this->bag_db.empty()) {
			break;
		}
		
		if(times % 50 == 0) printf("waited client No.%d %d s\n", this->id, times / 10);
		Sleep(100);
	}
	string str = bag_db.front();
	bag_db.pop_front();
	return str;
}

void Game::Client::UpdateDataThread() {
	while(this->online) {
		char buf[1024];
		string buf_str;
		recv(this->sersok, buf, 1024, 0);
		if(this->sersok == NULL) {
			printf("get server socket (Game::Client::UpdatDataThread) [FAILED]\n");
			exit(-1);
		}
		buf_str = buf;
		this->data += buf;
		if(buf_str.length() == 0) {
			printf("Client No.%d get an empty bag\n", this->id);
			continue;
		}
		printf("debug: buf_str->%s\n", buf_str.c_str());
		printf("debug: now data->%s\n", this->data.c_str());
		//find ending_str
		while(true) {
			// !!!34hello world.find(hello)
			// p =  ^
			int p = this->data.find(ending_str);
			if(p == string::npos) break;
			string nextbag_str = this->data.substr(0, p);
			this->data = this->data.substr(p + strlen(ending_str));
			this->bag_db.push_back(nextbag_str);
			printf("debug: new bag from client No.%d->%s\n", this->id, nextbag_str.c_str());
		}
		Sleep(100);
	}
	printf("Client No.%d is offline now.\n", this->id);
	this->online = false;
}
