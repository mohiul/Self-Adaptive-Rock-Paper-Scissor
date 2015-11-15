/*
 * RPSGame.h
 *
 *  Created on: Nov 14, 2015
 *      Author: mohiul
 */

#ifndef RPSGAME_H_
#define RPSGAME_H_

#include <iostream>
#include <list>

#include "Player.h"

class RPSGame {
public:
	RPSGame();
	virtual ~RPSGame();
	void play(int noOfGame);
	void printResult();
protected:
	std::list<Player*> players;
};

#endif /* RPSGAME_H_ */
