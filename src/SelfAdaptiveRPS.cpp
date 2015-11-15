//============================================================================
// Name        : SelfAdaptiveRPS.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdlib>
#include <time.h>
#include "RPSGame.h"

using namespace std;

int main() {
	srand(time(NULL));
	cout << "!!!Rock Paper Scissor!!!" << endl;
	RPSGame game;
	int noOfPlay = 1000;
	game.play(noOfPlay);
	game.printResult();
	return 0;
}
