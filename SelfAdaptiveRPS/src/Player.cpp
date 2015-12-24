/*
 * Player.cpp
 *
 *  Created on: Nov 14, 2015
 *      Author: mohiul
 */

#include <cstdlib>

#include "Player.h"

Player::Player(std::string name) {
	this->name = name;
	winCount = 0;
	looseCount = 0;
	drawCount = 0;
}

Player::~Player() {
	// TODO Auto-generated destructor stub
}

char Player::nextMove() {
	char moveToReturn = 0;
	if(history.size() > 0){
		std::list<Rule*>::const_iterator iterator;
		bool match = false;
		for (iterator = rules.begin(); iterator != rules.end(); ++iterator) {
		    Rule* rule = *iterator;
		    const char *expr = rule->expr.c_str();
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
		    	moveToReturn = rule->outcome;
		    	break;
		    }
		}
	    if(!match){
	    	moveToReturn = nextRandomMove();
	    }
	} else {
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
	char gameResult = 0;
	if(ownMove == 'R' && opponentMove == 'R'){
		gameResult = '3';
		drawCount++;
	} else if(ownMove == 'R' && opponentMove == 'P'){
		gameResult = '0';
		looseCount++;
	} else if(ownMove == 'R' && opponentMove == 'S'){
		gameResult = '8';
		winCount++;
	} else if(ownMove == 'P' && opponentMove == 'R'){
		gameResult = '6';
		winCount++;
	} else if(ownMove == 'P' && opponentMove == 'P'){
		gameResult = '4';
		drawCount++;
	} else if(ownMove == 'P' && opponentMove == 'S'){
		gameResult = '1';
		looseCount++;
	} else if(ownMove == 'S' && opponentMove == 'R'){
		gameResult = '2';
		looseCount++;
	} else if(ownMove == 'S' && opponentMove == 'P'){
		gameResult = '7';
		winCount++;
	} else if(ownMove == 'S' && opponentMove == 'S'){
		gameResult = '5';
		drawCount++;
	}
	history.push_front(gameResult);

}

void Player::addRule(Rule* rule) {
	rules.push_back(rule);
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
