#ifndef AUTOAI_H
#define AUTOAI_H

#include "Game.h"
#include "MyDefines.h"
#include "XTcp.h"
#include <cstdio>
#include <ctime>
#include <random>
#include <cstdio>
#include <string>
#include <map>

using namespace std;
using namespace Game;

extern map<string, Player*> players;
extern Block block_map[MAPSIZE + 1][MAPSIZE + 1];
extern bool AddNewCity(Player* pla, int x, int y);

namespace Game{
	bool ChooseThings_ai(Player* pla);
	bool MkThing_ai(Player* pla, string cmd, XTcp* client);
}

#endif
