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
#define textbox_height 195
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
GLUI_TextBox *p1RulesTextBox;
GLUI_TextBox *p2RulesTextBox;
GLUI_TextBox *p1ActionTextBox;
GLUI_TextBox *p2ActionTextBox;
GLUI_Spinner *iterationSpnr;
GLUI_Spinner *noOfRuleEngineSpnr;
GLUI_Spinner *noOfRulesPerEngineSpnr;
GLUI_Spinner *noOfGamePlaySpnr;
GLUI_Spinner *learningFactorSpnr;
GLUI_Spinner *parentSelectionSpnr;
GLUI_Spinner *parentPoolWithReplaceSpnr;
GLUI_Spinner *mutationRateSpnr;
GLUI_RadioGroup *radioGroup;
GLUI_Button *ctlStart;
GLUI_Button *ctlPause;
GLUI_Button *ctlStop;
GLUI_Button *ctlExit;
GLUI_Button *ctlRock;
GLUI_Button *ctlPaper;
GLUI_Button *ctlScissor;
GLUI_Checkbox *updateTextCheck;
GLUI_Checkbox *applyAdaptationCheck;
GLUI_Checkbox *initParentFitCheck;

int radioSelection = EXPERIMENT;
int noOfRuleEngine = 5;
int noOfRulesPerEngine = 10;
int iterations = 30;
int noOfGamePlay = 10;
float learningFactor = 0.01;
float parentSelection = 0.8;
float parentPoolWithReplacement = 0.2;
float mutationRate = 0.5;
int initParentFitCheckVal = 1;
int updateTextBoxesCheck = 1;
int applyAdaptCheckValue = 0;
int updateTextBoxesId;

static bool paused = false;

RPSGame *rpsGame;

/** Termination */
void exit() {
	exit(0);
}

void clear(){
	historyTextBox->set_text("");
	p1RulesTextBox->set_text("");
	p2RulesTextBox->set_text("");
	p1ActionTextBox->set_text("");
	p2ActionTextBox->set_text("");
	if(rpsGame != NULL){
		rpsGame->~RPSGame();
		rpsGame = NULL;
	}
}

void stop() {
	glutIdleFunc(0);
	if(rpsGame != NULL){
		rpsGame->~RPSGame();
		rpsGame = NULL;
	}
	ctlRock->disable();
	ctlPaper->disable();
	ctlScissor->disable();
	ctlStop->disable();
	ctlPause->disable();
	ctlStart->enable();
	iterationSpnr->enable();
}

void idle()
{
	if(!rpsGame->play()){
		glutIdleFunc(0);
		rpsGame->printResult();
		stop();
	}
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
		ctlPause->disable();
		break;
	case EXPERIMENT:
		iterationSpnr->enable();
		ctlStop->enable();
		ctlPause->enable();
		rpsGame->setTotalIterations(iterations);
		glutIdleFunc(idle);
		break;
	}
}

void pause() {
    if (paused)
    {
        glutIdleFunc(idle);
    }
    else
    {
    	rpsGame->printResult();
        glutIdleFunc(0);
    }
    paused = !paused;
}

void updateCheck(int key)
{
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

    case PAUSE_BTN:
    	pause();
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
//    controlPanel->set_alignment(GLUI_ALIGN_RIGHT);

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
//    glui->add_column_to_panel(controlPanel);

    GLUI_Panel *experimentPanel = glui->add_panel_to_panel(controlPanel, "Experiment");
    ctlStart = glui->add_button_to_panel(experimentPanel, "Start", START_BTN, control);
    ctlPause = glui->add_button_to_panel(experimentPanel, "Pause", PAUSE_BTN, control);
    ctlStop = glui->add_button_to_panel(experimentPanel, "Stop", STOP_BTN, control);
    ctlStop->disable();
    ctlExit = glui->add_button_to_panel(experimentPanel, "Exit", EXIT_BTN, control);

    GLUI_Panel *initPanel = glui->add_panel_to_panel(controlPanel, "Initialization");
    updateTextCheck = glui->add_checkbox_to_panel(initPanel, "Update Text Boxes", &updateTextBoxesCheck, updateTextBoxesId, updateCheck);

    applyAdaptationCheck = glui->add_checkbox_to_panel(initPanel, "Apply Adaptation", &applyAdaptCheckValue, updateTextBoxesId, updateCheck);


    initFile = glui->add_edittext_to_panel(initPanel, "Rule Init File ", GLUI_EDITTEXT_TEXT);
    initFile->set_w(200);
    initFile->enable();
    initFile->set_text("initialconfig.xml");

    GLUI_Panel *paramPanel = glui->add_panel_to_panel(controlPanel, "Parameters");
    noOfRuleEngineSpnr = new GLUI_Spinner( paramPanel, "No of Rule Engine:", &noOfRuleEngine);
    noOfRuleEngineSpnr->set_int_limits( 1, 1000 );

    noOfRulesPerEngineSpnr = new GLUI_Spinner( paramPanel, "No of Rules per Engine:", &noOfRulesPerEngine);
    noOfRulesPerEngineSpnr->set_int_limits( 1, 1000 );

    iterationSpnr = new GLUI_Spinner( paramPanel, "Iterations:", &iterations);
    iterationSpnr->set_int_limits( 2, 10000 );

    noOfGamePlaySpnr = new GLUI_Spinner( paramPanel, "No of Game Play:", &noOfGamePlay);
    noOfGamePlaySpnr->set_int_limits( 1, 500 );

    learningFactorSpnr = new GLUI_Spinner( paramPanel, "Learning Factor:", &learningFactor);
    learningFactorSpnr->set_float_limits( 0.001, 1.0 );

    parentSelectionSpnr = new GLUI_Spinner( paramPanel, "Parent Selection:", &parentSelection);
    parentSelectionSpnr->set_float_limits( 0.0, 1.0 );

    parentPoolWithReplaceSpnr = new GLUI_Spinner( paramPanel, "Parent Replacement:", &parentPoolWithReplacement);
    parentPoolWithReplaceSpnr->set_float_limits( 0.0, 1.0 );

    initParentFitCheck = glui->add_checkbox_to_panel(paramPanel, "Initialize parent fitness", &initParentFitCheckVal, updateTextBoxesId, updateCheck);

    mutationRateSpnr = new GLUI_Spinner( paramPanel, "Mutation Rate:", &mutationRate);
    mutationRateSpnr->set_float_limits( 0.1, 1.0 );

    glui->add_column(false);

    GLUI_Panel *resultPanel = glui->add_panel("Result");
    resultTextBox = new GLUI_TextBox(resultPanel, true, 1, control);
    resultTextBox->set_h(textbox_height);
    resultTextBox->set_w(textbox_width);

    GLUI_Panel *historyPanel = glui->add_panel("History");
    historyTextBox = new GLUI_TextBox(historyPanel, true, 1, control);
    historyTextBox->set_h(textbox_height);
    historyTextBox->set_w(textbox_width);
    glui->add_column(false);

    GLUI_Panel *p1RulesPanel = glui->add_panel("Player 1 Rules");
    p1RulesTextBox = new GLUI_TextBox(p1RulesPanel,true,1,control);
    p1RulesTextBox->set_h(textbox_height);
    p1RulesTextBox->set_w(textbox_width);
//    glui->add_column(false);

    GLUI_Panel *p1ActionPanel = glui->add_panel("Player 1 Adapters");
    p1ActionTextBox = new GLUI_TextBox(p1ActionPanel,true,1,control);
    p1ActionTextBox->set_h(textbox_height);
    p1ActionTextBox->set_w(textbox_width);
    glui->add_column(false);

    GLUI_Panel *p2RulesPanel = glui->add_panel("Player 2 Rules");
    p2RulesTextBox = new GLUI_TextBox(p2RulesPanel,true,1,control);
    p2RulesTextBox->set_h(textbox_height);
    p2RulesTextBox->set_w(textbox_width);
//    glui->add_column(false);

    GLUI_Panel *p2ActionPanel = glui->add_panel("Player 2 Adapters");
    p2ActionTextBox = new GLUI_TextBox(p2ActionPanel,true,1,control);
    p2ActionTextBox->set_h(textbox_height);
    p2ActionTextBox->set_w(textbox_width);

	/* display callback invoked when window opened */
	glutMainLoop(); /* enter event loop */
	return 0;
}
