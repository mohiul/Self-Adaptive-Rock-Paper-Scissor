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

Player::Player(string name) {
	this->name = name;
	winCount = 0;
	looseCount = 0;
	drawCount = 0;
}

Player::~Player() {
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

list<Rule*> Player::matchRules(list<char> history) {
	list<Rule*> matchedRules;
	if (history.size() > 0) {
		list<Rule*>::const_iterator iterator;
		for (iterator = rules.begin(); iterator != rules.end(); ++iterator) {
			Rule* rule = *iterator;
			string expr = rule->getCondition();
			//cout << "expr: " << expr << endl;
			bool match = false;
			unsigned int i;
			list<char>::const_iterator historyItr;
			for (i = 0, historyItr = history.begin();
					i < expr.length() && historyItr != history.end();
					i++, historyItr++) {
				char historyVal = *historyItr;
				char expr_ch = expr.at(i);
				//cout << "historyVal: " << historyVal << " expr_ch: " << expr_ch << endl;
				if (expr_ch == '?') {
					int nextNo = (int) (toDigit(expr.at(++i))) - 1;
					//cout << "ignoreNo: " << nextNo << endl;
					while (nextNo > 0 && historyItr != history.end()) {
						historyItr++;
						nextNo--;
					}
					if (nextNo <= 0 && historyItr != history.end()) {
						continue;
					} else {
						break;
					}
				} else if (expr_ch == '<') {
					int nextNo = (int) (toDigit(expr.at(++i)));
					int historyNo = (int) (toDigit(historyVal));
					if (historyNo < nextNo) {
						continue;
					}
				} else if (expr_ch == '>') {
					int nextNo = (int) (toDigit(expr.at(++i)));
					int historyNo = (int) (toDigit(historyVal));
					if (historyNo > nextNo) {
						continue;
					}
				} else if (historyVal != expr_ch) {
					break;
				}
			}
			if (i == expr.length())
				match = true;

			if (match) {
				matchedRules.push_back(rule);
				cout << "Rule matched: " << rule->getString() << endl;
			}
		}
	}
	return matchedRules;
}

struct CompareRuleSpecificity {
    bool operator()(Rule * lhs, Rule * rhs) {return lhs->getSpecificity() > rhs->getSpecificity();}
};

char Player::nextMove() {
	char moveToReturn = 0;
	list<Rule*> matchedRules = matchRules(history);
	if(matchedRules.size() == 0){
		moveToReturn = nextRandomMove();
	} else {
		matchedRules.sort(CompareRuleSpecificity());
		Rule *rule = matchedRules.front();
		cout << "Using Rule : " << rule->getString() << endl;
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
	Rule *r1 = new Rule("012345?48", 'R');
	Rule *r2 = new Rule("0>0<83", 'P');
	Rule *r3 = new Rule("0?34", 'P');
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
