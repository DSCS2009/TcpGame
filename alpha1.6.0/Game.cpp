#include "Game.h"

bool Game::Player::isreg() {
	return this->reg;
}

bool Game::Player::isdead() {
	return this->cap == NULL || this->cap->hp <= 0;
}

//-1 is capital
bool Game::Player::update_city(int x) {
	City* city = this->cap;
	if(x != -1) city = this->city_data[x];
	if(city == NULL || city->hp <= 0) return false;
	if(this->money < Game::CityPrice[city->lv + 1]) return false;
	this->money -= Game::CityPrice[++city->lv];
	city->hp += CityHp[city->lv] - CityHp[city->lv - 1];
	return true;
}

bool Game::Player::add_army(int x, int y, int type) {
	if(this->money < Game::ArmyPrice[type]) return false;
	this->money -= Game::ArmyPrice[type];
	Game::Army* new_army = new Game::Army;
	new_army->hp = Game::ArmyHp[type];
	new_army->type = type;
	new_army->x = x;
	new_army->y = y;
	new_army->id = this->army_data.size();
	new_army->whos = this;
	this->army_data.push_back(new_army);
	return true;
}
