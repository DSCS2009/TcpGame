#include "File.h"

using namespace std;
using namespace Game;

void Game::SaveCity(City* city, ofstream& outFile) {
	/*
	Note: We need to save
	int hp, lv, x, y;
	string name;
	*/
	if(city == NULL || city->hp <= 0) {
		outFile << "0 0 0 0 null" << endl;
		return;
	}
	outFile << city->hp << " " << city->lv << " " << city->x << " " << city->y << " " << city->name << endl;
}

void Game::OpenCity(City* city, ifstream& inFile) {
	inFile >> city->hp >> city->lv >> city->x >> city->y >> city->name;
//	cout << city->hp << " " << city->lv << " " << city->x << " " << city->y << " " << city->name << endl;
}

void Game::SaveArmy(Army* army, ofstream& outFile) {
	/*
	Note: We need to save
	int hp, x, y, type, id;
	*/
	if(army == NULL || army->hp <= 0) {
		outFile << "0 0 0 0" << endl;
		return;
	}
	outFile << army->hp << " " << army->x << " " << army->y << " " << army->type << endl;
}

void Game::OpenArmy(Army* army, ifstream& inFile) {
	inFile >> army->hp >> army->x >> army->y >> army->type;
}

void Game::SavePlayer(Player* pla, ofstream& outFile) {
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

void Game::OpenPlayer(Player* pla, ifstream& inFile) {
	pla->cap = new City;
	OpenCity(pla->cap, inFile);
	int len;
	inFile >> len;
	for(int i = 1; i <= len; i++) {
		City* city = new City;
		OpenCity(city, inFile);
		if(city->hp != 0) pla->city_data.push_back(city);
		else pla->city_data.push_back(NULL);
	}
	inFile >> len;
	for(int i = 1; i <= len; i++) {
		Army* army = new Army;
		OpenArmy(army, inFile);
		if(army->hp != 0) pla->army_data.push_back(army);
		else pla->army_data.push_back(NULL);
	}
	inFile >> pla->flag_path >> pla->uname >> pla->ukey;
	inFile >> pla->reg >> pla->money;
}
