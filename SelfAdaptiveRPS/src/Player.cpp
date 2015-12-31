/*
 * Player.cpp
 *
 *  Created on: Nov 14, 2015
 *      Author: mohiul
 */

#include <cstdlib>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>
#include "Player.h"
#include "SelfAdaptiveRPS.h"
#include "Utils.h"

Player::Player(string name) {
	this->name = name;
	winCount = 0;
	looseCount = 0;
	drawCount = 0;
	parser = new RuleParser(this);
}

Player::~Player() {
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

char Player::nextMove() {
	return parser->nextMove();
}

void Player::addHistory(char ownMove, char opponentMove){
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

void Player::addRule(Rule* rule) {
	rules.push_back(rule);
}

void Player::addAdapter(Adapter* adapter) {
	adapters.push_back(adapter);
}

void Player::adapt() {
	list<Adapter*>::const_iterator iterator;
	for (iterator = adapters.begin(); iterator != adapters.end(); ++iterator) {
		Adapter *adapter = *iterator;
		list<Action> actions = adapter->getActions();
		list<Action>::const_iterator iterator;
		for (iterator = actions.begin(); iterator != actions.end(); ++iterator) {
			Action action = *iterator;
			switch(action) {
			case ADD:
				actionAdd(adapter->getRule());
				break;
			case DEL:
				actionDelete(adapter->getRule());
				break;
			case MOD:
				actionModify(adapter->getRule());
				break;
			case SHF:
				actionShift(adapter->getRule());
				break;
			}
		}
	}
}

void Player::actionAdd(Rule *adapterRule){
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

void Player::actionDelete(Rule *adapterRule){
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

void Player::actionModify(Rule *adapterRule){

}

void Player::actionShift(Rule *adapterRule){

}

string Player::getEntireHistoryStr(){
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

string Player::getCurrentHistoryStr(bool flipStr){
	ostringstream oss;
	if(flipStr){
		oss << moveHistory.front() << " " << history.front() << " " << resultHistory.front();
	} else {
		oss << resultHistory.front() << " " << history.front() << " " << moveHistory.front();
	}
	return oss.str();
}

void Player::printHistory(){
	list<char>::const_iterator iterator;
	cout << name << " history: ";
	for (iterator = history.begin(); iterator != history.end(); ++iterator) {
	    cout << " " << *iterator;
	}
	cout << endl;
}

void Player::printHistory(list<char> history){
	list<char>::const_iterator iterator;
	cout << name << " history: ";
	for (iterator = history.begin(); iterator != history.end(); ++iterator) {
	    cout << " " << *iterator;
	}
	cout << endl;
}

list<Rule*> Player::getRules(){
	return rules;
}

list<Adapter*> Player::getAdapters(){
	return adapters;
}

void Player::setName(string name){
	this->name = name;
}

string Player::getName(){
	return name;
}

int Player::getWin() {
	return winCount;
}

int Player::getLoose() {
	return looseCount;
}

int Player::getDraw() {
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
