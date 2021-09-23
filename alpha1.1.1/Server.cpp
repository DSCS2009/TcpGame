#define MY_DSCS2009 "DSCS2009's"
#define CODE_DSCS2009 " excrement mountain code(little slow)"
#define GAME_VERSION "alpha 1.1.1 1/2 + 0.00001% server and no client and debug a little dscs2009 ohhh"
#define UPDATE_TIME "2021.9.20"
#define START_TO_BURN_MY_CPY/*?? is CPU*/ "qwq"
#include <cstdio>
#include <iostream>
#include <thread>
#include <map>
#include <string>
#include <fstream>
#include <vector>
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
void Open();

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
					pla->flag_path = "unknow";
					pla->uname = name;
					pla->money = 50;
					pla->cap = new City;
					pla->cap->lv = 1;
					pla->cap->hp = CityHp[1];
					pla->cap->name = name + "'s_capital";
					pla->cap->id = 0;
					pla->cap->whos = pla;
					pla->cap->x = (i % (MAPSIZE / 20)) * 20 + 10;
					pla->cap->y = (i / (MAPSIZE / 20)) * 20 + 10;
					cout << pla->cap->x << " " << pla->cap->y << endl;
					players[name] = pla;
				}
			} else {
				printf("i will done it soon, please wait...\n");
				string name;
				while(inName >> name) {
					playernum++;
					Player* pla = new Player;
					pla->flag_path = "unknow";
					pla->uname = name;
					pla->money = 50;
					pla->cap = new City;
					pla->cap->lv = 1;
					pla->cap->hp = CityHp[1];
					pla->cap->name = name + "'s_capital";
					pla->cap->id = 0;
					pla->cap->whos = pla;
					pla->cap->x = (playernum % (MAPSIZE / 20)) * 20 + 10;
					pla->cap->y = (playernum / (MAPSIZE / 20)) * 20 + 10;
					cout << pla->cap->x << " " << pla->cap->y << endl;
					players[name] = pla;
				}
			}
		} else if(str == "NO" || str == "No" || str == "no" || str == "N" || str == "n") {
			printf("Then please copy to my place(%s) or create by your hands\n", exeplace.c_str());
			exit(0);
		} else goto __PRINT;
	}
	else Open(); //let's open it
	inFile.close();
}

void init() {
	cout << (string)MY_DSCS2009 + (string)CODE_DSCS2009 + " Server Version: " + (string)GAME_VERSION + " Update Time: " + (string)UPDATE_TIME << endl;
	
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

void SaveCity(City* city, ofstream& outFile) {
	/*
	Note: We need to save
	int hp, lv, x, y;
	string name;
	*/
	outFile << city->hp << " " << city->lv << " " << city->x << " " << city->y << " " << city->name << endl;
}

void OpenCity(City* city, ifstream& inFile) {
	inFile >> city->hp >> city->lv >> city->x >> city->y >> city->name;
//	cout << city->hp << " " << city->lv << " " << city->x << " " << city->y << " " << city->name << endl;
}

void SaveArmy(Army* army, ofstream& outFile) {
	/*
	Note: We need to save
	int hp, x, y, type, id;
	*/
	outFile << army->hp << " " << army->x << " " << army->y << " " << army->type << endl;
}

void OpenArmy(Army* army, ifstream& inFile) {
	inFile >> army->hp >> army->x >> army->y >> army->type;
}

void SavePlayer(Player* pla, ofstream& outFile) {
	/*
	Note: We need to save
	City* cap;
	vector<Game::City*> city_data;
	vector<Game::Army*> army_data;
	string flag_path, uname, ukey;
	bool reg;
	int money;
	*/
	//output capital
	SaveCity(pla->cap, outFile);
	outFile << pla->city_data.size() << endl;
	for(auto it = pla->city_data.begin(); it != pla->city_data.end(); it++) {
		SaveCity(*it, outFile);
	}
	outFile << pla->army_data.size() << endl;
	for(auto it = pla->army_data.begin(); it != pla->army_data.end(); it++) {
		SaveArmy(*it, outFile);
	}
	outFile << pla->flag_path << " " << pla->uname << " " << pla->ukey << endl;
	outFile << pla->reg << " " << pla->money << endl;
}

void OpenPlayer(Player* pla, ifstream& inFile) {
	pla->cap = new City;
	OpenCity(pla->cap, inFile);
	int len;
	inFile >> len;
	for(int i = 1; i <= len; i++) {
		City* city = new City;
		OpenCity(city, inFile);
		pla->city_data.push_back(city);
	}
	inFile >> len;
	for(int i = 1; i <= len; i++) {
		Army* army = new Army;
		OpenArmy(army, inFile);
		pla->army_data.push_back(army);
	}
	inFile >> pla->flag_path >> pla->uname >> pla->ukey;
	inFile >> pla->reg >> pla->money;
}

void Save() {
	/*
	Note: We need to save
	map<string, Player*> players;
	*/
	ofstream outFile("data.txt");
	outFile << players.size() << endl;
	for(auto it = players.begin(); it != players.end(); it++) {
		SavePlayer(it->second, outFile);
	}
}

void Open() {
	ifstream inFile("data.txt");
	int len;
	inFile >> len;
//	cout << "len->" << len << endl;
	for(int _ = 1; _ <= len; _++) { //awa
		Player* pla = new Player;
		OpenPlayer(pla, inFile);
		players[pla->uname] = pla;
	}
}

int main(int argc, char *argv[]) {
	exeplace = argv[0];
	printf("initializing...\n");
	init();
	printf("initialization [DONE]\n");
	while(true) {
		bool no_body_is_online = true;
		while(no_body_is_online) {
			for(auto it = online_clt.begin(); it != online_clt.end(); it++) {
				if(it->second->online) {
					no_body_is_online = false;
					break;
				}
			}
			#ifndef START_TO_BURN_MY_CPY
			Sleep(100); //i don't want to burn my cpu
			#endif
			if(no_body_is_online) {
//				printf("no body is online now qwq.\n");
				//debug
				break;
			}
		}
		for(auto it = players.begin(); it != players.end(); it++) {
			if(it->second->isdead()) continue;
			UpdateBlocks(); //oh, my cpu
			Player* & pla = it->second;
			printf("%s's turn now.\n", pla->uname.c_str());
			if(!pla->isreg() || online_clt[pla->uname] == NULL || !online_clt[pla->uname]->online) {
				//auto ai
				//cooking an egg on my cpu
				printf("debug: auto ai mode for player %s\n", pla->uname.c_str());

			} else {
				//player self
				//cooking an egg on my cpu and the player's cpu
			}
			#ifndef START_TO_BURN_MY_CPY
			Sleep(100);
			#endif
		}
		Save();
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
			} else {
				client->SendEx(uname);
				players[uname]->reg = true;
			}
		} else if(players[uname]->isreg() && players[uname]->ukey == ukey) {
			online_clt[uname] = client;
			client->SendEx("[DONE]");
		} else {
			client->SendEx("[FAILED]");
			client->Close();
			delete client;
		}
	}
}
