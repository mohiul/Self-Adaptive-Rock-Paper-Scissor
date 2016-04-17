/*
 * Player.cpp
 *
 *  Created on: Apr 3, 2016
 *      Author: mohiul
 */

#include <sstream>
#include <algorithm>

#include "Player.h"
#include "SelfAdaptiveRPS.h"

Player::Player() {
	bestRuleEngine = NULL;
	this->name = name;
	winCount = 0;
	looseCount = 0;
	drawCount = 0;
	evolution = new Evolution();
	fitness = 0;
}

Player::~Player() {
	for (list<RuleEngine*>::const_iterator iterator = ruleEngines.begin();
			iterator != ruleEngines.end();
			++iterator) {
//		(*iterator)->delRulesAdapters();
		(*iterator)->~RuleEngine();
	}
	evolution->~Evolution();
	history.clear();
}

bool compare_ruleEngine (RuleEngine* first, RuleEngine* second)
{
//	cout << first->getFitness() << " ";
  return ( first->getFitness() > second->getFitness() );
}

void Player::evolve() {
	list<RuleEngine*>::const_iterator iterator;
	list<RuleEngine*> parentRuleEngines;
	ruleEngines.sort(compare_ruleEngine);
	int i = 0;
	int ruleEngineSize = ruleEngines.size();
	int parentPoolSize = ruleEngineSize*parentSelection;
	if(parentPoolSize == 0){
		cerr << "ParentPoolSize should be greater than zero! "<< endl;
	}
	for (iterator = ruleEngines.begin(); iterator != ruleEngines.end(); ++iterator, i++) {
		if(i < parentPoolSize){
			parentRuleEngines.push_back(*iterator);
		} else {
//			(*iterator)->delRulesAdapters();
			(*iterator)->~RuleEngine();
		}
	}
	if(ruleEngines.size() > 1){
		list<RuleEngine*> childrenList = evolution->evolve(parentRuleEngines);
		ruleEngines.clear();
		parentRuleEngines.sort(compare_ruleEngine);
		int eliteParentPoolSize = ruleEngineSize*parentPoolWithReplacement;
//		cout << "eliteParentPoolSize: " << eliteParentPoolSize << endl;
		for (iterator = parentRuleEngines.begin(), i = 0; iterator != parentRuleEngines.end() && i < eliteParentPoolSize; ++iterator, ++i) {
			if(i < eliteParentPoolSize){
				ruleEngines.push_back(*iterator);
			} else {
				(*iterator)->~RuleEngine();
			}

		}
//		cout << "childrenList.size(): " << childrenList.size() << endl;
		for (iterator = childrenList.begin(); iterator != childrenList.end(); ++iterator) {
			ruleEngines.push_back(*iterator);
		}
	}
}

char Player::nextMove() {
	bestRuleEngine = getBestRuleEngine();
	list<RuleEngine*>::const_iterator iterator;
	char nextMove = 0;
	for (iterator = ruleEngines.begin(); iterator != ruleEngines.end(); ++iterator) {
		 char ch = (*iterator)->nextMove();
		if((*iterator)->getId() == bestRuleEngine->getId()){
//			cout << name << ": bestRuleEngine->getId(): " << bestRuleEngine->getId() << endl;
			nextMove = ch;
		}
	}
	return nextMove;
}

RuleEngine* Player::getBestRuleEngine(){

	ruleEngines.sort(compare_ruleEngine);

	list<RuleEngine*> bestFitnessEngines;
	float bestFitness = ruleEngines.front()->getFitness();
//	cout << " bestFitness: " << bestFitness << endl;
	list<RuleEngine*>::const_iterator iterator;
	for (iterator = ruleEngines.begin(); iterator != ruleEngines.end(); ++iterator) {
		RuleEngine* engine = *iterator;
		if(engine->getFitness() == bestFitness){
			bestFitnessEngines.push_back(engine);
		}
	}
	int randomFitnessEngine = 0;
	if(bestFitnessEngines.size() > 1){
		randomFitnessEngine = rand() % bestFitnessEngines.size();
	}
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

	moveHistory.push_back(ownMove);
	char gameResult = 0;
	if(ownMove == 'R' && opponentMove == 'R'){
		gameResult = '3';
		drawCount++;
		resultHistory.push_back('D');
	} else if(ownMove == 'R' && opponentMove == 'P'){
		gameResult = '0';
		looseCount++;
		resultHistory.push_back('L');
	} else if(ownMove == 'R' && opponentMove == 'S'){
		gameResult = '8';
		winCount++;
		resultHistory.push_back('W');
	} else if(ownMove == 'P' && opponentMove == 'R'){
		gameResult = '6';
		winCount++;
		resultHistory.push_back('W');
	} else if(ownMove == 'P' && opponentMove == 'P'){
		gameResult = '4';
		drawCount++;
		resultHistory.push_back('D');
	} else if(ownMove == 'P' && opponentMove == 'S'){
		gameResult = '1';
		looseCount++;
		resultHistory.push_back('L');
	} else if(ownMove == 'S' && opponentMove == 'R'){
		gameResult = '2';
		looseCount++;
		resultHistory.push_back('L');
	} else if(ownMove == 'S' && opponentMove == 'P'){
		gameResult = '7';
		winCount++;
		resultHistory.push_back('W');
	} else if(ownMove == 'S' && opponentMove == 'S'){
		gameResult = '5';
		drawCount++;
		resultHistory.push_back('D');
	}
//	cout << "Player: " << name << " Adding history: " << gameResult << endl;
	history.push_back(gameResult);
	calculateFitness();
}

float Player::calculateFitness() {
	char result = resultHistory.back();
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

float Player::getFitness(){
	return fitness;
}

float Player::getAvgFitness(){
	float totalFitness = 0;
	list<RuleEngine*>::const_iterator iterator;
	for (iterator = ruleEngines.begin(); iterator != ruleEngines.end(); ++iterator) {
		totalFitness += (*iterator)->getFitness();
	}
	return totalFitness/ruleEngines.size();
}

list<Rule*> Player::getRules(){
	return *(bestRuleEngine->getRules());
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

/*
int main() {
	Player* player1 = new Player();

	player1->addHistory('R', 'S');
	cout << "P1 Fitness: " << player1->getFitness() << endl;
	player1->addHistory('R', 'S');
	cout << "P1 Fitness: " << player1->getFitness() << endl;
	player1->addHistory('R', 'S');
	cout << "P1 Fitness: " << player1->getFitness() << endl;
	player1->addHistory('R', 'S');
	cout << "P1 Fitness: " << player1->getFitness() << endl;
	player1->addHistory('R', 'S');
	cout << "P1 Fitness: " << player1->getFitness() << endl;
	player1->addHistory('R', 'S');
	cout << "P1 Fitness: " << player1->getFitness() << endl;
	player1->addHistory('S', 'R');
	cout << "P1 Fitness: " << player1->getFitness() << endl;
	player1->addHistory('S', 'R');
	cout << "P1 Fitness: " << player1->getFitness() << endl;
	player1->addHistory('S', 'R');
	cout << "P1 Fitness: " << player1->getFitness() << endl;
	player1->addHistory('S', 'R');
	cout << "P1 Fitness: " << player1->getFitness() << endl;

	Player* player2 = new Player();
	player2->addHistory('S', 'R');
	cout << "P2 Fitness: " << player2->getFitness() << endl;
	player2->addHistory('S', 'R');
	cout << "P2 Fitness: " << player2->getFitness() << endl;
	player2->addHistory('S', 'R');
	cout << "P2 Fitness: " << player2->getFitness() << endl;
	player2->addHistory('S', 'R');
	cout << "P2 Fitness: " << player2->getFitness() << endl;
	player2->addHistory('S', 'R');
	cout << "P2 Fitness: " << player2->getFitness() << endl;
	player2->addHistory('S', 'R');
	cout << "P2 Fitness: " << player2->getFitness() << endl;
	player2->addHistory('R', 'S');
	cout << "P2 Fitness: " << player2->getFitness() << endl;
	player2->addHistory('R', 'S');
	cout << "P2 Fitness: " << player2->getFitness() << endl;
	player2->addHistory('R', 'S');
	cout << "P2 Fitness: " << player2->getFitness() << endl;
	player2->addHistory('R', 'S');
	cout << "P2 Fitness: " << player2->getFitness() << endl;

}
*/
