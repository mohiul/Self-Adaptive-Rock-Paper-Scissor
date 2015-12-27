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

class Rule {
protected:
	std::string condition;
	char action;
	int specifcity;
	int howRecent;
public:
	Rule(std::string condition, char action);
	virtual ~Rule();
	std::string getCondition();
	char getAction();
	std::string getString();
	std::string getDetailString();
	int getSpecificity();
	void setHowRecent(int howRecent);
	int getHowRecent();
protected:
};

#endif /* RULE_H_ */
