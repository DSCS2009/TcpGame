#include "AutoAi.h"

using namespace Game;

bool Game::ChooseThings_ai(Player* pla) {
	srand(time(0) + clock());
	int computer_want = rand() % 100 + 1;
//	cout << computer_want << endl;
	if(computer_want <= 10) { //10% attack!
		printf("attack!\n");
		bool find = false;
		for(auto it = pla->army_data.begin(); it != pla->army_data.end(); it++) {
			int & x = (*it)->x;
			int & y = (*it)->y;
			Block & blk = block_map[x][y];
			if(blk.city->whos != pla) {
				blk.city->hp -= ArmyAtk[(*it)->type];
				if(blk.city->hp <= 0) {
					Player* city_whos = blk.city->whos;
					if(city_whos->cap == blk.city) { //fuckkkk kill a player
						delete city_whos->cap;
					}
					else {
						delete city_whos->city_data[blk.city->id];
					}
					delete blk.city;
				}
				find = true;
				break;
			}
			for(auto jt = blk.army.begin(); jt != blk.army.end(); jt++) {
				if((*jt)->whos != pla) {
					(*jt)->hp -= ArmyAtk[(*it)->type];
					if((*jt)->hp <= 0) {
						Player* army_whos = (*jt)->whos;
						delete army_whos->army_data[(*jt)->id];
						delete *jt;
					}
					find = true;
					break;
				}
			}
			if(find) break;
		}
		return find;
	}
	else if(computer_want > 10 && computer_want <= 25) { //15% add a new city
		printf("add a new city!\n");
		//code here 
	}
	else if(computer_want > 25 && computer_want <= 60) { //35% update a city
		printf("update a city!\n");
		//code here 
	}
	else if(computer_want > 60 && computer_want <= 75) { //15% add a new city
		printf("add a new city!\n");
		//code here 
	}
	else if(computer_want > 75 && computer_want <= 80) { //5% move an army
		printf("move an army!\n");
		//code here 
	}
	else if(computer_want > 80 && computer_want <= 90) { //10% add a new army
		printf("add a new army!\n");
		//code here 
	}
	else if(computer_want > 90 && computer_want <= 100) { //10% healing self
		printf("healing self!\n");
		//code here 
	}
	return true;
}
