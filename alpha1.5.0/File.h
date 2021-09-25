#ifndef FILE_H
#define FILE_H

#include "Game.h"
#include <fstream>

using namespace std;
using namespace Game;

namespace Game{
	void SaveCity(City* city, ofstream& outFile);
	void OpenCity(City* city, ifstream& inFile);
	void SaveArmy(Army* army, ofstream& outFile);
	void OpenArmy(Army* army, ifstream& inFile);
	void SavePlayer(Player* pla, ofstream& outFile);
	void OpenPlayer(Player* pla, ifstream& inFile);
}

#endif //!FILE_H
