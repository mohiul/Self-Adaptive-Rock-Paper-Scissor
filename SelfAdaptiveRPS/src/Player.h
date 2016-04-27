/*
 * Player.h
 *
 *  Created on: Apr 3, 2016
 *      Author: mohiul
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <iostream>

#include "RuleEngine.h"
#include "Rule.h"
#include "Adapter.h"
#include "Evolution.h"

using namespace std;
class RuleEngine;
class Evolution;

class Player {
protected:
	string name;
	int winCount;
	int looseCount;
	int drawCount;
	list<char> resultHistory;
	list<RuleEngine*> ruleEngines;
	Evolution* evolution;
	float fitness;
	float calculateFitness();
public:
	RuleEngine* bestRuleEngine;
	list<char> history;
	list<char> moveHistory;
	Player();
	virtual ~Player();
	string getName();
	void setName(string name);
	int getWin();
	int getLoose();
	int getDraw();
	void addHistory(char ownMove, char opponentMove);
	void printHistory(int gameIteration);
	void printHistory(list<char> history);
	string getEntireHistoryStr();
	string getCurrentHistoryStr(bool flipStr);
	list<Rule*> getRules();
	list<Adapter*> getAdapters();
	char nextMove();
	void addRuleEngine(RuleEngine* engine);
	RuleEngine* getBestRuleEngine(list<RuleEngine*> ruleEngines);
	void evolve();
	float getFitness();
	float getAvgFitness();
	void adapt();
};

#endif /* PLAYER_H_ */
