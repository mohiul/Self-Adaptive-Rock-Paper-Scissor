/*
 * Rule.cpp
 *
 *  Created on: Nov 14, 2015
 *      Author: mohiul
 */

#include <sstream>
#include <list>
#include <utility>
#include "Rule.h"
#include "SelfAdaptiveRPS.h"
#include "Utils.h"

Rule::Rule(string condition, char action) {
	this->condition = condition;
	this->action = action;
	howRecent = 0;
	specifcity = 0;
	for(unsigned int i = 0; i < condition.length(); i++){
		if(condition.at(i) == '?'){
			specifcity -= 9*(int)toDigit(condition.at(++i));
		} else if(condition.at(i) == '<'){
			int nextDigit = (int)toDigit(condition.at(++i));
			while(nextDigit > 0){
				specifcity--;
				nextDigit--;
			}
		} else if(condition.at(i) == '>'){
			int nextDigit = (int)toDigit(condition.at(++i));
			while(nextDigit < 8){
				specifcity--;
				nextDigit++;
			}
		}
	}
}

Rule::~Rule() {

}

string Rule::getCondition(){
	return condition;
}

char Rule::getAction(){
	return action;
}

string Rule::getString() {
	ostringstream oss;
	oss << condition << ":" << action;
	return oss.str();
}

string Rule::getDetailString() {
	ostringstream oss;
	oss << condition << ":" << action << " specificity: " << specifcity << " how recent: " << howRecent;
	return oss.str();
}


int Rule::getSpecificity(){
	return specifcity;
}

void Rule::setHowRecent(int howRecent){
	this->howRecent = howRecent;
}

int Rule::getHowRecent(){
	return howRecent;
}

RuleComparison Rule::compare(Rule *rule){
	RuleComparison comp = EQUAL;
	std::string theOtherConditon = rule->getCondition();
	if(condition.compare(theOtherConditon) != 0){
		comp = NOT_EQUAL;
		unsigned int i = 0, j = 0;
		for(i = 0, j = 0;
				i < condition.length() && j < theOtherConditon.length();
				i++, j++){
			char thisChar = condition.at(i);
			char thisOtherChar = theOtherConditon.at(j);
//			cout << "i: " << i << " thisChar: " << thisChar << " j: " << j << " thisOtherChar: " << thisOtherChar << endl;
			if((thisChar == '?' && condition.at(i + 1) == '0')
					|| (thisChar == '<' && condition.at(i + 1) == '0')){
				if(i < condition.length()-1)
					i += 2;
				else
					break;

//				cout << "I am here i: " << i << endl;
			}
			if((thisOtherChar == '?' && theOtherConditon.at(j + 1) == '0')
					|| (thisOtherChar == '<' && theOtherConditon.at(j + 1) == '0')){
				if(j < theOtherConditon.length()-1)
					j += 2;
				else
					break;

//				cout << "I am here i: " << j << endl;
			}
			thisChar = condition.at(i);
			thisOtherChar = theOtherConditon.at(j);
//			cout << "i: " << i << " thisChar: " << thisChar << " j: " << j << " thisOtherChar: " << thisOtherChar << endl;
			if(thisChar == '?'
					&& (thisOtherChar != '?'
						|| thisOtherChar != '<'
						|| thisOtherChar != '>')){
				comp = GENERAL;
				i++;
			} else if(thisChar == '<'
					&& (thisOtherChar != '?'
						|| thisOtherChar != '<'
						|| thisOtherChar != '>')){
				int thisValue = (int)toDigit(condition.at(++i));
				int thisOtherValue = (int)toDigit(theOtherConditon.at(++j));
				if(thisValue > thisOtherValue){
					comp = GENERAL;
				} else {
					comp = NOT_EQUAL;
				}
			} else if(thisChar == '>'
					&& (thisOtherChar != '?'
						|| thisOtherChar != '<'
						|| thisOtherChar != '>')){
				int thisValue = (int)toDigit(condition.at(++i));
				int thisOtherValue = (int)toDigit(theOtherConditon.at(++j));
				if(thisValue < thisOtherValue){
					comp = GENERAL;
				} else {
					comp = NOT_EQUAL;
				}
			} else if(thisOtherChar == '?'
					&& (thisChar != '?'
						|| thisChar != '<'
						|| thisChar != '>')){
				//Conditional switch to SPECIFIC
				if(comp == GENERAL){ comp = NOT_EQUAL; break;} else comp = SPECIFIC;
				j++;
			} else if(thisOtherChar == '<'
					&& (thisChar != '?'
						|| thisChar != '<'
						|| thisChar != '>')){
				int thisValue = (int)toDigit(condition.at(++i));
				int thisOtherValue = (int)toDigit(theOtherConditon.at(++j));
				if(thisOtherValue > thisValue){
					//Conditional switch to SPECIFIC
					if(comp == GENERAL){ comp = NOT_EQUAL; break;} else comp = SPECIFIC;
				} else {
					comp = NOT_EQUAL;
				}
			} else if(thisOtherChar == '>'
					&& (thisChar != '?'
						|| thisChar != '<'
						|| thisChar != '>')){
				int thisValue = (int)toDigit(condition.at(++i));
				int thisOtherValue = (int)toDigit(theOtherConditon.at(++j));
				if(thisOtherValue < thisValue){
					//Conditional switch to SPECIFIC
					if(comp == GENERAL){ comp = NOT_EQUAL; break;} else comp = SPECIFIC;
				} else {
					comp = NOT_EQUAL;
				}
			} else if(thisChar == '?' && thisOtherChar == '?'){
				int thisValue = (int)toDigit(condition.at(++i));
				int thisOtherValue = (int)toDigit(theOtherConditon.at(++j));
//				cout << "i: " << i << "thisValue: " << thisValue << " j: " << j << " thisOtherValue: " << thisOtherValue << endl;
				if(thisValue > thisOtherValue){
					comp = GENERAL;
					int diff = thisValue - thisOtherValue;
//					cout << "general diff: " << diff << endl;
					if((j + diff) >= theOtherConditon.length()){
						j = theOtherConditon.length();
					} else {
						j += diff;
					}
				} else if(thisValue < thisOtherValue){
					//Conditional switch to SPECIFIC
					if(comp == GENERAL){ comp = NOT_EQUAL; break;} else comp = SPECIFIC;

					int diff = thisOtherValue - thisValue;
//					cout << "specific diff: " << diff << endl;
					if((i + diff) >= condition.length()){
						i = condition.length();
					} else {
						i += diff;
					}
				}
				continue;
			} else if(thisChar == '?' && thisOtherChar == '<'){
				char thisNextChar = condition.at(++i);
				char thisOtherNextChar = theOtherConditon.at(++j);
				if(thisNextChar == '1' && thisOtherNextChar == '0'){
					if(comp != GENERAL && comp != SPECIFIC){
						comp = EQUAL;
					}
				} else {
					comp = GENERAL;
				}
				continue;
			} else if(thisChar == '?' && thisOtherChar == '>'){
				i++; j++;
				comp = GENERAL;
				continue;
			} else if(thisChar == '<' && thisOtherChar == '?'){
				char thisNextChar = condition.at(++i);
				char thisOtherNextChar = theOtherConditon.at(++j);
				if(thisNextChar == '1' && thisOtherNextChar == '0'){
					if(comp != GENERAL && comp != SPECIFIC){
						comp = EQUAL;
					}
				} else {
					//Conditional switch to SPECIFIC
					if(comp == GENERAL){ comp = NOT_EQUAL; break;} else comp = SPECIFIC;
				}
				continue;
			} else if(thisChar == '<' && thisOtherChar == '<'){
				int thisValue = (int)toDigit(condition.at(++i));
				int thisOtherValue = (int)toDigit(theOtherConditon.at(++j));
//				cout << "i: " << i << "thisValue: " << thisValue << " j: " << j << " thisOtherValue: " << thisOtherValue << endl;
				if(thisValue > thisOtherValue){
					comp = GENERAL;
				} else if(thisValue < thisOtherValue){
					//Conditional switch to SPECIFIC
					if(comp == GENERAL){ comp = NOT_EQUAL; break;} else comp = SPECIFIC;
				} else {
					//Conditional switch to EQUAL
					if(comp != SPECIFIC) comp = EQUAL;
				}
				continue;
			} else if(thisChar == '<' && thisOtherChar == '>'){
				int thisValue = (int)toDigit(condition.at(++i));
				int thisOtherValue = (int)toDigit(theOtherConditon.at(++j));
//				cout << "i: " << i << "thisValue: " << thisValue << " j: " << j << " thisOtherValue: " << thisOtherValue << endl;
				if(thisValue > thisOtherValue){
					int diff = thisValue - thisOtherValue;
					if(diff > 1){
						comp = GENERAL;
					} else {
						//Conditional switch to EQUAL
						if(comp != SPECIFIC) comp = EQUAL;
					}
				} else {
					comp = NOT_EQUAL;
				}
				continue;
			} else if(thisChar == '>' && thisOtherChar == '?'){
				i++; j++;
				//Conditional switch to SPECIFIC
				if(comp == GENERAL){ comp = NOT_EQUAL; break;} else comp = SPECIFIC;
				continue;
			} else if(thisChar == '>' && thisOtherChar == '<'){
				int thisValue = (int)toDigit(condition.at(++i));
				int thisOtherValue = (int)toDigit(theOtherConditon.at(++j));
//				cout << "i: " << i << "thisValue: " << thisValue << " j: " << j << " thisOtherValue: " << thisOtherValue << endl;
				if(thisOtherValue > thisValue){
					int diff = thisOtherValue - thisValue;
					if(diff > 1){
						//Conditional switch to SPECIFIC
						if(comp == GENERAL){ comp = NOT_EQUAL; break;} else comp = SPECIFIC;
					} else {
						//Conditional switch to EQUAL
						if(comp != SPECIFIC) comp = EQUAL;
					}
				} else {
					comp = NOT_EQUAL;
				}
				continue;
			} else if(thisChar == '>' && thisOtherChar == '>'){
				char thisNextChar = condition.at(++i);
				char thisOtherNextChar = theOtherConditon.at(++j);
				if(thisNextChar > thisOtherNextChar){
					//Conditional switch to SPECIFIC
					if(comp == GENERAL){ comp = NOT_EQUAL; break;} else comp = SPECIFIC;
				} else if(thisNextChar < thisOtherNextChar){
					comp = GENERAL;
				} else {
					//Conditional switch to EQUAL
					if(comp != SPECIFIC) comp = EQUAL;
				}
				continue;
			} else if(thisChar == thisOtherChar){
				if(comp != GENERAL && comp != SPECIFIC){
					comp = EQUAL;
				}
				continue;
			} else if(thisChar != thisOtherChar){
				comp = NOT_EQUAL;
				break;
			}
		}
		if(comp == GENERAL){
			if(i == condition.length() && j < theOtherConditon.length()){
				comp = NOT_EQUAL;
			}
		} else if(comp == SPECIFIC){
			if(i < condition.length() && j == theOtherConditon.length()){
				comp = NOT_EQUAL;
			}
		} else if(comp == EQUAL){
			if((i == condition.length() && j < theOtherConditon.length())
					|| (i < condition.length() && j == theOtherConditon.length())){
				comp = NOT_EQUAL;
			}
		}
	}
	return comp;
}

/*
int main() {
	string r1cond = "1<034";
	string r2cond = "1?134";
	Rule *r1 = new Rule(r1cond, 'R');
	Rule *r2 = new Rule(r2cond, 'R');
	cout << r1->getString() << endl << r2->getString() << endl;
	RuleComparison comp = r1->compare(r2);
	switch(comp){
	case EQUAL:
		cout << "equal" << endl;
		break;
	case NOT_EQUAL:
		cout << "not equal" << endl;
		break;
	case GENERAL:
		cout << r1->getString() << " general" << endl;
		break;
	case SPECIFIC:
		cout << r1->getString() << " specific" << endl;
		break;
	}
//	set<string> strSet = r1->getSubset();
//	cout << "myset contains:";
//
//	set<string>::iterator it;
//	for (it = strSet.begin(); it != strSet.end(); ++it)
//		cout << ' ' << '\n' << *it;

	return 0;
}
*/
