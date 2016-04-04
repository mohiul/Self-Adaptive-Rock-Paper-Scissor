/*
 * Player.cpp
 *
 *  Created on: Nov 14, 2015
 *      Author: mohiul
 */

#include "RuleEngine.h"

#include <cstdlib>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>
#include "SelfAdaptiveRPS.h"
#include "Utils.h"

RuleEngine::RuleEngine(Player* player) {
	this->player = player;
	parser = new RuleParser(this);
	fitness = 0;
	learningFactor = 0.5;
}

RuleEngine::~RuleEngine() {
	parser->~RuleParser();
	for (list<Rule*>::const_iterator iterator = rules.begin();
			iterator != rules.end();
			++iterator) {
		(*iterator)->~Rule();
	}
	rules.clear();
	for (list<Adapter*>::const_iterator iterator = adapters.begin();
			iterator != adapters.end();
			++iterator) {
		(*iterator)->~Adapter();
	}
	adapters.clear();
}

void RuleEngine::addHistory(char opponentMove){
	char ownMove = moveHistory.front();
	if(ownMove == 'R' && opponentMove == 'R'){
		resultHistory.push_front('D');
	} else if(ownMove == 'R' && opponentMove == 'P'){
		resultHistory.push_front('L');
	} else if(ownMove == 'R' && opponentMove == 'S'){
		resultHistory.push_front('W');
	} else if(ownMove == 'P' && opponentMove == 'R'){
		resultHistory.push_front('W');
	} else if(ownMove == 'P' && opponentMove == 'P'){
		resultHistory.push_front('D');
	} else if(ownMove == 'P' && opponentMove == 'S'){
		resultHistory.push_front('L');
	} else if(ownMove == 'S' && opponentMove == 'R'){
		resultHistory.push_front('L');
	} else if(ownMove == 'S' && opponentMove == 'P'){
		resultHistory.push_front('W');
	} else if(ownMove == 'S' && opponentMove == 'S'){
		resultHistory.push_front('D');
	}
	calculateFitness();
}

float RuleEngine::calculateFitness() {
	char result = resultHistory.front();
	switch(result){
	case 'W':
		fitness = (1 - learningFactor)*fitness + learningFactor * 1;
		break;
	case 'D':
		fitness = (1 - learningFactor)*fitness + learningFactor * 0;
		break;
	case 'L':
		fitness = (1 - learningFactor)*fitness + learningFactor * (-1);
		break;
	}
	return fitness;
}

char RuleEngine::nextMove() {
	char nextMove = parser->nextMove();
	moveHistory.push_front(nextMove);
	return nextMove;
}

void RuleEngine::addRule(Rule* rule) {
	rules.push_back(rule);
}

void RuleEngine::addAdapter(Adapter* adapter) {
	adapters.push_back(adapter);
}

void RuleEngine::adapt() {
	list<Adapter*>::const_iterator iterator;
	for (iterator = adapters.begin(); iterator != adapters.end(); ++iterator) {
		Adapter *adapter = *iterator;
		list<Action> actions = adapter->getActions();
		list<Action>::const_iterator iterator;
		for (iterator = actions.begin(); iterator != actions.end(); ++iterator) {
			Action action = *iterator;
			switch(action) {
			case ADD:
				actionAddRule(adapter->getRule());
				break;
			case DEL:
				actionDeleteRule(adapter->getRule());
				break;
			case MOD:
				actionModifyRule(adapter->getRule());
				break;
			case SHF:
				actionShiftRule(adapter->getRule());
				break;
			}
		}
	}
}

void RuleEngine::selfAdapt() {
	list<Adapter*>::const_iterator iterator;
	for (iterator = adapters.begin(); iterator != adapters.end(); ++iterator) {
		Adapter *adapter = *iterator;
		list<Action> actions = adapter->getActions();
		list<Action>::const_iterator iterator;
		for (iterator = actions.begin(); iterator != actions.end(); ++iterator) {
			Action action = *iterator;
			switch(action) {
			case ADD:
				actionAddAdapter(adapter);
				break;
			case DEL:
				actionDeleteAdapter(adapter);
				break;
			case MOD:
				actionModifyAdapter(adapter);
				break;
			case SHF:
				actionShiftAdapter(adapter);
				break;
			}
		}
	}
}

void RuleEngine::actionAddRule(Rule *adapterRule){
	bool addRule = true;
	list<Rule*>::const_iterator iterator;
	for (iterator = rules.begin(); iterator != rules.end(); ++iterator) {
		Rule *rule = *iterator;
		RuleComparison comp = rule->compare(adapterRule);
		switch(comp){
		case GENERAL:
			rules.remove(rule);
			rules.push_back(adapterRule);
			addRule = false;
			break;
		case SPECIFIC:
			addRule = false;
			break;
		case EQUAL:
			addRule = false;
			break;
		case NOT_EQUAL:
			break;
		}
		if(!addRule){
			break;
		}
	}
	if(addRule){
		rules.push_back(adapterRule);
	}
}

void RuleEngine::actionDeleteRule(Rule *adapterRule){
	bool deleteRule = false;
	list<Rule*>::const_iterator iterator;
	for (iterator = rules.begin(); iterator != rules.end(); ++iterator) {
		Rule *rule = *iterator;
		RuleComparison comp = rule->compare(adapterRule);
		switch(comp){
		case GENERAL:
			//Don't do anything
			break;
		case SPECIFIC:
			deleteRule = true;
			break;
		case EQUAL:
			deleteRule = true;
			break;
		case NOT_EQUAL:
			break;
		}
		if(deleteRule){
			break;
		}
	}
	if(deleteRule){
		rules.remove(adapterRule);
	}
}

void RuleEngine::actionModifyRule(Rule *adapterRule){

}

void RuleEngine::actionShiftRule(Rule *adapterRule){

}

void RuleEngine::actionAddAdapter(Adapter *adapter){
	bool addAction = true;
	list<Adapter*>::const_iterator iterator;
	for (iterator = adapters.begin(); iterator != adapters.end(); ++iterator) {
		Adapter *itrAdapter = *iterator;
		RuleComparison comp = adapter->getRule()->compare(adapter->getRule());
		switch(comp){
		case GENERAL:
			adapters.remove(itrAdapter);
			adapters.push_back(adapter);
			addAction = false;
			break;
		case SPECIFIC:
			addAction = false;
			break;
		case EQUAL:
			addAction = false;
			break;
		case NOT_EQUAL:
			break;
		}
		if(!addAction){
			break;
		}
	}
	if(addAction){
		adapters.push_back(adapter);
	}
}

void RuleEngine::actionDeleteAdapter(Adapter *adapter){
	bool deleteAdapter = false;
	list<Adapter*>::const_iterator iterator;
	for (iterator = adapters.begin(); iterator != adapters.end(); ++iterator) {
		Adapter *itrAdapter = *iterator;
		RuleComparison comp = itrAdapter->getRule()->compare(adapter->getRule());
		switch(comp){
		case GENERAL:
			//Don't do anything
			break;
		case SPECIFIC:
			deleteAdapter = true;
			break;
		case EQUAL:
			deleteAdapter = true;
			break;
		case NOT_EQUAL:
			break;
		}
		if(deleteAdapter){
			break;
		}
	}
	if(deleteAdapter){
		adapters.remove(adapter);
	}
}

void RuleEngine::actionModifyAdapter(Adapter *adapter){

}

void RuleEngine::actionShiftAdapter(Adapter *adapter){

}

list<Rule*> RuleEngine::getRules(){
	return rules;
}

list<Adapter*> RuleEngine::getAdapters(){
	return adapters;
}

list<char> RuleEngine::getResultHistory(){
	return resultHistory;
}

list<char> RuleEngine::getMoveHistory(){
	return moveHistory;
}

float RuleEngine::getFitness(){
	return fitness;
}

/*
int main(int argc, char** argv)
{
	Player *p = new Player("P");
	Rule *r1 = new Rule("2>0<85", 'P');
	Rule *r2 = new Rule("0?34", 'P');
	Rule *r3 = new Rule("012345?48", 'R');
	cout << "r1 specificity: " << r1->getSpecificity() << endl;
	cout << "r2 specificity: " << r2->getSpecificity() << endl;
	cout << "r3 specificity: " << r3->getSpecificity() << endl;
	p->addRule(r1);
	p->addRule(r2);
	p->addRule(r3);
//	p->addRule(new Rule("0?11", 'R'));
//	p->addRule(new Rule("2?13", 'S'));
//	p->addRule(new Rule("123?13", 'R'));
	p->addHistory('R', 'S');
	p->addHistory('S', 'P');
	p->addHistory('P', 'R');
	p->addHistory('S', 'S');
	p->addHistory('P', 'P');
	p->addHistory('R', 'R');
	p->addHistory('S', 'R');
	p->addHistory('P', 'S');
	p->addHistory('R', 'P');
	p->printHistory();
	cout << "next move: " << p->nextMove();
	return 0;
}
*/
