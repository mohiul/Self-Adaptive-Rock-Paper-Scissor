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
	static char *convertListToArray(std::list<char> listToConvert);
	static std::list<char> convertStrToList(std::string strToConvert);
};

#endif /* UTILS_H_ */
