/*
 * Rule.h
 *
 *  Created on: Nov 14, 2015
 *      Author: mohiul
 */

#ifndef RULE_H_
#define RULE_H_

#include <iostream>

class Rule {
public:
	std::string expr;
	char outcome;
	Rule(std::string expr, char outcome);
	virtual ~Rule();
protected:
};

#endif /* RULE_H_ */
