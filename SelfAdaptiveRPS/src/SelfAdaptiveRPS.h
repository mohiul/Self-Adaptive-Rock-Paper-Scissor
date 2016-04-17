/*
 * SelfAdaptiveRPS.h
 *
 *  Created on: Dec 24, 2015
 *      Author: mohiul
 */

#ifndef SELFADAPTIVERPS_H_
#define SELFADAPTIVERPS_H_

#define toDigit(c) (c-'0')
#define toChar(i) (i+'0')

#include <iostream>
#include <cstdlib>
#include <time.h>

#include <GL/glui.h>
#include <GL/glut.h>

extern GLUI *glui;

extern GLUI_EditText *initFile;
extern GLUI_TextBox *resultTextBox;
extern GLUI_TextBox *historyTextBox;
extern GLUI_TextBox *p1RulesTextBox;
extern GLUI_TextBox *p2RulesTextBox;
extern GLUI_TextBox *p1ActionTextBox;
extern GLUI_TextBox *p2ActionTextBox;
extern GLUI_Button *ctlStart;
extern GLUI_Button *ctlStop;
extern GLUI_Button *ctlExit;
extern GLUI_Button *ctlRock;
extern GLUI_Button *ctlPaper;
extern GLUI_Button *ctlScissor;

enum Radio {
	PLAY,
	EXPERIMENT
};

extern int radioSelection;

enum Move {
	ROCK,
	PAPER,
	SCISSOR
};

extern int noOfRuleEngine;
extern int noOfRulesPerEngine;
extern int iterations;
extern int noOfGamePlay;
extern float learningFactor;
extern float parentSelection;
extern float parentPoolWithReplacement;
extern float mutationRate;
extern int updateTextBoxesCheck;

#endif /* SELFADAPTIVERPS_H_ */
