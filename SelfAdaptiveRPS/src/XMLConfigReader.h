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
	void getRulesFromXML(TiXmlElement* playerElm, Player *player);
	void getAdaptersFromXML(TiXmlElement* playerElm, Player *player);
public:
	XMLConfigReader(RPSGame *rpsGame);
	virtual ~XMLConfigReader();
	bool readConfigFile(std::string configFile);
};

#endif /* XMLCONFIGREADER_H_ */
