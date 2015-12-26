/*
 * Rule.cpp
 *
 *  Created on: Nov 14, 2015
 *      Author: mohiul
 */

#include <sstream>
#include "Rule.h"

Rule::Rule(std::string condition, char action) {
	this->condition = condition;
	this->action = action;

}

Rule::~Rule() {

}

std::string Rule::getCondition(){
	return condition;
}

char Rule::getAction(){
	return action;
}

const char *Rule::getString() {
	std::ostringstream oss;
	oss << condition << ":" << action;
	return oss.str().c_str();
}
