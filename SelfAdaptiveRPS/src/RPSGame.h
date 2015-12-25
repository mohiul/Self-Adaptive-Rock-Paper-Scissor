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
#include "XMLConfigReader.h"

class XMLConfigReader;

class RPSGame {
	friend class XMLConfigReader;
public:
	RPSGame(std::string configFile);
	virtual ~RPSGame();
	void play(int noOfGame);
	std::string printRules(Player *player);
	std::string printAdapters(Player *player);
	void printResult();
protected:
	Player *player1;
	Player *player2;

private:
	XMLConfigReader *xmlConfigReader;
};

#endif /* RPSGAME_H_ */
