/*
 * Rule.cpp
 *
 *  Created on: Nov 14, 2015
 *      Author: mohiul
 */

#include "Rule.h"

Rule::Rule(std::string expr, char outcome) {
	this->expr = expr;
	this->outcome = outcome;

}

Rule::~Rule() {
	// TODO Auto-generated destructor stub
}

