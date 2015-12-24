/*
 * SelfAdaptiveRPS.h
 *
 *  Created on: Dec 24, 2015
 *      Author: mohiul
 */

#ifndef SELFADAPTIVERPS_H_
#define SELFADAPTIVERPS_H_

#include <iostream>
#include <cstdlib>
#include <time.h>

#include <GL/glui.h>
#include <GL/glut.h>

extern GLUI *glui;

extern GLUI_EditText *initFile;
extern GLUI_TextBox *historyTextBox;
extern GLUI_TextBox *rulesTextBox;
extern GLUI_TextBox *actionTextBox;
extern GLUI_Button *ctlStart;
extern GLUI_Button *ctlStop;
extern GLUI_Button *ctlExit;
extern GLUI_Button *ctlRock;
extern GLUI_Button *ctlPaper;
extern GLUI_Button *ctlScissor;

#endif /* SELFADAPTIVERPS_H_ */
