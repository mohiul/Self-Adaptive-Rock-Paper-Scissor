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
#include "Player.h"
#include "SelfAdaptiveRPS.h"

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

struct CompareRuleSpecificity {
    bool operator()(Rule * lhs, Rule * rhs) {return lhs->getSpecificity() > rhs->getSpecificity();}
};

char Player::nextMove() {
	char moveToReturn = 0;
	std::list<Rule*> matchedRules;
	if(history.size() > 0){
		std::list<Rule*>::const_iterator iterator;
		for (iterator = rules.begin(); iterator != rules.end(); ++iterator) {
		    Rule* rule = *iterator;
		    std::string expr = rule->getCondition();
//		    std::cout << "expr: " << expr << std::endl;
		    bool match = false;
		    unsigned int i; std::list<char>::const_iterator historyItr;
		    for(i = 0, historyItr = history.begin();
		    		i < expr.length() && historyItr != history.end();
		    		i++, historyItr++){
		    	char historyVal = *historyItr;
		    	char expr_ch = expr.at(i);
//	    		std::cout << "historyVal: " << historyVal << " expr_ch: " << expr_ch << std::endl;
		    	if(expr_ch == '?'){
		    		int nextNo = (int)toDigit(expr.at(++i)) - 1;
//		    		std::cout << "ignoreNo: " << nextNo << std::endl;
		    		while(nextNo > 0 && historyItr != history.end()){
		    			historyItr++;
		    			nextNo--;
		    		}
		    		if(nextNo <= 0 && historyItr != history.end()){
		    			continue;
		    		} else {
		    			break;
		    		}
		    	}else if(expr_ch == '<'){
		    		int nextNo = (int)toDigit(expr.at(++i));
		    		int historyNo = (int)toDigit(historyVal);
		    		if(historyNo < nextNo){
		    			continue;
		    		}
		    	}else if(expr_ch == '>'){
		    		int nextNo = (int)toDigit(expr.at(++i));
		    		int historyNo = (int)toDigit(historyVal);
		    		if(historyNo > nextNo){
		    			continue;
		    		}
		    	}else if(historyVal != expr_ch) {
					break;
		    	}
		    }
		    if(i == expr.length()) match = true;
		    if(match){
		    	matchedRules.push_back(rule);
		    	std::cout << "Rule matched: " << rule->getString() << std::endl;
		    }
		}
	}
	if(matchedRules.size() == 0){
		moveToReturn = nextRandomMove();
	} else {
		matchedRules.sort(CompareRuleSpecificity());
		Rule *rule = matchedRules.front();
		std::cout << "Using Rule : " << rule->getString() << std::endl;
		moveToReturn = rule->getAction();
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

/*
int main(int argc, char** argv)
{
	Player *p = new Player("P");
	Rule *r1 = new Rule("012345?48", 'R');
	Rule *r2 = new Rule("0>0<83", 'P');
	Rule *r3 = new Rule("0?34", 'P');
	std::cout << "r1 specificity: " << r1->getSpecificity() << std::endl;
	std::cout << "r2 specificity: " << r2->getSpecificity() << std::endl;
	std::cout << "r3 specificity: " << r3->getSpecificity() << std::endl;
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
	std::cout << "next move: " << p->nextMove();
	return 0;
}
*/
