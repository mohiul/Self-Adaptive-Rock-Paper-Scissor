/*
 * RPSGame.cpp
 *
 *  Created on: Nov 14, 2015
 *      Author: mohiul
 */

#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>

#include "RPSGame.h"
#include "SelfAdaptiveRPS.h"
#include "Adapter.h"

void RPSGame::getRulesFromXML(TiXmlElement* playerElm, Player *player) {
	for (TiXmlElement* ruleElm = playerElm->FirstChildElement("rule");
			ruleElm != NULL; ruleElm = ruleElm->NextSiblingElement("rule")) {
		std::string rule = std::string(ruleElm->GetText());
		int delimiterLoc = rule.find(":");
		player->addRule(new Rule(rule.substr(0, delimiterLoc),
				rule.substr(delimiterLoc + 1, rule.length()).at(0)));
	}
}

void RPSGame::getAdaptersFromXML(TiXmlElement* playerElm, Player *player) {
	for (TiXmlElement* adapterElm = playerElm->FirstChildElement("adapter");
			adapterElm != NULL; adapterElm = adapterElm->NextSiblingElement("adapter")) {
		TiXmlElement* conditionElm = adapterElm->FirstChildElement("condition");
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
bool RPSGame::readConfigFile(std::string configFile)
{
	TiXmlDocument doc( configFile );
	doc.LoadFile();

	TiXmlElement* root = doc.FirstChildElement( "root" );
	if ( root )
	{
		TiXmlElement* playerElm = root->FirstChildElement("player");
		if(playerElm != NULL){
			player1 = new Player(playerElm->Attribute("name"));
			getRulesFromXML(playerElm, player1);
			getAdaptersFromXML(playerElm, player1);

			playerElm = playerElm->NextSiblingElement("player");
			if(playerElm != NULL){
				player2 = new Player(playerElm->Attribute("name"));
				getRulesFromXML(playerElm, player2);
				getAdaptersFromXML(playerElm, player2);
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

RPSGame::RPSGame(std::string configFile) {
	readConfigFile(configFile);

}

RPSGame::~RPSGame() {
	// TODO Auto-generated destructor stub
}

void RPSGame::play(int noOfGame) {

	for(int i = 0; i < noOfGame; i++){
		char p1Move = player1->nextMove();
		char p2Move = player2->nextMove();
		player1->addHistory(p1Move, p2Move);
		player2->addHistory(p2Move, p1Move);

	    std::ostringstream oss;
	    oss << historyTextBox->get_text() << "\n" << i << ": "
				<< player1->getName() << ": "<< p1Move << " | "
				<< player2->getName() << ": " << p2Move;
	    historyTextBox->set_text(oss.str().c_str());

		std::cout << "Itr " << i << ": "
				<< player1->getName() << ": "<< p1Move << " "
				<< player2->getName() << ": " << p2Move << std::endl;

		oss.str("");
		oss.clear();
		oss << printRules(player1) << printRules(player2);
	    rulesTextBox->set_text(oss.str().c_str());

		oss.str("");
		oss.clear();
		oss << printAdapters(player1) << printAdapters(player2);
	    actionTextBox->set_text(oss.str().c_str());

	}
	player1->printHistory();
	player2->printHistory();
}

std::string RPSGame::printRules(Player *player) {
    std::ostringstream oss;
	std::list<Rule*> rules = player->getRules();
	oss << "Player "<< player->getName() << " rules:\n";
	std::list<Rule*>::const_iterator iterator;
	int r = 1;
	for (iterator = rules.begin(); iterator != rules.end(); ++iterator) {
		Rule* rule = *iterator;
		oss << r << ": " << rule->getString() << "\n";
		r++;
	}
	return oss.str();
}

std::string RPSGame::printAdapters(Player *player) {
    std::ostringstream oss;
	std::list<Adapter*> adapters = player->getAdapters();
	oss << "Player "<< player->getName() << " adapters:\n";
	std::list<Adapter*>::const_iterator iterator;
	int a = 1;
	for (iterator = adapters.begin(); iterator != adapters.end(); ++iterator) {
		Adapter* adapter = *iterator;
		oss << a << ": " << adapter->getString() << "\n";
		a++;
	}
	return oss.str();
}

void RPSGame::printResult() {
	std::cout << player1->getName()
			<< " Win: " << player1->getWin()
			<< " Loose: " << player1->getLoose()
			<< " Draw: " << player1->getDraw()
			<< std::endl;
	std::cout << player2->getName()
			<< " Win: " << player2->getWin()
			<< " Loose: " << player2->getLoose()
			<< " Draw: " << player2->getDraw()
			<< std::endl;
}
