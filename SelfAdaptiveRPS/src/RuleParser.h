/*
 * RuleParser.h
 *
 *  Created on: Nov 14, 2015
 *      Author: mohiul
 */

#ifndef RULEPARSER_H_
#define RULEPARSER_H_

#include <list>
#include "Rule.h"
#include "RuleEngine.h"
#include "Utils.h"

using namespace std;
class RuleEngine;

class RuleParser {
protected:
	RuleEngine *player;
	list<Rule*> matchRules(list<char> history);
public:
	RuleParser(RuleEngine *player);
	virtual ~RuleParser();
	char nextMove();
	char nextRandomMove();
	void printRuleList(list<Rule*> ruleList);
};

#endif /* RULEPARSER_H_ */
