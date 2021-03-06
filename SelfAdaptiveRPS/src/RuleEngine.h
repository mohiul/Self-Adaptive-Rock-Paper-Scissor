/*
 * Player.h
 *
 *  Created on: Nov 14, 2015
 *      Author: mohiul
 */

#ifndef RULEENGINE_H_
#define RULEENGINE_H_

#include <iostream>
#include <list>

#include "Player.h"
#include "Rule.h"
#include "Adapter.h"
#include "RuleParser.h"

using namespace std;
class RuleParser;
class Player;

class RuleEngine {
	friend class RuleParser;
public:
	static int totalNoOfRuleEngine;
	RuleEngine(Player* player);
	virtual ~RuleEngine();
	void delRulesAdapters();
	char nextMove();
	void addRule(Rule* rule);
	void addAdapter(Adapter* adapter);
	void adapt();
	void selfAdapt();
	list<Rule*> *getRules();
	list<Adapter*> getAdapters();
	void addHistory(char opponentMove);
	list<char> getResultHistory();
	list<char> getMoveHistory();
	float calculateFitness();
	float getFitness();
	Player* getPlayer();
	int getId();
	void setFitness(int newFitness);

protected:
	Player* player;
	RuleParser *parser;
	list<Rule*> rules;
	list<Adapter*> adapters;
	list<char> resultHistory;
	list<char> moveHistory;
	float fitness;
	int id;

	void actionAddRule(Rule *adapterRule);
	void actionDeleteRule(Rule *adapterRule);
	void actionModifyRule(Rule *adapterRule);
	void actionShiftRule(Rule *adapterRule);

	void actionAddAdapter(Adapter *adapter);
	void actionDeleteAdapter(Adapter *adapter);
	void actionModifyAdapter(Adapter *adapter);
	void actionShiftAdapter(Adapter *adapter);

private:

};

#endif /* RULEENGINE_H_ */
