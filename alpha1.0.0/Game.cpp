#include "Game.h"

bool Game::Player::isreg() {
	return this->reg;
}

bool Game::Player::isdead() {
	return this->cap == NULL || this->cap->hp <= 0;
}

bool Game::Player::update_city(int x) {
	if(this->money < Game::CityPrice[this->city_data[x]->lv + 1]) return false;
	this->money -= Game::CityPrice[++this->city_data[x]->lv];
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
