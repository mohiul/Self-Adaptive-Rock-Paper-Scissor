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

RPSGame::RPSGame() {
	// TODO Auto-generated constructor stub

}

RPSGame::~RPSGame() {
	// TODO Auto-generated destructor stub
}

void RPSGame::play(int noOfGame) {
	Player* player1 = new Player("P1");
	player1->addRule(new Rule("0?1", 'R'));
	player1->addRule(new Rule("1?2", 'P'));
	player1->addRule(new Rule("2?3", 'S'));
	player1->addRule(new Rule("123?3", 'R'));
	players.push_back(player1);

	Player* player2 = new Player("P2");
	players.push_back(player2);

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
		std::list<Player*>::const_iterator playerItr;
		for (playerItr = players.begin(); playerItr != players.end(); ++playerItr) {
			std::list<Rule*> rules = (*playerItr)->getRules();
			oss << "Player "<< (*playerItr)->getName() << " rules:\n";
			std::list<Rule*>::const_iterator iterator;
			int r = 1;
			for (iterator = rules.begin(); iterator != rules.end(); ++iterator) {
				Rule* rule = *iterator;
				oss << r << ": " << rule->getString() << "\n";
			    r++;
			}
		}

	    rulesTextBox->set_text(oss.str().c_str());

	}
	player1->printHistory();
	player2->printHistory();
}

void RPSGame::printResult() {
	std::list<Player*>::const_iterator iterator;
	for (iterator = players.begin(); iterator != players.end(); ++iterator) {
		Player* player = *iterator;
	    std::cout << player->getName()
	    		<< " Win: " << player->getWin()
				<< " Loose: " << player->getLoose()
				<< " Draw: " << player->getDraw();
	    std::cout << std::endl;
	}
}
