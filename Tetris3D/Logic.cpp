// Logic.cpp - implementation of the Logic class.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.

#include "Logic.h"

Logic::Logic(bool startscreen , bool options, bool howto, bool play, 
	bool run, bool pause, bool over, bool bsound, bool asound, float scrw, float scrh) :
	bShowStartScreen(startscreen), bShowOptionsScreen(options), 
	bShowHowtoScreen(howto), bShowPlayScreen(play),
	bAppRunning(run), bGamePaused(pause), bGameOver(over),
	bBackgroundSound(bsound), bActionsSound(asound),
	flScreenWidth(scrw), flScreenHeight(scrh), 
	iRoundMax(1), iMaxScore(15), iSystemSizeMax(10)		// Constants.
{
	bTrain = false;
	iRound = 1;
	iUserScore = 0;					// Num of planes annihilated so far.
	bNewRound = true;
	bNewOptionsScreen = false;
	bRotated = false;
	bRoundFinished = false;			// To show the message when someone looses.
	bNewFigure = false;

	iSystemSize = 4;				// Num of cells in x and z directions.
}

Logic::~Logic(void)
{
}

/*______________*/
// RoundParameters implementation.
RoundParameters::RoundParameters(float w, float h):
	flBoxWidth(w), flBoxHeight(h)
{
}

RoundParameters::~RoundParameters()
{
}