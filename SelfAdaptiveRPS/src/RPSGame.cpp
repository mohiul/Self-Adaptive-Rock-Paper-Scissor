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

void RPSGame::getRulesFromXML(TiXmlElement* playerElm, Player *player) {
	for (TiXmlElement* ruleElm = playerElm->FirstChildElement("rule");
			ruleElm != NULL; ruleElm = ruleElm->NextSiblingElement("rule")) {
		std::string rule = std::string(ruleElm->GetText());
		int delimiterLoc = rule.find(":");
		std::string condition = rule.substr(0, delimiterLoc);
		std::string action = rule.substr(delimiterLoc + 1, rule.length());
		player->addRule(new Rule(condition, action.at(0)));
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

			playerElm = playerElm->NextSiblingElement("player");
			if(playerElm != NULL){
				player2 = new Player(playerElm->Attribute("name"));
				getRulesFromXML(playerElm, player2);
			} else {
				std::cout << "second player not found!!" << std::endl;
			}
		} else {
			std::cout << "<player></player> not found!!" << std::endl;
		}
	} else {
		std::cout << "Initial configuration file unformatted!!" << std::endl;
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
