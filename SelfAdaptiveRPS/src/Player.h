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

using namespace std;

class Player {
public:
	Player(string name);
	virtual ~Player();
	char nextMove();
	char nextRandomMove();
	void addHistory(char ownMove, char opponentMove);
	void addRule(Rule* rule);
	void addAdapter(Adapter* adapter);
	void printHistory();
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
	string name;
	int winCount;
	int looseCount;
	int drawCount;
	list<char> resultHistory;
	list<char> moveHistory;
	list<char> history;
	list<Rule*> rules;
	list<Adapter*> adapters;
	list<Rule*> matchRules(list<char> history);
private:

};

#endif /* PLAYER_H_ */
