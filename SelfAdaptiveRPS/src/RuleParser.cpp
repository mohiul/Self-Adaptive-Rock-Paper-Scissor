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

RuleParser::RuleParser(Player *player) {
	this->player = player;

}

RuleParser::~RuleParser() {
	// TODO Auto-generated destructor stub
}

list<Rule*> RuleParser::matchRules(list<char> history) {
	list<Rule*> matchedRules;
	if (history.size() > 0) {
		list<Rule*>::const_iterator iterator;
		for (iterator = player->rules.begin(); iterator != player->rules.end(); ++iterator) {
			Rule* rule = *iterator;
			string expr = rule->getCondition();
			//cout << "expr: " << expr << endl;
			bool match = false;
			unsigned int i;
			list<char>::const_iterator historyItr;
			for (i = 0, historyItr = history.begin();
					i < expr.length() && historyItr != history.end();
					i++, historyItr++) {
				char historyVal = *historyItr;
				char expr_ch = expr.at(i);
				//cout << "historyVal: " << historyVal << " expr_ch: " << expr_ch << endl;
				if (expr_ch == '?') {
					int nextNo = (int) (toDigit(expr.at(++i))) - 1;
					//cout << "ignoreNo: " << nextNo << endl;
					while (nextNo > 0 && historyItr != history.end()) {
						historyItr++;
						nextNo--;
					}
					if (nextNo <= 0 && historyItr != history.end()) {
						continue;
					} else {
						break;
					}
				} else if (expr_ch == '<') {
					int nextNo = (int) (toDigit(expr.at(++i)));
					int historyNo = (int) (toDigit(historyVal));
					if (historyNo < nextNo) {
						continue;
					}
				} else if (expr_ch == '>') {
					int nextNo = (int) (toDigit(expr.at(++i)));
					int historyNo = (int) (toDigit(historyVal));
					if (historyNo > nextNo) {
						continue;
					}
				} else if (historyVal != expr_ch) {
					break;
				}
			}
			if (i == expr.length())
				match = true;

			if (match) {
				matchedRules.push_back(rule);
				cout << "Rule matched: " << rule->getString() << endl;
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
	string historyStr(Utils::convertListToArray(player->history));
	int strLength = historyStr.length();
	list<Rule*> matchedRules;
	for(int i = 0; i < strLength; i++){
		list<char> historySubList = Utils::convertStrToList(historyStr.substr(i, strLength));
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
	if(matchedRules.size() == 0){
		moveToReturn = nextRandomMove();
	} else {
//		player->printRuleList(matchedRules);
		matchedRules.sort(CompareRuleHowRecent());
//		player->printRuleList(matchedRules);
		matchedRules.sort(CompareRuleSpecificity());
//		player->printRuleList(matchedRules);
		Rule *rule = matchedRules.front();
		cout << "Using Rule : " << rule->getDetailString() << endl;
		moveToReturn = rule->getAction();
	}
	return moveToReturn;
}

char RuleParser::nextRandomMove() {
	char moveToReturn = 0;
	int newRandomNum = rand() % 3;
	switch (newRandomNum) {
	case 0:
		moveToReturn = 'R';
		break;
	case 1:
		moveToReturn = 'P';
		break;
	case 2:
		moveToReturn = 'S';
		break;
	}
	return moveToReturn;
}

void RuleParser::printRuleList(list<Rule*> ruleList){
	list<Rule*>::const_iterator iterator;
	cout << player->name << " rule list begin: " << endl;
	for (iterator = ruleList.begin(); iterator != ruleList.end(); ++iterator) {
		cout << (*iterator)->getDetailString() << endl;
	}
	cout << player->name << " rule list end: " << endl;
}
