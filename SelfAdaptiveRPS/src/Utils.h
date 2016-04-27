/*
 * Utils.h
 *
 *  Created on: Dec 27, 2015
 *      Author: mohiul
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <list>
#include <string>

class Utils {
public:
	Utils();
	virtual ~Utils();
	static std::string insertChar(std::string, int position, char c);
	static std::string insertString(std::string, int position, std::string str);
	static char *convertListToArray(std::list<char> listToConvert);
	static std::list<char> convertStrToList(std::string strToConvert);
	static void printHistory(std::string name, std::list<char> history);
	static char nextRandomMove();
};

#endif /* UTILS_H_ */
