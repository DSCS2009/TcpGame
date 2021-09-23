#define MY_DSCS2009 "DSCS2009's"
#define CODE_DSCS2009 " excrement mountain code(very slow)" 
#define GAME_VERSION "alpha 1.0.0 1/4 server and no client awa"

#include <cstdio>
#include <iostream>
#include <thread> 
#include <map>
#include <fstream>
#include <winsock2.h>
#include <Windows.h>
#include "XTcp.h"
#include "Game.h"

using namespace std;
using namespace Game;

string exeplace;
SOCKET serverSock;
map<string, XTcp*> online_clt;
map<string, Player*> players;
const int MAPSIZE = 1000;
Block block_map[1001][1001];

void AcceptThread();
thread tAccept(AcceptThread);

void quit();

void InitBlockMap() {
	ifstream inFile("data.txt");
	if(!inFile.good()) {
	__PRINT:
		printf("I found that you don't have a data.txt, do you want me to help you to do that?(Y/N)");
		string str;
		cin >> str;
		if(str == "YES" || str == "Yes" || str == "yes" || str == "Y" || str == "y") {
			printf("what is the pre of your player's name, if you wanted names created by yourself, please put a name.txt in my place(%s), in the txt there is the names.\n", exeplace.c_str());
			ifstream inName("name.txt");
			int playernum = 0;
			if(!inName.good()) {
				string pre;
				cin >> pre;
				printf("how many players do you want?");
				scanf("%d", &playernum);
				char name_c_str[1024];
				string name;
				for(int i = 1; i <= playernum; i++) {
					sprintf(name_c_str, "%s-%d", pre.c_str(), i);
					name = name_c_str;
					printf("debug: new player's name->%s\n", name.c_str());
					Player* pla = new Player;
					pla->uname = name;
					pla->money = 50;
					pla->cap = new City;
					pla->cap->lv = 1;
					pla->cap->hp = CityHp[1];
					pla->cap->name = name + "'s capital";
					pla->cap->id = 0;
					pla->cap->whos = pla;
					pla->cap->x = (i % (MAPSIZE / 20)) * 20 + 10;
					pla->cap->y = (i / (MAPSIZE / 20)) * 20 + 10;
					cout << pla->cap->x << " " << pla->cap->y << endl;
					players[name] = pla;
				}
			}
			else {
				printf("i will done it soon, please wait...\n");
				string name;
				while(inName >> name) {
					playernum++;
					Player* pla = new Player;
					pla->uname = name;
					pla->money = 50;
					pla->cap = new City;
					pla->cap->lv = 1;
					pla->cap->hp = CityHp[1];
					pla->cap->name = name + "'s capital";
					pla->cap->id = 0;
					pla->cap->whos = pla;
					pla->cap->x = (playernum % (MAPSIZE / 20)) * 20 + 10;
					pla->cap->y = (playernum / (MAPSIZE / 20)) * 20 + 10;
					cout << pla->cap->x << " " << pla->cap->y << endl;
					players[name] = pla;
				}
			}
		}
		else if(str == "NO" || str == "No" || str == "no" || str == "N" || str == "n") {
			printf("Then please copy to my place(%s) or create by your hands\n", exeplace.c_str());
			exit(0);
		}
		else goto __PRINT; 
	}
	inFile.close();
}

void init() {
	cout << (string)MY_DSCS2009 + (string)CODE_DSCS2009 + " Server Version: " + (string)GAME_VERSION << endl;
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		printf("WSAStartup [FAILED]\n");
		quit();
		exit(-1);
	}
	printf("WSAStartup [DONE]\n");
	
	serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(serverSock == NULL) {
		printf("init serverSock [FAILED]\n");
		quit();
		exit(-1);
	}
	printf("init serverSock [DONE]\n");
	
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = PF_INET;
	char name[255];
	char* ip;
	PHOSTENT hostinfo;
	gethostname(name, sizeof(name));
	hostinfo = gethostbyname(name);
	ip = inet_ntoa(*(struct in_addr *) *hostinfo->h_addr_list);
	sockAddr.sin_addr.s_addr = inet_addr(ip);
	printf("GETED IP->%s [DONE]\n", ip);
	
	int port;
	printf("port->");
	scanf("%d", &port);
	sockAddr.sin_port = htons(port);
	printf("GETED PORT->%d [DONE]\n", port);
	
	if(bind(serverSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		printf("binded socket [FAILED]\n");
		quit();
		exit(-1);
	}
	printf("binded socket [DONE]\n");
	
	if(listen(serverSock, 100) < 0) {
		printf("started listen socket [FAILED]\n");
		quit();
		exit(-1);
	}
	printf("started listen socket [DONE]\n");
	
	tAccept.detach();
	printf("detached accept thread [DONE]\n");
	
	int sbuflen = MAXBUFLEN;
	setsockopt(serverSock, SOL_SOCKET, SO_SNDBUF, (const char* )&sbuflen, sizeof(int));
	
	InitBlockMap();
	printf("inited blocks map [DONE]\n");
}

void quit() {
	closesocket(serverSock);
	WSACleanup();
}

void UpdateBlocks_City(City* city) {
	if(block_map[city->x][city->y].city != NULL) {
		printf("error: two or more citys in one place x->%d y->%d\n", city->x, city->y);
		exit(-1);
	}
	block_map[city->x][city->y].city = city;
}

void UpdateBlocks_Army(Army* army) {
	block_map[army->x][army->y].army.push_back(army);
}

void UpdateBlocks() {
	for(int i = 0; i < 1001; i++) {
		for(int j = 0; j < 1001; j++) {
			block_map[i][j].army.clear();
			block_map[i][j].city = NULL;
			block_map[i][j].x = i;
			block_map[i][j].y = j;
		}
	}
	for(auto it = players.begin(); it != players.end(); it++) {
		Player* & pla = it->second;
//		printf("updating... at Player->%s\n", pla->uname.c_str());
		UpdateBlocks_City(pla->cap);
		for(auto jt = pla->city_data.begin();/*jter awa*/ jt != pla->city_data.end(); jt++) {
			UpdateBlocks_City(*jt);
		}
		for(auto jt = pla->army_data.begin();/*jter awa*/ jt != pla->army_data.end(); jt++) {
			UpdateBlocks_Army(*jt);
		}
	}
}

void Save() {
	
}

int main(int argc, char *argv[]) {
	exeplace = argv[0];
	printf("initializing...\n");
	init();
	printf("initialization [DONE]\n");
	while(true) {
		UpdateBlocks(); //oh, my cpu
		Save();
		Sleep(100);
	}
	
	printf("quiting...\n");
	quit();
	printf("quited [DONE]\n");
	system("pause");
	
	return 0;
}

void AcceptThread() {
	int clientidx = 1;
	while(true) {
		SOCKET clientSock;
		SOCKADDR clientAddr;
		sockaddr_in sin;
		int nSize = sizeof(SOCKADDR);
		clientSock = accept(serverSock, (SOCKADDR*)&clientAddr, &nSize);
		if(clientSock == NULL) {
			printf("accept new client socket [FAILED]\n");
			continue;
		}
		memcpy(&sin, &clientAddr, sizeof(sin));
		if((string)inet_ntoa(sin.sin_addr) == "0.0.0.0") {
//			printf("ip==0.0.0.0 [FAILED]\n");
			continue;
		}
		int sbuflen = MAXBUFLEN;
		setsockopt(clientSock, SOL_SOCKET, SO_SNDBUF, (const char* )&sbuflen, sizeof(int));
		printf("new client ip->%s\n", inet_ntoa(sin.sin_addr));
		
		XTcp* client = new XTcp;
		client->online = true;
		client->sock = clientSock;
		string uname = client->RecvEx();
		if(!client->online) {
			delete client;
			continue;
		}
		string ukey = client->RecvEx();
		if(!client->online) {
			delete client;
			continue;
		}
//		while(true) {
//			cout << "get " << client->RecvEx() << endl;
//		}
		if(uname == "[reg_new_user]") {
			uname = "";
			for(auto it = players.begin(); it != players.end(); it++) {
				if(!it->second->isreg()) {
					uname = it->first;
					break;
				}
			}
			if(uname == "") {
				client->SendEx("[FAILED]");
				client->Close();
				delete client;
			}
			else {
				client->SendEx(uname);
				players[uname]->reg = true;
			}
		}
		else if(players[uname]->isreg() && players[uname]->ukey == ukey) {
			online_clt[uname] = client;
			client->SendEx("[DONE]");
		}
		else {
			client->SendEx("[FAILED]");
			client->Close();
			delete client;
		}
	}
}
