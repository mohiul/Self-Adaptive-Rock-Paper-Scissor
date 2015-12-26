/*
 * Player.cpp
 *
 *  Created on: Nov 14, 2015
 *      Author: mohiul
 */

#include <cstdlib>
#include <sstream>
#include "Player.h"

Player::Player(std::string name) {
	this->name = name;
	winCount = 0;
	looseCount = 0;
	drawCount = 0;
}

Player::~Player() {
	history.clear();
	for (std::list<Rule*>::const_iterator iterator = rules.begin();
			iterator != rules.end();
			++iterator) {
		(*iterator)->~Rule();
	}
	rules.clear();
	for (std::list<Adapter*>::const_iterator iterator = adapters.begin();
			iterator != adapters.end();
			++iterator) {
		(*iterator)->~Adapter();
	}
	adapters.clear();
}

char Player::nextMove() {
	char moveToReturn = 0;
	bool match = false;
	if(history.size() > 0){
		std::list<Rule*>::const_iterator iterator;
		for (iterator = rules.begin(); iterator != rules.end(); ++iterator) {
		    Rule* rule = *iterator;
		    const char *expr = rule->getCondition().c_str();
		    match = false;
		    int i; std::list<char>::const_iterator historyItr;
		    for(i = sizeof(expr) - 2, historyItr = history.begin();
		    		i >= 0 && historyItr != history.end();
		    		i--, historyItr++){
		    	char historyVal = *historyItr;
		    	if(expr[i] == '?'){
		    		continue;
		    	} else if(historyVal != expr[i]) {
		    		break;
		    	}
		    }
		    if(i == -1) match = true;
		    if(match){
		    	moveToReturn = rule->getAction();
		    	break;
		    }
		}
	}
	if(!match){
		moveToReturn = nextRandomMove();
	}
	return moveToReturn;
}

char Player::nextRandomMove() {
	char moveToReturn = 0;
	int newRandomNum = rand() % 3;
	switch (newRandomNum) {
	case 0:
		moveToReturn = 'R';
		break;
	case 1:
		moveToReturn = 'P';
		break;
	case 2:
		moveToReturn = 'S';
		break;
	}
	return moveToReturn;
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

std::string Player::getEntireHistoryStr(){
	std::ostringstream oss;
	std::list<char>::const_iterator iterator;
	for (iterator = history.begin(); iterator != history.end(); ++iterator) {
		oss << " " << *iterator;
	}
	oss << std::endl;

	for (iterator = moveHistory.begin(); iterator != moveHistory.end(); ++iterator) {
		oss << " " << *iterator;
	}
	oss << std::endl;
	return oss.str();
}

std::string Player::getCurrentHistoryStr(bool flipStr){
	std::ostringstream oss;
	if(flipStr){
		oss << moveHistory.front() << " " << history.front() << " " << resultHistory.front();
	} else {
		oss << resultHistory.front() << " " << history.front() << " " << moveHistory.front();
	}
	return oss.str();
}

void Player::printHistory(){
	std::list<char>::const_iterator iterator;
	std::cout << name << " history: ";
	for (iterator = history.begin(); iterator != history.end(); ++iterator) {
	    std::cout << " " << *iterator;
	}
	std::cout << std::endl;
}

std::list<Rule*> Player::getRules(){
	return rules;
}

std::list<Adapter*> Player::getAdapters(){
	return adapters;
}

void Player::setName(std::string name){
	this->name = name;
}

std::string Player::getName(){
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
