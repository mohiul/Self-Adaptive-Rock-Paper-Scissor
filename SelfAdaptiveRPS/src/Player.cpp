/*
 * Player.cpp
 *
 *  Created on: Apr 3, 2016
 *      Author: mohiul
 */

#include <sstream>
#include <algorithm>

#include "Player.h"

Player::Player() {
	bestRuleEngine = NULL;
	this->name = name;
	winCount = 0;
	looseCount = 0;
	drawCount = 0;
	evolution = new Evolution();
}

Player::~Player() {
	history.clear();
}

void Player::evolve() {
	if(ruleEngines.size() > 1){
		list<RuleEngine*> childrenList = evolution->evolve(ruleEngines);
		ruleEngines.clear();
		list<RuleEngine*>::const_iterator iterator;
		for (iterator = childrenList.begin(); iterator != childrenList.end(); ++iterator) {
			ruleEngines.push_back(*iterator);
		}
	}
}

char Player::nextMove() {
	bestRuleEngine = getBestRuleEngine();
	return bestRuleEngine->nextMove();
}

bool compare_ruleEngine (RuleEngine* first, RuleEngine* second)
{
  return ( first->getFitness() > second->getFitness() );
}

RuleEngine* Player::getBestRuleEngine(){

	ruleEngines.sort(compare_ruleEngine);

	list<RuleEngine*> bestFitnessEngines;
	float bestFitness = ruleEngines.front()->getFitness();
//	cout << name << ": current best fitness: " << bestFitness << endl;
	list<RuleEngine*>::const_iterator iterator;
	for (iterator = ruleEngines.begin(); iterator != ruleEngines.end(); ++iterator) {
		RuleEngine* engine = *iterator;
//		cout << name << ": engine->getFitness(): " << engine->getFitness() << endl;
		if(engine->getFitness() == bestFitness){
			bestFitnessEngines.push_back(engine);
		}
	}
	int randomFitnessEngine = 0;
	if(bestFitnessEngines.size() > 1){
		randomFitnessEngine = rand() % bestFitnessEngines.size();
	}
//	cout << name << ": randomFitnessEngine: " << randomFitnessEngine << endl;
	iterator = bestFitnessEngines.begin();
	advance(iterator, randomFitnessEngine);
	return *iterator;
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

const list<char>& Player::getHistory() const {
	return history;
}

void Player::addRuleEngine(RuleEngine* engine) {
	return ruleEngines.push_back(engine);
}

void Player::addHistory(char ownMove, char opponentMove){
	//Update rule engine history
	list<RuleEngine*>::const_iterator iterator;
	for (iterator = ruleEngines.begin(); iterator != ruleEngines.end(); ++iterator) {
		RuleEngine* engine = *iterator;
		engine->addHistory(opponentMove);
	}

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

list<Rule*> Player::getRules(){
	return bestRuleEngine->getRules();
}

list<Adapter*> Player::getAdapters(){
	return bestRuleEngine->getAdapters();
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
