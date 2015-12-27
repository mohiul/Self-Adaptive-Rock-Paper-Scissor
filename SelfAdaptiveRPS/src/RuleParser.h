/*
 * RuleParser.h
 *
 *  Created on: Nov 14, 2015
 *      Author: mohiul
 */

#ifndef RULEPARSER_H_
#define RULEPARSER_H_

#include <list>
#include "Player.h"
#include "Rule.h"

using namespace std;
class Player;

class RuleParser {
protected:
	Player *player;
	list<Rule*> matchRules(list<char> history);
public:
	RuleParser(Player *player);
	virtual ~RuleParser();
	char nextMove();
	char nextRandomMove();
	void printRuleList(list<Rule*> ruleList);
};

#endif /* RULEPARSER_H_ */
