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

#include "RuleEngine.h"
#include "SelfAdaptiveRPS.h"
#include "XMLConfigReader.h"

class XMLConfigReader;

class RPSGame {
	friend class XMLConfigReader;
public:
	RPSGame(std::string configFile);
	virtual ~RPSGame();
	void play(int noOfGame);
	void playOneMove(Move move);
	std::string printRules(RuleEngine *player);
	std::string printAdapters(RuleEngine *player);
	void printResult();
protected:
	RuleEngine *player1;
	RuleEngine *player2;
	static int gameNo;
	int gameIteration;
	std::string resultBoxText;
	std::string historyBoxText;
private:
	XMLConfigReader *xmlConfigReader;
	void updateTextBoxes(char p1Move, char p2Move);
	char getMoveFromEnum(Move move);
};

#endif /* RPSGAME_H_ */
