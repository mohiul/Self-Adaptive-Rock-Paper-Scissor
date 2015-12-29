/*
 * Utils.cpp
 *
 *  Created on: Dec 27, 2015
 *      Author: mohiul
 */

#include <iostream>
#include <sstream>
#include "Utils.h"

Utils::Utils() {
	// TODO Auto-generated constructor stub

}

Utils::~Utils() {
	// TODO Auto-generated destructor stub
}

std::string Utils::insertChar(std::string str, int position, char c){
	std::string first = str.substr(0, position);
	std::string second = str.substr(position, str.length());
	std::stringstream os;
	os << first << c << second;
	return os.str();
}

std::string Utils::insertString(std::string str, int position, std::string strToInsert){
	std::string first = str.substr(0, position);
	std::string second = str.substr(position, str.length());
	std::stringstream os;
	os << first << strToInsert << second;
	return os.str();
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