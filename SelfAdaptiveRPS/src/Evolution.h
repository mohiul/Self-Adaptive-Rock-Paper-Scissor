/*
 * Evolution.h
 *
 *  Created on: Apr 3, 2016
 *      Author: mohiul
 */

#ifndef EVOLUTION_H_
#define EVOLUTION_H_

#include "RuleEngine.h"

class RuleEngine;

class Evolution {
public:
	Evolution();
	virtual ~Evolution();
	list<RuleEngine*> evolve(list<RuleEngine*> parentList);
	pair<RuleEngine*, RuleEngine*> crossover(RuleEngine* engine1, RuleEngine* engine2);
	void mutate(RuleEngine* engine);
protected:

};

#endif /* EVOLUTION_H_ */
