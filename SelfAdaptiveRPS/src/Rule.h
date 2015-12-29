/*
 * Rule.h
 *
 *  Created on: Nov 14, 2015
 *      Author: mohiul
 */

#ifndef RULE_H_
#define RULE_H_

#include <iostream>
#include <string>
#include <set>

enum RuleComparison{
	GENERAL,
	SPECIFIC,
	EQUAL,
	NOT_EQUAL
};

using namespace std;

class Rule {
protected:
	string condition;
	char action;
	int specifcity;
	int howRecent;
public:
	Rule(string condition, char action);
	virtual ~Rule();
	string getCondition();
	char getAction();
	string getString();
	string getDetailString();
	int getSpecificity();
	void setHowRecent(int howRecent);
	int getHowRecent();
	RuleComparison compare(Rule *rule);
protected:
};

#endif /* RULE_H_ */
