// Logic.h - declaration of the Logic and RoundParameters classes.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#ifndef LOGIC_H
#define LOGIC_H

#include "Observer.h"
#include "Material.h"

class Logic : public Subject	// Logic is a subject in the Observer pattern.
{
public:
	Logic(bool startscreen, bool options, bool howto, bool play, 
		bool run, bool pause, 
		bool over, bool bsound, bool asound, float scrw, float scrh);
	~Logic(void);

	// Public members.
public:
	bool bShowStartScreen;		// True if show options screen; false if play game screeen.
	bool bShowOptionsScreen;
	bool bShowHowtoScreen;
	bool bShowPlayScreen;
	bool bAppRunning;			// True if the game is going on.
	bool bGamePaused;
	bool bGameOver;				// True if the user has lost.
	bool bBackgroundSound;
	bool bActionsSound;			// True if play actions sounds.
	bool bTrain;				// True if we want to train.
	bool bNewRound;				// True if a new round has to be started.
	bool bNewOptionsScreen;		// When we switch to the options screen, create new buttons.
	bool bRoundFinished;		// To show the message when someone looses.
	bool bNewFigure;

	std::size_t iRound;			// Current round.
	std::size_t iSystemSize;	// Number of cells in x and z directions.
	int iUserScore;				// Number of planes annihilated so far.

	float flScreenWidth;
	float flScreenHeight;
	bool bRotated;				// True if the view has been rotated at the beginning of a round.

	const std::size_t iRoundMax;// Maximum number of rounds, it is == 1.
	const int iMaxScore;		// Maximum number of planes that can be annihilated sequentially.
	const std::size_t iSystemSizeMax;
};

//----------------------------------
class RoundParameters
{
public:
	RoundParameters(float w, float h);
	~RoundParameters();

	float flBoxWidth;
	float flBoxHeight;
};

typedef std::vector<std::tr1::shared_ptr<RoundParameters> > RoundParamsVector;

#endif // LOGIC_H