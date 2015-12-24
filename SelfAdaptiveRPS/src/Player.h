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

class Player {
public:
	Player(std::string name);
	virtual ~Player();
	char nextMove();
	char nextRandomMove();
	void addHistory(char ownMove, char opponentMove);
	void addRule(Rule* rule);
	void printHistory();
	std::string getName();
	std::list<Rule*> getRules();
	int getWin();
	int getLoose();
	int getDraw();
protected:
	std::string name;
	int winCount;
	int looseCount;
	int drawCount;
	std::list<char> history;
	std::list<Rule*> rules;
	std::list<char> adapter;
};

#endif /* PLAYER_H_ */
