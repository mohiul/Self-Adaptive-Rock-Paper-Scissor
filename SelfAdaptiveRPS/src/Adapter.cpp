/*
 * Adaptation.cpp
 *
 *  Created on: Dec 25, 2015
 *      Author: mohiul
 */

#include <sstream>
#include "Adapter.h"

Adapter::Adapter(Rule *rule) {
	this->rule = rule;
}

Adapter::~Adapter() {
	// TODO Auto-generated destructor stub
}

void Adapter::addAction(Action action){
	actions.push_back(action);
}

std::string Adapter::getString() {
	std::ostringstream oss;
	oss << rule->getString() << "-";
	std::list<Action>::const_iterator iterator;
	for (iterator = actions.begin(); iterator != actions.end(); ++iterator) {
		Action action = *iterator;
		switch(action){
		case ADD:
			oss << "ADD; ";
			break;
		case DEL:
			oss << "DEL; ";
			break;
		case MOD:
			oss << "MOD; ";
			break;
		case SHF:
			oss << "SHF; ";
			break;
		}
	}
	return oss.str();
}
