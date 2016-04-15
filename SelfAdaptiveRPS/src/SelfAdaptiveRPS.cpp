/*
 * SelfAdaptiveRPS.cpp
 *
 *  Created on: Dec 22, 2015
 *      Author: mohiul
 */

#include "SelfAdaptiveRPS.h"
#include "RPSGame.h"

#define window_width  640
#define window_height 480
#define textbox_width 150
#define textbox_height 215
#define resultbox_width 130
#define resultbox_height 155

enum
{
	START_BTN,
    PAUSE_BTN,
    STOP_BTN,
	ROCK_BTN,
    PAPER_BTN,
    SCISSOR_BTN,
	EXIT_BTN
};

GLUI *glui;

GLUI_EditText *initFile;
GLUI_TextBox *resultTextBox;
GLUI_TextBox *historyTextBox;
GLUI_TextBox *rulesTextBox;
GLUI_TextBox *actionTextBox;
GLUI_Spinner *iterationSpnr;
GLUI_Spinner *noOfGamePlaySpnr;
GLUI_Spinner *learningFactorSpnr;
GLUI_Spinner *parentSelectionSpnr;
GLUI_Spinner *parentPoolWithReplaceSpnr;
GLUI_Spinner *mutationRateSpnr;
GLUI_RadioGroup *radioGroup;
GLUI_Button *ctlStart;
GLUI_Button *ctlStop;
GLUI_Button *ctlExit;
GLUI_Button *ctlRock;
GLUI_Button *ctlPaper;
GLUI_Button *ctlScissor;

int radioSelection = EXPERIMENT;
int iterations = 100;
int noOfGamePlay = 10;
float learningFactor = 0.1;
float parentSelection = 0.6;
float parentPoolWithReplacement = 0.4;
float mutationRate = 0.5;

RPSGame *rpsGame;

/** Termination */
void exit() {
	exit(0);
}

void clear(){
	historyTextBox->set_text("");
	rulesTextBox->set_text("");
	actionTextBox->set_text("");
	if(rpsGame != NULL){
		rpsGame->~RPSGame();
		rpsGame = NULL;
	}
}

void stop() {
	if(rpsGame != NULL){
		rpsGame->~RPSGame();
		rpsGame = NULL;
	}
	ctlRock->disable();
	ctlPaper->disable();
	ctlScissor->disable();
	ctlStop->disable();
	ctlStart->enable();
}

void start() {
	clear();
	rpsGame = new RPSGame(std::string(initFile->get_text()));
	switch(radioSelection){
	case PLAY:
		iterationSpnr->disable();
		ctlRock->enable();
		ctlPaper->enable();
		ctlScissor->enable();
		ctlStop->enable();
		ctlStart->disable();
		break;
	case EXPERIMENT:
		iterationSpnr->enable();
		ctlStop->enable();
		rpsGame->play(iterations);
		rpsGame->printResult();
		stop();
		break;
	}
}

/**
 * Callback function for all GLUI controls.
 */
void control(int key)
{
    switch (key)
    {
    case START_BTN:
    	start();
        break;

    case STOP_BTN:
    	stop();
		break;

    case ROCK_BTN:
    	rpsGame->playOneMove(ROCK);
		break;
    case PAPER_BTN:
    	rpsGame->playOneMove(PAPER);
		break;
    case SCISSOR_BTN:
    	rpsGame->playOneMove(SCISSOR);
		break;
    case EXIT_BTN:
    	exit();
		break;
   }
}

int main(int argc, char** argv)
{
	srand(time(NULL));
	/* standard GLUT initialization */

	glutInit(&argc,argv);

    // Set up model Window
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); /* default, not needed */
	glClearColor(0.5, 0.5, 0.5, 1);
	glutIdleFunc(0);

    // GLUI initialization
    glui = GLUI_Master.create_glui("Rock Paper Scissor", 0, 200, 200);

    GLUI_Panel *controlPanel = glui->add_panel("Control");
    controlPanel->set_alignment(GLUI_ALIGN_RIGHT);

    initFile = glui->add_edittext_to_panel(controlPanel, "Rule Init File ", GLUI_EDITTEXT_TEXT);
    initFile->set_w(150);
    initFile->enable();
    initFile->set_text("initialconfig.xml");
    iterationSpnr = new GLUI_Spinner( controlPanel, "Iterations:", &iterations);
    iterationSpnr->set_int_limits( 10, 10000 );

    noOfGamePlaySpnr = new GLUI_Spinner( controlPanel, "No of Game Play:", &noOfGamePlay);
    noOfGamePlaySpnr->set_int_limits( 1, 500 );

    learningFactorSpnr = new GLUI_Spinner( controlPanel, "Learning Factor:", &learningFactor);
    learningFactorSpnr->set_float_limits( 0.1, 1.0 );

    parentSelectionSpnr = new GLUI_Spinner( controlPanel, "Parent Selection:", &parentSelection);
    parentSelectionSpnr->set_float_limits( 0.1, 1.0 );

    parentPoolWithReplaceSpnr = new GLUI_Spinner( controlPanel, "Parent Replacement:", &parentPoolWithReplacement);
    parentPoolWithReplaceSpnr->set_float_limits( 0.1, 1.0 );

    mutationRateSpnr = new GLUI_Spinner( controlPanel, "Mutation Rate:", &mutationRate);
    mutationRateSpnr->set_float_limits( 0.1, 1.0 );

    GLUI_Panel *resultPanel = glui->add_panel_to_panel(controlPanel, "Result");
    resultTextBox = new GLUI_TextBox(resultPanel, true, 1, control);
    resultTextBox->set_h(resultbox_height);
    resultTextBox->set_w(resultbox_width);
    glui->add_column_to_panel(controlPanel, false);

    GLUI_Panel *selectPanel = glui->add_panel_to_panel(controlPanel, "Select");

    radioGroup = glui->add_radiogroup_to_panel(selectPanel, &radioSelection);
    glui->add_radiobutton_to_group(radioGroup, "Play");
    glui->add_radiobutton_to_group(radioGroup, "Experiment");

    GLUI_Panel *playPanel = glui->add_panel_to_panel(controlPanel, "Play");
    ctlRock = glui->add_button_to_panel(playPanel, "Rock", ROCK_BTN, control);
    ctlRock->disable();
    ctlPaper = glui->add_button_to_panel(playPanel, "Paper", PAPER_BTN, control);
    ctlPaper->disable();
    ctlScissor = glui->add_button_to_panel(playPanel, "Scissor", SCISSOR_BTN, control);
    ctlScissor->disable();

    GLUI_Panel *experimentPanel = glui->add_panel_to_panel(controlPanel, "Experiment");
    ctlStart = glui->add_button_to_panel(experimentPanel, "Start", START_BTN, control);
    ctlStop = glui->add_button_to_panel(experimentPanel, "Stop", STOP_BTN, control);
    ctlStop->disable();
    ctlExit = glui->add_button_to_panel(experimentPanel, "Exit", EXIT_BTN, control);
    glui->add_column(false);

    GLUI_Panel *historyPanel = glui->add_panel("History");
    historyTextBox = new GLUI_TextBox(historyPanel, true, 1, control);
    historyTextBox->set_h(textbox_height);
    historyTextBox->set_w(textbox_width);
    glui->add_column(false);

    GLUI_Panel *rulesPanel = glui->add_panel("Rules");
    rulesTextBox = new GLUI_TextBox(rulesPanel,true,1,control);
    rulesTextBox->set_h(textbox_height);
    rulesTextBox->set_w(textbox_width);
    glui->add_column(false);

    GLUI_Panel *actionPanel = glui->add_panel("Adapters");
    actionTextBox = new GLUI_TextBox(actionPanel,true,1,control);
    actionTextBox->set_h(textbox_height);
    actionTextBox->set_w(textbox_width);

	/* display callback invoked when window opened */
	glutMainLoop(); /* enter event loop */
	return 0;
}
