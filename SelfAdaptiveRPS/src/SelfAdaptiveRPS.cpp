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

enum
{
	START,
    PAUSE,
    STOP,
	ROCK,
    PAPER,
    SCISSOR,
	EXIT
};

GLUI *glui;

GLUI_EditText *initFile;
GLUI_TextBox *historyTextBox;
GLUI_TextBox *rulesTextBox;
GLUI_TextBox *actionTextBox;
GLUI_Button *ctlStart;
GLUI_Button *ctlStop;
GLUI_Button *ctlExit;
GLUI_Button *ctlRock;
GLUI_Button *ctlPaper;
GLUI_Button *ctlScissor;

RPSGame *rpsGame;
int iterations = 100;

/** Termination */
void exit()
{
	exit(0);
}

/**
 * Callback function for all GLUI controls.
 */
void control(int key)
{
    switch (key)
    {
    case START:
    	rpsGame = new RPSGame();
    	rpsGame->play(iterations);
    	rpsGame->printResult();
    	ctlStop->enable();
        break;

    case STOP:
    	if(rpsGame != NULL){
    		rpsGame->~RPSGame();
    		rpsGame = NULL;
    	}
		break;

    case ROCK:
		break;
    case PAPER:
		break;
    case SCISSOR:
		break;
    case EXIT:
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
    GLUI_Spinner *iteration = new GLUI_Spinner( controlPanel, "Iterations:", &iterations);
    iteration->set_int_limits( 10, 10000 );
    glui->add_column(false);

    GLUI_Panel *playPanel = glui->add_panel_to_panel(controlPanel, "Play");
    ctlRock = glui->add_button_to_panel(playPanel, "Rock", ROCK, control);
    ctlRock->disable();
    ctlPaper = glui->add_button_to_panel(playPanel, "Paper", PAPER, control);
    ctlPaper->disable();
    ctlScissor = glui->add_button_to_panel(playPanel, "Scissor", SCISSOR, control);
    ctlScissor->disable();

    GLUI_Panel *experimentPanel = glui->add_panel_to_panel(controlPanel, "Experiment");
    ctlStart = glui->add_button_to_panel(experimentPanel, "Start", START, control);
    ctlStop = glui->add_button_to_panel(experimentPanel, "Stop", STOP, control);
    ctlStop->disable();
    ctlExit = glui->add_button_to_panel(experimentPanel, "Exit", EXIT, control);

    GLUI_Panel *historyPanel = glui->add_panel("History");
    historyTextBox = new GLUI_TextBox(historyPanel,true,1,control);
    historyTextBox->set_h(textbox_height);
    historyTextBox->set_w(textbox_width);
    glui->add_column(false);

    GLUI_Panel *rulesPanel = glui->add_panel("Rules");
    rulesTextBox = new GLUI_TextBox(rulesPanel,true,1,control);
    rulesTextBox->set_h(textbox_height);
    rulesTextBox->set_w(textbox_width);
    glui->add_column(false);

    GLUI_Panel *actionPanel = glui->add_panel("Action");
    actionTextBox = new GLUI_TextBox(actionPanel,true,1,control);
    actionTextBox->set_h(textbox_height);
    actionTextBox->set_w(textbox_width);

	/* display callback invoked when window opened */
	glutMainLoop(); /* enter event loop */
	return 0;
}
