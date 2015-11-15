/*
 * RPSGame.cpp
 *
 *  Created on: Nov 14, 2015
 *      Author: mohiul
 */

#include "RPSGame.h"

RPSGame::RPSGame() {
	// TODO Auto-generated constructor stub

}

RPSGame::~RPSGame() {
	// TODO Auto-generated destructor stub
}

void RPSGame::play(int noOfGame) {
	Player* player1 = new Player("The one");
	player1->addRule(new Rule("0?1", 'R'));
	player1->addRule(new Rule("1?2", 'P'));
	player1->addRule(new Rule("2?3", 'S'));
	players.push_back(player1);

	Player* player2 = new Player("The second");
	players.push_back(player2);

	for(int i = 0; i < noOfGame; i++){
		char p1Move = player1->nextMove();
		char p2Move = player2->nextMove();
		player1->addHistory(p1Move, p2Move);
		player2->addHistory(p2Move, p1Move);
		std::cout << "Itr " << i << ": "
				<< player1->getName() << ": "<< p1Move << " "
				<< player2->getName() << ": " << p2Move << std::endl;
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
