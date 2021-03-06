/*
 * RuleParser.cpp
 *
 *  Created on: Nov 14, 2015
 *      Author: mohiul
 */

#include <cstdlib>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>

#include "RuleParser.h"
#include "RPSGame.h"
#include "Utils.h"

RuleParser::RuleParser(RuleEngine *player) {
	this->ruleEngine = player;

}

RuleParser::~RuleParser() {
	// TODO Auto-generated destructor stub
}

list<Rule*> RuleParser::matchRules(list<char> history) {
	list<Rule*> matchedRules;
	if (history.size() > 0) {
		list<Rule*>::const_iterator iterator;
//		cout << "No of rules: " << ruleEngine->rules.size() << endl;
//		cout << "RuleParser::matchRules historySize: " << history.size() << endl;
//		Utils::printHistory("RuleParser::matchRules " + ruleEngine->player->getName(), history);
		for (iterator = ruleEngine->rules.begin(); iterator != ruleEngine->rules.end(); ++iterator) {
			Rule* rule = *iterator;
			string expr = rule->getCondition();
//			cout << "expr: " << expr << endl;
			bool match = false;
			unsigned int i = 0;
			list<char>::const_iterator historyItr;
			for (historyItr = history.begin();
					historyItr != history.end()
							&& i < expr.length();
					historyItr++, i++) {
				char historyVal = *historyItr;
				char expr_ch = expr.at(i);
//				cout << "historyVal: " << historyVal << " expr_ch: " << expr_ch << endl;
				if (expr_ch == '?') {
					i++;
					int nextNo = (int) (toDigit(expr.at(i))) - 1;
					if(nextNo < 0) historyItr--;// For ?0 scenarios
					while (nextNo > 0 && historyItr != history.end()) {
						historyItr++;
						nextNo--;
					}
					if (nextNo <= 0 && historyItr != history.end()) {
						match = true;
						continue;
					}
				} else if (expr_ch == '<') {
					int nextNo = (int) (toDigit(expr.at(++i)));
					int historyNo = (int) (toDigit(historyVal));
					if (historyNo < nextNo) {
						match = true;
						continue;
					} else {
						match = false;
						break;
					}
				} else if (expr_ch == '>') {
					int nextNo = (int) (toDigit(expr.at(++i)));
					int historyNo = (int) (toDigit(historyVal));
					if (historyNo > nextNo) {
						match = true;
						continue;
					} else {
						match = false;
						break;
					}
				} else if (historyVal != expr_ch) {
					match = false;
					break;
				} else if (historyVal == expr_ch) {
					match = true;
				}
			}
			if (i < expr.length() && historyItr == history.end()) {
				match = false;
			} else if(i == expr.length() && historyItr != history.end()) {
				match = false;
			}

			if (match) {
				matchedRules.push_back(rule);
//				cout << ruleEngine->player->getName() << "Rule matched: " << rule->getString() << endl;
			}
		}
	}
	return matchedRules;
}

struct CompareRuleSpecificity {
    bool operator()(Rule * lhs, Rule * rhs) {return lhs->getSpecificity() > rhs->getSpecificity();}
};

struct CompareRuleHowRecent {
    bool operator()(Rule * lhs, Rule * rhs) {return lhs->getHowRecent() > rhs->getHowRecent();}
};

char RuleParser::nextMove() {
	char moveToReturn = 0;
	const list<char> historyList = ruleEngine->player->history;
	int historyListSize = historyList.size();
	list<Rule*> matchedRules;
	for(int i = 0; i < historyListSize; i++){
//		cout << "historyListSize: " << historyListSize << endl;
		list<char> historySubList;
		list<char>::const_iterator listItr = historyList.begin();
		for(int k = 0; k < i; k++) listItr++;
		for(; listItr != historyList.end(); listItr++){
			historySubList.push_back(*listItr);
		}
		list<Rule*> newMatchedRules = matchRules(historySubList);
		list<Rule*>::const_iterator iterator;
		for (iterator = newMatchedRules.begin(); iterator != newMatchedRules.end(); ++iterator) {
			Rule *newMatchedRule = *iterator;
			if(find(matchedRules.begin(), matchedRules.end(), newMatchedRule) == matchedRules.end()){
//				cout << "not found: " << endl;
				newMatchedRule->setHowRecent(-i);
				matchedRules.push_back(newMatchedRule);
			}
		}
	}
	lastMatchingRule = NULL;
	if(matchedRules.size() == 0 && ruleEngPlayByLastMove){
		if(ruleEngine->moveHistory.size() > 0){
			moveToReturn = ruleEngine->moveHistory.back();
		}
	} else if(matchedRules.size() > 0){
//		printRuleList(matchedRules);
		matchedRules.sort(CompareRuleHowRecent());
//		printRuleList(matchedRules);
		matchedRules.sort(CompareRuleSpecificity());
//		printRuleList(matchedRules);
		lastMatchingRule = matchedRules.front();
//		cout << "Using Rule : " << lastMatchingRule->getDetailString() << endl;
		moveToReturn = lastMatchingRule->getAction();
	}
	return moveToReturn;
}

void RuleParser::printRuleList(list<Rule*> ruleList){
	list<Rule*>::const_iterator iterator;
//	cout << ruleEngine->player->getName() << " rule list begin: " << endl;
	cout << " rule list begin: " << endl;
	for (iterator = ruleList.begin(); iterator != ruleList.end(); ++iterator) {
		cout << (*iterator)->getDetailString() << endl;
	}
	cout << " rule list end: " << endl;
//	cout << ruleEngine->player->getName() << " rule list end: " << endl;
}

void RuleParser::calculateFitness(char result){
	if(lastMatchingRule != NULL){
		lastMatchingRule->calculateFitness(result);
	}
}

//int main(){
//	RuleEngine engine(new Player());
//	engine.addRule(new Rule("1>134", 'R'));
//	RuleParser parser(&engine);
//	list<char> historyList;
//	historyList.push_back('1');
//	historyList.push_back('2');
//	historyList.push_back('3');
//	historyList.push_back('4');
//	list<Rule*> matchedRules = parser.matchRules(historyList);
//	return 0;
//}
