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
	RuleEngine *ruleEngine;
	Rule* lastMatchingRule;
	list<Rule*> matchRules(list<char> history);
public:
	RuleParser(RuleEngine *player);
	virtual ~RuleParser();
	char nextMove();
	char nextRandomMove();
	static void printRuleList(list<Rule*> ruleList);
	void calculateFitness(char result);
};

#endif /* RULEPARSER_H_ */
