#ifndef PLAYER_H
#define PLAYER_H 1919810

#include <vector>
#include <string>

using namespace std;

namespace Game{
	class City;
	class Army;
	
	class Player{
	public:
		City* cap;
		vector<Game::City*> city_data;
		vector<Game::Army*> army_data;
		string flag_path, uname, ukey;
		bool reg;
		int money;
		
		bool isreg();
		bool isdead();
		//outside in Server.cpp now
//		bool add_city(int x, int y);
		bool update_city(int x);
		bool add_army(int x, int y, int type);
	};
	
	const int CityPrice[] = {20, 50, 100, 200/*......*/};
	const int CityHp[] = {0, 100, 200, 500, 1000/*......*/};
	struct City{
	public:
		int hp, lv, x, y, id;
		string name;
		Game::Player* whos;
	};
	
	const int ArmyPrice[] = {20, 50, 100, 150, 200/*......*/};
	const int ArmyHp[] = {20, 50, 100, 150, 200/*......*/};
	const int ArmyAtk[] = {20, 50, 100, 150, 200/*......*/};
	struct Army{
	public:
		int hp, x, y, type, id;
		Game::Player* whos;
	};
	
	struct Block{
	public:
		int x, y, type;
		Game::City* city;
		vector<Game::Army*> army;
	};
}

#endif //!PLAYER_H
