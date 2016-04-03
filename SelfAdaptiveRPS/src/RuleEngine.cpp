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

RuleEngine::RuleEngine(string name) {
	this->name = name;
	winCount = 0;
	looseCount = 0;
	drawCount = 0;
	parser = new RuleParser(this);
}

RuleEngine::~RuleEngine() {
	parser->~RuleParser();
	history.clear();
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

char RuleEngine::nextMove() {
	return parser->nextMove();
}

void RuleEngine::addHistory(char ownMove, char opponentMove){
	moveHistory.push_front(ownMove);
	char gameResult = 0;
	if(ownMove == 'R' && opponentMove == 'R'){
		gameResult = '3';
		drawCount++;
		resultHistory.push_front('D');
	} else if(ownMove == 'R' && opponentMove == 'P'){
		gameResult = '0';
		looseCount++;
		resultHistory.push_front('L');
	} else if(ownMove == 'R' && opponentMove == 'S'){
		gameResult = '8';
		winCount++;
		resultHistory.push_front('W');
	} else if(ownMove == 'P' && opponentMove == 'R'){
		gameResult = '6';
		winCount++;
		resultHistory.push_front('W');
	} else if(ownMove == 'P' && opponentMove == 'P'){
		gameResult = '4';
		drawCount++;
		resultHistory.push_front('D');
	} else if(ownMove == 'P' && opponentMove == 'S'){
		gameResult = '1';
		looseCount++;
		resultHistory.push_front('L');
	} else if(ownMove == 'S' && opponentMove == 'R'){
		gameResult = '2';
		looseCount++;
		resultHistory.push_front('L');
	} else if(ownMove == 'S' && opponentMove == 'P'){
		gameResult = '7';
		winCount++;
		resultHistory.push_front('W');
	} else if(ownMove == 'S' && opponentMove == 'S'){
		gameResult = '5';
		drawCount++;
		resultHistory.push_front('D');
	}
	history.push_front(gameResult);

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

string RuleEngine::getEntireHistoryStr(){
	ostringstream oss;
	list<char>::const_iterator iterator;
	for (iterator = history.begin(); iterator != history.end(); ++iterator) {
		oss << " " << *iterator;
	}
	oss << endl;

	for (iterator = moveHistory.begin(); iterator != moveHistory.end(); ++iterator) {
		oss << " " << *iterator;
	}
	oss << endl;
	return oss.str();
}

string RuleEngine::getCurrentHistoryStr(bool flipStr){
	ostringstream oss;
	if(flipStr){
		oss << moveHistory.front() << " " << history.front() << " " << resultHistory.front();
	} else {
		oss << resultHistory.front() << " " << history.front() << " " << moveHistory.front();
	}
	return oss.str();
}

void RuleEngine::printHistory(){
	list<char>::const_iterator iterator;
	cout << name << " history: ";
	for (iterator = history.begin(); iterator != history.end(); ++iterator) {
	    cout << " " << *iterator;
	}
	cout << endl;
}

void RuleEngine::printHistory(list<char> history){
	list<char>::const_iterator iterator;
	cout << name << " history: ";
	for (iterator = history.begin(); iterator != history.end(); ++iterator) {
	    cout << " " << *iterator;
	}
	cout << endl;
}

list<Rule*> RuleEngine::getRules(){
	return rules;
}

list<Adapter*> RuleEngine::getAdapters(){
	return adapters;
}

void RuleEngine::setName(string name){
	this->name = name;
}

string RuleEngine::getName(){
	return name;
}

int RuleEngine::getWin() {
	return winCount;
}

int RuleEngine::getLoose() {
	return looseCount;
}

int RuleEngine::getDraw() {
	return drawCount;
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
