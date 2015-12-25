/*
 * RPSGame.cpp
 *
 *  Created on: Nov 14, 2015
 *      Author: mohiul
 */

#include <sstream>
#include <string>

#include "RPSGame.h"
#include "SelfAdaptiveRPS.h"
#include "Adapter.h"

RPSGame::RPSGame(std::string configFile) {
	player1 = NULL;
	player2 = NULL;
	xmlConfigReader = new XMLConfigReader(this);
	xmlConfigReader->readConfigFile(configFile);
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
