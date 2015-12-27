/*
 * Rule.cpp
 *
 *  Created on: Nov 14, 2015
 *      Author: mohiul
 */

#include <sstream>
#include "Rule.h"
#include "SelfAdaptiveRPS.h"

Rule::Rule(std::string condition, char action) {
	this->condition = condition;
	this->action = action;
	howRecent = 0;
	specifcity = 0;
	for(unsigned int i = 0; i < condition.length(); i++){
		if(condition.at(i) == '?'){
			specifcity -= 9*(int)toDigit(condition.at(++i));
		} else if(condition.at(i) == '<'){
			int nextDigit = (int)toDigit(condition.at(++i));
			while(nextDigit > 0){
				specifcity--;
				nextDigit--;
			}
		} else if(condition.at(i) == '>'){
			int nextDigit = (int)toDigit(condition.at(++i));
			while(nextDigit < 8){
				specifcity--;
				nextDigit++;
			}
		}
	}
}

Rule::~Rule() {

}

std::string Rule::getCondition(){
	return condition;
}

char Rule::getAction(){
	return action;
}

std::string Rule::getString() {
	std::ostringstream oss;
	oss << condition << ":" << action;
	return oss.str();
}

std::string Rule::getDetailString() {
	std::ostringstream oss;
	oss << condition << ":" << action << " specificity: " << specifcity << " how recent: " << howRecent;
	return oss.str();
}


int Rule::getSpecificity(){
	return specifcity;
}

void Rule::setHowRecent(int howRecent){
	this->howRecent = howRecent;
}

int Rule::getHowRecent(){
	return howRecent;
}
