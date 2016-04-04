/*
 * XMLReader.h
 *
 *  Created on: Dec 25, 2015
 *      Author: mohiul
 */

#ifndef XMLCONFIGREADER_H_
#define XMLCONFIGREADER_H_

#include "tinyxml.h"
#include "RPSGame.h"

class RPSGame;

class XMLConfigReader {
protected:
	RPSGame *rpsGame;
	void getRuleEnginesFromXML(TiXmlElement* playerElm, Player *player);
	void getRulesFromXML(TiXmlElement* playerElm, RuleEngine* engine);
	void getAdaptersFromXML(TiXmlElement* playerElm, RuleEngine* engine);
public:
	XMLConfigReader(RPSGame *rpsGame);
	virtual ~XMLConfigReader();
	bool readConfigFile(std::string configFile);
};

#endif /* XMLCONFIGREADER_H_ */
