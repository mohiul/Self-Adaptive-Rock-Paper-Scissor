/*
 * Player.h
 *
 *  Created on: Nov 14, 2015
 *      Author: mohiul
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <iostream>
#include <list>

#include "Rule.h"
#include "Adapter.h"
#include "RuleParser.h"

using namespace std;
class RuleParser;

class Player {
	friend class RuleParser;
public:
	Player(string name);
	virtual ~Player();
	char nextMove();
	void addHistory(char ownMove, char opponentMove);
	void addRule(Rule* rule);
	void addAdapter(Adapter* adapter);
	void adapt();
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
	void actionAdd(Rule *adapterRule);
	void actionDelete(Rule *adapterRule);
	void actionModify(Rule *adapterRule);
	void actionShift(Rule *adapterRule);
private:

};

#endif /* PLAYER_H_ */
