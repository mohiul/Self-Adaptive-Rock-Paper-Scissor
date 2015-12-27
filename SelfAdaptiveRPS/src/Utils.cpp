/*
 * Utils.cpp
 *
 *  Created on: Dec 27, 2015
 *      Author: mohiul
 */

#include "Utils.h"

Utils::Utils() {
	// TODO Auto-generated constructor stub

}

Utils::~Utils() {
	// TODO Auto-generated destructor stub
}

char *Utils::convertListToArray(std::list<char> listToConvert){
	char *arr = new char[listToConvert.size()];
	copy(listToConvert.begin(),listToConvert.end(),arr);
	return arr;
}

std::list<char> Utils::convertStrToList(std::string strToConvert){
	std::list<char> charList;
	for(int i = 0; i < strToConvert.length(); i++){
		charList.push_back(strToConvert.at(i));
	}
	return charList;
}
