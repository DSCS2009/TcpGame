#include "AutoAi.h"
//#define false true

bool Game::ChooseThings_ai(Player* pla) {
	srand(clock());
	int random_variable = rand(), computer_want = 0;
	{ // i don't want to use x outside
		int x = 100;
		while(x > 99) x = 1 + rand() / ((RAND_MAX + 1u) / 100);
		computer_want = x;
	}
	#ifdef AIDEBUG
//	cout << computer_want << endl;
	printf("cw->%d\n", computer_want);
	#endif
	if(false && computer_want >= 1 && computer_want <= 10) { //10% attack!
//		printf("attack!\n");
		bool find = false;
		for(auto it = pla->army_data.begin(); it != pla->army_data.end(); it++) {
			if((*it) == NULL) continue;
			int & x = (*it)->x;
			int & y = (*it)->y;
//			printf("at 19\n");
			Block & blk = block_map[x][y];
			if(blk.city != NULL && blk.city->whos != NULL && blk.city->hp > 0 && blk.city->whos != pla) {
//				printf("pla->%X\n", blk.city->whos);
//				printf("at 22\n");
//				printf("%s try to attack %s's city\n", pla->uname.c_str(), blk.city->whos->uname.c_str());
				blk.city->hp -= ArmyAtk[(*it)->type];
//				printf("hp->%d\n", blk.city->hp);
				if(blk.city->hp <= 0) {
					printf("%s's city dead\n", blk.city->whos->uname.c_str());
					Player* city_whos = blk.city->whos;
					if(city_whos->cap == blk.city) { //fuckkkk kill a player
						city_whos->cap = NULL;
						printf("%s dead\n", city_whos->uname.c_str());
					}
					else {
						city_whos->city_data[blk.city->id] = NULL;
					}
					delete blk.city;
				}
				find = true;
				break;
			}
			for(auto jt = blk.army.begin(); jt != blk.army.end(); jt++) {
				if((*jt) != NULL && (*jt)->whos != pla) {
					(*jt)->hp -= ArmyAtk[(*it)->type];
					if((*jt)->hp <= 0) {
						printf("%s's army was killed by %s\n", (*jt)->whos->uname.c_str(), pla->uname.c_str());
						Player* army_whos = (*jt)->whos;
						army_whos->army_data[(*jt)->id] = NULL;
						delete *jt;
					}
					find = true;
					break;
				}
			}
			if(find) break;
		}
		if(!find) return false;
		return true;
	}
	else if(computer_want > 10 && computer_want <= 40) { //15% add a new city
//		printf("add a new city!\n");
		const int dx[] = {0, 0, 1, -1};
		const int dy[] = {1, -1, 0, 0};
		//capital
		City* city = pla->cap;
		bool done = false;
		for(int i = 0; i < 4; i++) {
			int x = city->x + dx[i];
			int y = city->y + dy[i];
			if(block_map[x][y].type != 0) continue;
			if(block_map[x][y].city == NULL) {
//				printf("debug: try to add a new city at x->%d y->%d\n", x, y);
				if(AddNewCity(pla, x, y)) {
//					printf("debug: new city done at x->%d y->%d\n", x, y);
					done = true;
					break;
				}
				else continue;
			}
		}
		if(done) return true;
		for(auto it = pla->city_data.begin(); it != pla->city_data.end(); it++) {
			city = (*it);
			if(city == NULL || city->hp <= 0) continue;
			for(int i = 0; i < 4; i++) {
				int x = city->x + dx[i];
				int y = city->y + dy[i];
				if(block_map[x][y].type != 0) continue;
				if(block_map[x][y].city == NULL) {
//					printf("debug: try to add a new city at x->%d y->%d\n", x, y);
					if(AddNewCity(pla, x, y)) {
//						printf("debug: new city done at x->%d y->%d\n", x, y);
						done = true;
						break;
					}
					else continue;
				}
			}
			if(done) break;
		}
		if(done) return true;
		return false;
	}
	else if(computer_want > 40 && computer_want <= 75) { //35% update a city
//		printf("update a city!\n");
		int cityid = rand() % (pla->city_data.size() + 1);
		return pla->update_city(cityid - 1);
	}
	else if(computer_want > 75 && computer_want <= 80) { //5% move an army
//		printf("move an army!\n");
		if(pla->army_data.size() == 0) return false;
		int armyid = rand() % pla->army_data.size();
		int dir = rand() % 4;
		const int dx[] = {0, 0, 1, -1};
		const int dy[] = {1, -1, 0, 0};
		if(dx[dir] != 0) {
			for(int i = pla->army_data[armyid]->x; pla->army_data[armyid]->x != pla->army_data[armyid]->x + dx[dir] * ArmySpd[pla->army_data[armyid]->type]; dx[dir] == 1 ? i++ : i--) {
				if(block_map[i][pla->army_data[armyid]->y].type != 0) {
					return false;
				}
			}
		}
		else {
			for(int i = pla->army_data[armyid]->y; pla->army_data[armyid]->y != pla->army_data[armyid]->y + dy[dir] * ArmySpd[pla->army_data[armyid]->type]; dy[dir] == 1 ? i++ : i--) {
				if(block_map[pla->army_data[armyid]->x][i].type != 0) {
					return false;
				}
			}
		}
		int x = pla->army_data[armyid]->x + dx[dir] * ArmySpd[pla->army_data[armyid]->type];
		int y = pla->army_data[armyid]->y + dy[dir] * ArmySpd[pla->army_data[armyid]->type];
		if(x <= 0 || y <= 0 || x > MAPSIZE || y > MAPSIZE) return false;
		//the army says: "I want to die!"
//		printf("x from %d to %d\n", pla->army_data[armyid]->x, x);
//		printf("y from %d to %d\n", pla->army_data[armyid]->y, y);
		pla->army_data[armyid]->x = x;
		pla->army_data[armyid]->y = y;
	}
	else if(computer_want > 80 && computer_want <= 90) { //10% add a new army
//		printf("add a new army!\n");
		int armytype = rand() % ArmyTypeSize;
		int cityid = rand() % (pla->city_data.size() + 1) - 1;
//		printf("armytype->%d cityid->%d\n", armytype, cityid);
		if(cityid != -1 && pla->city_data[cityid] == NULL) return false;
//		if(cityid != -1) printf("%d %d\n", pla->city_data[cityid]->x, pla->city_data[cityid]->y);
		if(cityid != -1) return pla->add_army(pla->city_data[cityid]->x, pla->city_data[cityid]->y, armytype);
		else return pla->add_army(pla->cap->x, pla->cap->y, armytype);
	}
	else if(computer_want > 90 && computer_want <= 100) { //10% healing self
//		printf("healing self!\n");
		int city_or_army = rand() % 2;
		int type = rand() % HealTypeSize;
		if(pla->money < HealPrice[type]) return false;
		if(city_or_army == 0) { //city
			int cityid = rand() % (pla->city_data.size() + 1) - 1;
			if(cityid == -1) {
				if(pla->cap == NULL) return false;
				if(pla->cap->hp + HealHp[type] > CityHp[pla->cap->lv]) return false;
				pla->cap->hp += HealHp[type];
			}
			else {
				if(pla->city_data[cityid] == NULL) return false;
				if(pla->city_data[cityid]->hp + HealHp[type] > CityHp[pla->city_data[cityid]->lv]) return false;
				pla->city_data[cityid]->hp += HealHp[type];
			}
		}
		else if(city_or_army == 1) { //army
			if(pla->army_data.size() == 0) return false;
			int armyid = rand() % pla->army_data.size();
			if(pla->army_data[armyid] == NULL) return false;
			if(pla->army_data[armyid]->hp + HealHp[type] > ArmyHp[pla->army_data[armyid]->type]) return false;
			pla->army_data[armyid]->hp += HealHp[type];
		}
	}
	
	return true;
}

bool Game::MkThing_ai(Player* pla, string cmd, XTcp* client) {
	printf("get from player %s command->%s\n", pla->uname.c_str(), cmd.c_str());
	stringstream ss_cmd;
	string head_cmd;
	ss_cmd << cmd;
	ss_cmd >> head_cmd;
	if(head_cmd == "[GETBLOCK]") {
		int x, y;
		string output = "[BLOCKDATA] ";
		ss_cmd >> x >> y;
		for(int i = x; i <= x + SCREEN_ROW; i++) {
			for(int j = y; j <= y + SCREEN_LINE; j++) {
				if(x <= 0 || x > MAPSIZE || y <= 0 || y > MAPSIZE) {
					client->SendEx("[FAILED]");
					return false;
				}
				output += to_string(i) + " " + to_string(j) + " " + to_string(block_map[i][j].type) + " ";
				output += (block_map[i][j].city == NULL || block_map[i][j].city->hp <= 0) ? "false " : 
					"true " + 
					to_string(block_map[i][j].city->hp) + " " + 
					to_string(block_map[i][j].city->id) + " " + 
					to_string(block_map[i][j].city->lv) + " " +
					block_map[i][j].city->name + " " +
					block_map[i][j].city->whos->uname + " "
					; //have city
				Block & block = block_map[i][j];
				output += to_string(block.army.size()) + " ";
				//code here
				for(int k = 0; k < block.army.size(); k++) {
					output += 
						to_string(block.army[k]->hp) + " " +
						to_string(block.army[k]->type) + " " +
						to_string(block.army[k]->id) + " " +
						block.army[k]->whos->uname + " "
					;
				}
			}
		}
		client->SendEx(output);
//		printf("return %s\n", output.c_str());
	}
	
	return true;
}
