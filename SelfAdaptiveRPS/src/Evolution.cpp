/*
 * Evolution.cpp
 *
 *  Created on: Apr 3, 2016
 *      Author: mohiul
 */
#include <utility>
#include <sstream>
#include <cstdlib>
#include <list>

#include "Evolution.h"
#include "Rule.h"

Evolution::Evolution() {
	// TODO Auto-generated constructor stub

}

Evolution::~Evolution() {
	// TODO Auto-generated destructor stub
}

list<RuleEngine*> Evolution::evolve(list<RuleEngine*> parentList) {
	list<RuleEngine*> childrenList;
	list<RuleEngine*>::const_iterator iterator;
	for (iterator = parentList.begin(); iterator != parentList.end(); ++iterator) {
		RuleEngine* engine1 = *iterator;
		++iterator;
		if(iterator != parentList.end()){
			RuleEngine* engine2 = *iterator;
			pair<RuleEngine*, RuleEngine*> childrenPair = crossover(engine1, engine2);
			childrenList.push_back(mutate(childrenPair.first));
			childrenList.push_back(mutate(childrenPair.second));
		}
	}
	return childrenList;
}

pair<RuleEngine*, RuleEngine*> Evolution::crossover(RuleEngine* engine1, RuleEngine* engine2) {
	RuleEngine* child1 = new RuleEngine(engine1->getPlayer());
	RuleEngine* child2 = new RuleEngine(engine1->getPlayer());
	list<Rule*> ruleList1 = *(engine1->getRules());
	list<Rule*> ruleList2 = *(engine1->getRules());
//	int rand1 = rand() % (ruleList1.size() - 1) + 1;
//	int rand2 = rand() % (ruleList2.size() - 1) + 1;
	int rand1 = ruleList1.size()/2;
	int rand2 = ruleList2.size()/2;
	list<Rule*>::const_iterator iterator;
	int i = 0;
	for (iterator = ruleList1.begin(); iterator != ruleList1.end(); ++iterator, i++) {
		if(i < rand1){
			child1->addRule(*iterator);
		} else {
			child2->addRule(*iterator);
		}
	}
	i = 0;
	for (iterator = ruleList2.begin(); iterator != ruleList2.end(); ++iterator, i++) {
		if(i < rand2){
			child2->addRule(*iterator);
		} else {
			child1->addRule(*iterator);
		}
	}
	return make_pair(child1, child2);
}

RuleEngine* Evolution::mutate(RuleEngine* engine) {
	Rule* ruleToInsert;
	list<Rule*>* ruleList = engine->getRules();
	int listSize = ruleList->size();
	int randNum = rand() % ruleList->size();
	list<Rule*>::iterator iterator = ruleList->begin();
	advance(iterator, randNum);

	if(listSize > 1){
		randNum = rand() % 3;
	} else {
		randNum = rand() % 2;
	}
	switch(randNum) {
	case 0://ADD
//		cout << "Adding rule..." << endl;
		ruleToInsert = Rule::generateRule();
		ruleList->insert(iterator, ruleToInsert);
		break;
	case 1://MODIFY
//		cout << "Modifying rule..." << endl;
		(*iterator)->mutate();
		break;
	case 2://DELETE
//		cout << "Deleting rule..." << endl;
		ruleList->remove(*iterator);
		break;
	}

	return engine;
}
