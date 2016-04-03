/*
 * XMLReader.cpp
 *
 *  Created on: Dec 25, 2015
 *      Author: mohiul
 */

#include "XMLConfigReader.h"

XMLConfigReader::XMLConfigReader(RPSGame *rpsGame) {
	this->rpsGame = rpsGame;

}

XMLConfigReader::~XMLConfigReader() {
	// TODO Auto-generated destructor stub
}

void XMLConfigReader::getRulesFromXML(TiXmlElement* playerElm, RuleEngine *player) {
	for (TiXmlElement* ruleElm = playerElm->FirstChildElement("rule");
			ruleElm != NULL; ruleElm = ruleElm->NextSiblingElement("rule")) {
		std::string rule = std::string(ruleElm->GetText());
		int delimiterLoc = rule.find(":");
		player->addRule(new Rule(rule.substr(0, delimiterLoc),
				rule.substr(delimiterLoc + 1, rule.length()).at(0)));
	}
}

void XMLConfigReader::getAdaptersFromXML(TiXmlElement* playerElm, RuleEngine *player) {
	for (TiXmlElement* adapterElm = playerElm->FirstChildElement("adapter");
			adapterElm != NULL; adapterElm = adapterElm->NextSiblingElement("adapter")) {
		TiXmlElement* conditionElm = adapterElm->FirstChildElement("rule");
		if(conditionElm != NULL){
			std::string rule = std::string(conditionElm->GetText());
			int delimiterLoc = rule.find(":");
			Adapter *adapter = new Adapter(new Rule(rule.substr(0, delimiterLoc),
					rule.substr(delimiterLoc + 1, rule.length()).at(0)));
			for (TiXmlElement* actionElm = adapterElm->FirstChildElement("action");
					actionElm != NULL; actionElm = actionElm->NextSiblingElement("action")) {
				std::string actionTxt = std::string(actionElm->GetText());
				if(actionTxt == "ADD"){
					adapter->addAction(ADD);
				} else if(actionTxt == "DEL"){
					adapter->addAction(DEL);
				} else if(actionTxt == "MOD"){
					adapter->addAction(MOD);
				} else if(actionTxt == "SHF"){
					adapter->addAction(SHF);
				} else {
					std::cerr << "Action: " << actionTxt << " not defined!!" << std::endl;
				}
			}
			player->addAdapter(adapter);
		}
	}
}

/**
 * Read the configuration XML file which contains the initial configuration
 * of the rules and the actions
 */
bool XMLConfigReader::readConfigFile(std::string configFile)
{
	TiXmlDocument doc( configFile );
	doc.LoadFile();

	TiXmlElement* root = doc.FirstChildElement( "root" );
	if ( root )
	{
		TiXmlElement* playerElm = root->FirstChildElement("player");
		if(playerElm != NULL){
			rpsGame->player1 = new RuleEngine(playerElm->Attribute("name"));
			getRulesFromXML(playerElm, rpsGame->player1);
			getAdaptersFromXML(playerElm, rpsGame->player1);

			playerElm = playerElm->NextSiblingElement("player");
			if(playerElm != NULL){
				rpsGame->player2 = new RuleEngine(playerElm->Attribute("name"));
				getRulesFromXML(playerElm, rpsGame->player2);
				getAdaptersFromXML(playerElm, rpsGame->player2);
			} else {
				std::cerr << "second player not found!!" << std::endl;
			}
		} else {
			std::cerr << "<player></player> not found!!" << std::endl;
		}
	} else {
		std::cerr << "Initial configuration file unformatted!!" << std::endl;
	}
	return true;
}
