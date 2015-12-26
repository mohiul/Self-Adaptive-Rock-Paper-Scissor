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

#include "SelfAdaptiveRPS.h"
#include "Player.h"
#include "XMLConfigReader.h"

class XMLConfigReader;

class RPSGame {
	friend class XMLConfigReader;
public:
	RPSGame(std::string configFile);
	virtual ~RPSGame();
	void play(int noOfGame);
	void playOneMove(Move move);
	std::string printRules(Player *player);
	std::string printAdapters(Player *player);
	void printResult();
protected:
	Player *player1;
	Player *player2;
	static int gameNo;
	int gameIteration;
private:
	XMLConfigReader *xmlConfigReader;
	void updateTextBoxes(char p1Move, char p2Move);
	char getMoveFromEnum(Move move);
};

#endif /* RPSGAME_H_ */
