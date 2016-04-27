/*
 * Adaptation.cpp
 *
 *  Created on: Dec 25, 2015
 *      Author: mohiul
 */

#include <sstream>
#include <cstdlib>
#include "Adapter.h"

Adapter::Adapter(Rule *rule) {
	this->rule = rule;
}

Adapter::~Adapter() {
	rule->~Rule();
	actions.clear();
}

void Adapter::addAction(Action action){
	actions.push_back(action);
}

std::list<Action> Adapter::getActions(){
	return actions;
}

Rule *Adapter::getRule(){
	return rule;
}

Adapter* Adapter::generateAdapter() {
	Adapter *adapter = new Adapter(Rule::generateRule());
	int randNum = rand() % 2;
	switch(randNum){
	case 0:
		adapter->addAction(ADD);
		break;
	case 1:
		adapter->addAction(DEL);
		break;
	case 2:
		adapter->addAction(MOD);
		break;
	case 3:
		adapter->addAction(SHF);
		break;
	}
	return adapter;
}

std::string Adapter::getString() {
	std::ostringstream oss;
	oss << rule->getString() << "-";
	std::list<Action>::const_iterator iterator;
	for (iterator = actions.begin(); iterator != actions.end(); ++iterator) {
		Action action = *iterator;
		switch(action){
		case ADD:
			oss << "ADD;";
			break;
		case DEL:
			oss << "DEL;";
			break;
		case MOD:
			oss << "MOD;";
			break;
		case SHF:
			oss << "SHF;";
			break;
		}
	}
	return oss.str();
}
