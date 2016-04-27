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

int RPSGame::gameNo = 0;

RPSGame::RPSGame(std::string configFile) {
	player1 = NULL;
	player2 = NULL;
	gameIteration = 0;
	totalIteration = 0;
	gameNo++;
//	xmlConfigReader = new XMLConfigReader(this);
//	xmlConfigReader->readConfigFile(configFile);
	initPlayers();
	if(radioSelection == PLAY){
		player2->setName("U");
	}
	resultBoxText = resultTextBox->get_text();

	avgFitnessFile.open ("avgFitness.txt");
}

RPSGame::~RPSGame() {
	player1->~Player();
	player2->~Player();
//	xmlConfigReader->~XMLConfigReader();
	avgFitnessFile.close();
}

void RPSGame::setTotalIterations(int iterations) {
	totalIteration = iterations;
}

void RPSGame::updateTextBoxes(char p1Move, char p2Move) {
	std::ostringstream oss;

	oss << gameIteration << ": " << player1->getCurrentHistoryStr(false)
			<< " | " << player2->getCurrentHistoryStr(true) << std::endl
			<< historyBoxText;
	historyBoxText = oss.str();
	oss.str("");
	oss.clear();
	oss << player1->getName() << " | " << player2->getName() << std::endl;
	oss << historyBoxText;
	historyTextBox->set_text(oss.str().c_str());

//	Console print
//	std::cout << "Itr " << gameIteration << ": " << player1->getName() << ": "
//			<< p1Move << " " << player2->getName() << ": " << p2Move
//			<< std::endl;

	oss.str("");
	oss.clear();
	oss << printRules(player1);
	p1RulesTextBox->set_text(oss.str().c_str());
	oss.str("");
	oss.clear();
	if(radioSelection == EXPERIMENT){
		oss << printRules(player2);
		p2RulesTextBox->set_text(oss.str().c_str());
		oss.str("");
		oss.clear();
	}
	oss << printAdapters(player1);
	p1ActionTextBox->set_text(oss.str().c_str());
	if(radioSelection == EXPERIMENT){
		oss.str("");
		oss.clear();
		oss << printAdapters(player2);
		p2ActionTextBox->set_text(oss.str().c_str());
	}
}

char RPSGame::getMoveFromEnum(Move move){
	char toReturn = 0;
	switch(move){
	case ROCK:
		toReturn = 'R';
		break;
	case PAPER:
		toReturn = 'P';
		break;
	case SCISSOR:
		toReturn = 'S';
		break;
	default:
		std::cerr << "Move doesn't exist!" << std::endl;
	}
	return toReturn;
}

void RPSGame::playOneMove(Move move) {
	if(gameIteration > 0 && gameIteration % noOfGamePlay == 0){
		avgFitnessFile << gameIteration << ", " << player1->getAvgFitness() << ", " << player2->getAvgFitness() << endl;
		if(applyAdaptCheckValue){
			player1->bestRuleEngine->adapt();
			player2->bestRuleEngine->adapt();
		} else {
			player1->evolve();
			player2->evolve();
		}
	}
	char p1Move = player1->nextMove();
	char p2Move = getMoveFromEnum(move);
	player1->addHistory(p1Move, p2Move);
	player2->addHistory(p2Move, p1Move);

	if(updateTextBoxesCheck){
		updateTextBoxes(p1Move, p2Move);
	}

	player1->printHistory(gameIteration);
	player2->printHistory(gameIteration);
	printResult();
	gameIteration++;
}

bool RPSGame::play() {
	if(gameIteration >= totalIteration){
		player1->printHistory(gameIteration);
		player2->printHistory(gameIteration);
		return false;
	}
	if(gameIteration > 0 && gameIteration % noOfGamePlay == 0){
		avgFitnessFile << gameIteration << ", " << player1->getAvgFitness() << ", " << player2->getAvgFitness() << endl;
		if(applyAdaptCheckValue){
			player1->adapt();
			player2->adapt();
//			player1->selfAdapt();
//			player2->selfAdapt();
		} else {
			player1->evolve();
			player2->evolve();
		}
	}
	char p1Move = player1->nextMove();
	char p2Move = player2->nextMove();
	player1->addHistory(p1Move, p2Move);
	player2->addHistory(p2Move, p1Move);
	if(updateTextBoxesCheck){
		updateTextBoxes(p1Move, p2Move);
	}
	gameIteration++;
	return true;
}

std::string RPSGame::printRules(Player *player) {
    std::ostringstream oss;
    if(player->bestRuleEngine != NULL){
		std::list<Rule*> rules = player->getRules();
	//	oss << "Player "<< player->getName() << " rules:\n";
		std::list<Rule*>::const_iterator iterator;
		int r = 1;
		for (iterator = rules.begin(); iterator != rules.end(); ++iterator) {
			Rule* rule = *iterator;
			oss << r << ": " << rule->getString() << "\n";
			r++;
		}
    }
	return oss.str();
}

std::string RPSGame::printAdapters(Player *player) {
    std::ostringstream oss;
    if(player->bestRuleEngine != NULL){
		std::list<Adapter*> adapters = player->getAdapters();
	//	oss << "Player "<< player->getName() << " adapters:\n";
		std::list<Adapter*>::const_iterator iterator;
		int a = 1;
		for (iterator = adapters.begin(); iterator != adapters.end(); ++iterator) {
			Adapter* adapter = *iterator;
			oss << a << ": " << adapter->getString() << "\n";
			a++;
		}
    }
	return oss.str();
}

void RPSGame::printResult() {
	std::ostringstream oss;
	oss << "Game No: " << gameNo << std::endl;
	oss << player1->getName()
			<< " W: " << player1->getWin()
			<< " L: " << player1->getLoose()
			<< " D: " << player1->getDraw()
			<< std::endl;
	oss << player2->getName()
			<< " W: " << player2->getWin()
			<< " L: " << player2->getLoose()
			<< " D: " << player2->getDraw()
			<< std::endl;
	oss << resultBoxText;
	std::string result = oss.str();
	resultTextBox->set_text(result.c_str());
	std::cout << result;
}

void RPSGame::initPlayers() {
	player1 = initRandomPlayer("P1");
	player2 = initRandomPlayer("P2");

//	player2 = new Player();
//	player2->setName("P2");
//	RuleEngine* engine = new RuleEngine(player1);
//	player2->addRuleEngine(engine);
}

Player* RPSGame::initRandomPlayer(string name) {
	Player* player = new Player();
	player->setName(name);

	for(int i = 0; i < noOfRuleEngine; i++){
		RuleEngine* engine = new RuleEngine(player);
//		int noOfRules = rand() % noOfRulesPerEngine + 1;

//		for(int i = 0; i < noOfRules; i++){
		for(int i = 0; i < noOfRulesPerEngine; i++){
			engine->addRule(Rule::generateRule());
			if(applyAdaptCheckValue){
				engine->addAdapter(Adapter::generateAdapter());
			}
		}
		player->addRuleEngine(engine);
	}
	return player;
}
