/*
 * Adaptation.h
 *
 *  Created on: Dec 25, 2015
 *      Author: mohiul
 */

#ifndef ADAPTER_H_
#define ADAPTER_H_

#include <list>
#include "Rule.h"

enum Action
{
	ADD,
	DEL,
	MOD,
	SHF
};

class Adapter {
private:
protected:
	Rule *rule;
	std::list<Action> actions;
public:
	Adapter(Rule *rule);
	virtual ~Adapter();
	void addAction(Action action);
	std::string getString();
};

#endif /* ADAPTER_H_ */
