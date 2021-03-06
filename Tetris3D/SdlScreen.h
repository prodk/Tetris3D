// SdlScreen.h - declaration of the SdlScreen class and all its offsprings.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#ifndef SDL_SCREEN
#define SDL_SCREEN

#include <map>
#include "GuiObject.h"
#include "Figure.h"
#include "FixedCubes.h"

typedef std::vector<std::tr1::shared_ptr<TEXTURE> > TEXTURE_PTR_ARRAY;

// Abstract base class for screens.
class SdlScreen : public Observer		// All screens are observers.
{
public:
	SdlScreen(int idExt, float w, float h, SDL_Surface*, TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
		FMOD::System *sys, std::vector<FMOD::Sound*> snd);
	virtual ~SdlScreen(void);

	// Public methods to override. Some of them have the default implementation.
	virtual void doDrawing(Logic &logic) = 0;
	virtual void doInput(Logic &l, SDL_Event sdlEvent);	// Not pure virtual, Keyboard/mouse.	
	virtual void notify(Subject* s);

	// Class specific method with fixed implementation.
	void setScreenSize(float w, float h);

protected:
	int drawText(const std::string & txt, GLfloat x, GLfloat y, GLfloat w, GLfloat h,
		TTF_Font *font, Logic &logic, SDL_Color textColor);
	void drawBackgroundTexture(int id, float z);

	// Event handlers, possibly to override.
	virtual void handleKeyDown(const SDL_Event& sdle, Logic &l);
	virtual void handleKeyUp(const SDL_Event& sdle, Logic &l);
	virtual void handleResize(const SDL_Event& sdle, Logic &l);
	virtual void handleMouseMotion(const SDL_Event& sdle, Logic &l);
	virtual void handleMouseButtonDown(const SDL_Event& sdle, Logic &l);
	virtual void handleMouseButtonUp(const SDL_Event& sdle, Logic &l);

	// Members.
protected:
	enum{START_BTN, OPTIONS_BTN, HOWTO_BTN, TRAIN_BTN,			// Start screen buttons.
		 BACKGRSND_BTN, ACTIONSND_BTN, ROUND_BTN, BACK_BTN};	// Options screen buttons.

	int id;
	float flWidth;								// Screen width.
	float flHeight;								// Screen height.
	SDL_Surface* surfaceOnWhichWeRender;
	bool bLeftMouseButton;						// Whether left mouse button was pressed.
	SDL_Event sdlEvent;

	TEXTURE_PTR_ARRAY textures;
	FMOD::System *system;
	std::vector<FMOD::Sound*> sounds;
	FMOD::Channel *channel;
	TTF_Font** fonts;	
};

//-------------------------
// ButtonScreen.
class ButtonScreen : public SdlScreen
{
public:
	ButtonScreen(int idExt, float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
		FMOD::System *sys, std::vector<FMOD::Sound*> snd);
	virtual ~ButtonScreen();
	virtual void doDrawing(Logic &logic);
	virtual void notify(Subject* s);

protected:
	typedef std::map<std::size_t, std::tr1::shared_ptr<GuiObject> >::iterator map_iter;
	virtual void handleMouseButtonDown(const SDL_Event& sdle, Logic &l);
	virtual void handleMouseButtonUp(const SDL_Event& sdle, Logic &l);
	virtual void handleMouseMotion(const SDL_Event& sdle, Logic &logic);
	virtual void handleKeyDown(const SDL_Event& sdle, Logic &logic);
	virtual void handleKeyUp(const SDL_Event& sdle, Logic &logic);

	// Button management.
	int pressButton(float x, float y);	// Sets a button under cursor pressed and returns its id.
	void setUnpressed();				// Sets all the buttons unpressed.
	int pickButton(float px, float py);	// Returns the id of the button under the cursor.
	void processButton(std::size_t id, Logic & logic);	// Perform actions with logic depending on the button id.
	int getHighlightedButton();			// Gets the id of the highlighted button.

protected:
	std::map<std::size_t, std::tr1::shared_ptr<GuiObject> > guiObjects;
	bool bPlayButtonSound;
	bool bMouseOverButton;				// To prevent from several higlight messages.
	bool bKeyDown;
	int iPrevFocusButton;				// Previously highlighted button.
};

//--------------------------------
// StartScreen is a ButtonScreen.
class StartScreen : public ButtonScreen
{
public:
	StartScreen(int idExt, float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
		FMOD::System *sys, std::vector<FMOD::Sound*> snd);
	~StartScreen(void);
	
private:
	void addButtons();
};

//----------------------------------
// OptionsScreen is a ButtonScreen.
class OptionsScreen : public ButtonScreen
{
public:
	OptionsScreen(int idExt, float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
		FMOD::System *sys, std::vector<FMOD::Sound*> snd, Logic &logic);
	~OptionsScreen(void);

	void setupNewScreen(Logic &logic);
	
private:
	void addButtons(Logic & logic);
};

//----------------------------------
// HowtoScreen is a ButtonScreen.
class HowtoScreen : public ButtonScreen
{
public:
	HowtoScreen(int idExt, float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
		FMOD::System *sys, std::vector<FMOD::Sound*> snd);
	~HowtoScreen(void);

	// Overridden virtual functions.
	void doDrawing(Logic &logic);
	
private:
	void handleMouseButtonUp(const SDL_Event& sdle, Logic &l);
	void handleKeyDown(const SDL_Event& sdle, Logic &l);
};

//----------------------------------
// PlayScreen declaration.
class PlayScreen : public SdlScreen
{
public:
	PlayScreen(int idExt, float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt, 
		FMOD::System *sys, std::vector<FMOD::Sound*> snd, 
		RoundParamsVector & rp);
	~PlayScreen(void);

	// Overridden virtual functions.	
	void doInput(Logic &l, SDL_Event sdlEvent);			// Keyboard/mouse.
	void doDrawing(Logic &logic);
	void notify(Subject* s);

	// PlayScreen-specific.
	void play(Logic &l, SDL_Event sdlEvent);			// Play the game.

	// Private methods.
private:
	// PlayScreen-specific methods.
	void doLogic(Logic &l);
	void setupNewRound(Logic &logic);

	// Event handlers overridden.
	void handleKeyDown(const SDL_Event& sdle, Logic &l);
	void handleResize(const SDL_Event& sdle, Logic &l);
	void handleMouseMotion(const SDL_Event& sdle, Logic &l);
	void handleMouseButtonDown(const SDL_Event& sdle, Logic &l);

	// OpenGl-specific.
	void drawAxes() const;
	void initView();				// Avoid code duplication in doDrawing() and pickObjects().
	void initResize();				// Avoid code dupl. in setupMatrices() and handleResize().
	void rotateView(float dx);	
	void initMembers(const Logic &l);

	// Inline: calculate the angle used in the perspective.
	float calculateAngle(float size, double zAxisDistance) {
		double radtheta, degtheta;
		radtheta = 2.0 * std::atan2 (size/2.0, zAxisDistance);
		degtheta = (180.0 * radtheta) / atan(1.);
		return (degtheta);
	}

	// Optimized text output.
	std::tr1::shared_ptr<TEXTURE> createStringTexture(const std::string &txt, 
		TTF_Font *font, SDL_Color textColor);	// Create string texture only once.
	void drawStringTexture(int symbolIdx, float x, float y, float z, float w, float h, 
		float angley);
	void drawScoreAndRoundOptimized(Logic &logic);

	// Game-logic related.
	// Cells-related.
	void manageCellsFilling();				// Specify which cells should be highlighted.
	void setHighlightedCells(std::vector<CellIndeces>& cells);
	void resetHighlightedCells(std::vector<CellIndeces>& cells);
	void checkCollision(Logic &logic);		// Figure collision with the fixed cubes.
	void annihilateLayers(Logic &logic);	// Remove completely filled planes of cubes.
	std::tr1::shared_ptr<Figure> createNewFigure();	// Factory function.
	void createPlanes();
	int getTopEmptyPlane(int iStart, int iEnd);
	// Plane-related.	
	void drawPlanes();
	void addCubes();
	void dropFigure(Logic& logic);		// Make the figure to fall down.
	bool isRoundFinished(Logic& logic);	// True if the figure touches the highest filled plane 
										// or the user annihilated 15 layers.
	// Private members.
private:
	// Game logic.
	// Figure-related.
	std::tr1::shared_ptr<Figure> currentFigure;
	std::tr1::shared_ptr<Figure> nextFigure;
	vector_3d vFigureOrigin;
	int cubesPerFigure;
	int iCurrentFigureId;
	int iFrameDelayMove;	// Number of frames to wait before moves, the falling speed.
	float cubeSize;

	// Planes-related.
	std::vector<std::tr1::shared_ptr<PlaneOfCells> > plane;	// Planes containing cells.
	std::tr1::shared_ptr<FixedCubes> fixedCubes;
	int iNumOfPlanes;
	int iNumOfCellsX;
	int iNumOfCellsZ;
	int bottomPlane;
	int secondBottomPlane;
	int iLowestCurrentPlane;	
	std::vector<CellIndeces> currentCells;	// Indeces of cells that are occupied by the figure.
	std::vector<CellIndeces> previousCells;	// Cells that were occupied on the previous step.

	// Game drawing related.
	float flAngleFrustum;
	float flZaxisDistance;			// Distance from which we view the scene
	float flLengthUnit;				// Unit of measurement of length. The smaller - the closer we see the scene.

	// View rotation with mouse.
	float xViewOld;					// Vars for rotate view.
	float yViewOld;
	float angleViewY;				// Angle to rotate the view around Y.
	float angleViewZ;				// Rotate view around Z.
	float angleViewX;				// Rotate view around X.
	float flScaleAll;				// Zooming factor.
	float angleViewYMax;

	RoundParamsVector roundParams;
	std::size_t iCurRound;
	bool bPlaySound;
	bool bKeyDown;					// Prevent from multiple events while pressing one key.

	// Sizes/other parameters.
	float flScaleMax;				// Limit scaling the scene.
	float flScaleMin;
	float flDeltaAngleViewZ;
	float flDeltaScale;				// Scaling factor increment for keyboard handling.
	int iShowMessageCount;			// How many frames the msg "You lost!" is shown.
	int iMaxShowMessageCount;		// How many frames the msg "You lost!" is shown.

	// Members for optimized text output.
	int iNumOfStringTextures;
	// Enum hack - contains all the symbols we want to render on the scene.	
	enum{ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE,
		TEN, ELEVEN, TWELVE, THIRTEEN, FOURTEEN, FIFTEEN,
		LAYERS, NEXT_FIGURE, YOU_LOST, YOU_WON};
	// String texture array created in constructor for optimized string output.
	std::vector<std::tr1::shared_ptr<TEXTURE> > stringTextures;
	void loadStringTextures();
};

#endif // SDL_SCREEN