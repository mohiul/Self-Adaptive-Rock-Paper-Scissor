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

#include "Rule.h"
#include "Adapter.h"
#include "RuleParser.h"

using namespace std;
class RuleParser;

class RuleEngine {
	friend class RuleParser;
public:
	RuleEngine(string name);
	virtual ~RuleEngine();
	char nextMove();
	void addHistory(char ownMove, char opponentMove);
	void addRule(Rule* rule);
	void addAdapter(Adapter* adapter);
	void adapt();
	void selfAdapt();
	void printHistory();
	void printHistory(list<char> history);
	string getEntireHistoryStr();
	string getCurrentHistoryStr(bool flipStr);
	string getName();
	void setName(string name);
	list<Rule*> getRules();
	list<Adapter*> getAdapters();
	int getWin();
	int getLoose();
	int getDraw();
protected:
	RuleParser *parser;
	string name;
	int winCount;
	int looseCount;
	int drawCount;
	list<char> resultHistory;
	list<char> moveHistory;
	list<char> history;
	list<Rule*> rules;
	list<Adapter*> adapters;

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
