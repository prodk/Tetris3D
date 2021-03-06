// GameApp.h - GameApp class declaration.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#ifndef GAMEAPP_H
#define GAMEAPP_H

// Most headers are in the "Material.h" and in "Shape.h".

#include <cstdlib>		// Standard C functions, like fprintf.
#include <ctime>		// time() function for random seed.
#include <string>
#include "SdlScreen.h"

// The main class which contains all the game ingredients.
class GameApp : public Observer		// GameApp is an Observer.
{
public:
	GameApp(void);
	~GameApp(void);
	void manageGame();				// Check what screen should be used, and use it.
	void notify(Subject* s);		// Callback, invoked by the Subject in the Observer pattern.

	// Private methods.
private:
	int initLibraries();			// Initialize: SDL, OGL, sound, game params, rand numbers.
	void loadData();				// Load sounds, textures, fonts.
	void setupRoundParams();		// Initialize the parameters of all the rounds.
	void createScreens();
	void registerObservers();		// What objects should be tracked by the Subject in the Observer pattern.
	int setupSDL();
	int setupSound();				// Init fmod library.

	// Data load.
	std::tr1::shared_ptr<TEXTURE> loadTexture(const std::string &fileName);// Factory function.
	void loadSounds();
	void loadFonts();	
	void shutDown();				// Clean up resources.
	
	// Sounds.
	void playBackgroundSound();
	void checkErr(FMOD_RESULT r, std::string &msg);

	// OpenGl-specific.
	void setupRenderingContext();	// OGL initial state: depth, lights, materials, etc.
	void setupMatrices();			// Init transforms: texture matrix.
	void swapBuffers();				// Swap double buffer.	

	// Private members.
private:
	enum {START_SCREEN = 1, OPTIONS_SCREEN, HOWTO_SCREEN, PLAY_SCREEN};// 0 is for Button.
	float flScreenWidth;			// Screen width.
	float flScreenHeight;			// Screen height.

	// Game logic related.
	Logic logic;					// Contains all the flags controlling the flow of the app.
	bool bBackgroundSound;			// Whether to play background sound.
	RoundParamsVector roundParams;	// Parameters of the rounds. Now only 1 theme.

	// Screens.
	std::tr1::shared_ptr<StartScreen> startScreen;
	std::tr1::shared_ptr<OptionsScreen> optionsScreen;
	std::tr1::shared_ptr<HowtoScreen> howtoScreen;
	std::tr1::shared_ptr<PlayScreen> playScreen;
	SDL_Event sdlEvent;

	// FMOD sound.
	std::vector<FMOD::Sound*> sounds;
	FMOD::System *system;
	FMOD::Channel *channelOptions;
	std::vector<FMOD::Channel*> channelRound;	

	// Data.
	std::string strGameName;
	SDL_Surface* surfaceGame;			// SDL surface on which everything is rendered.
	std::vector<TTF_Font*> fonts;	
	std::vector<std::tr1::shared_ptr<TEXTURE> > textures;

	// Data-related constants.
	const std::size_t iNumOfSounds;
	const std::size_t iNumOfFonts;
	const std::size_t iNumOfTextures;
};
#endif // GAMEAPP_H