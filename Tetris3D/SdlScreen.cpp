// SdlScreen.cpp - implementation of the SdlScreen class and all its children.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
#include "SdlScreen.h"

//----------------------------------
//  SdlScreen implementation.
SdlScreen::SdlScreen(int idExt, float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
		FMOD::System *sys, std::vector<FMOD::Sound*> snd) :
		id(idExt), flWidth(w), flHeight(h), surfaceOnWhichWeRender(s),
		textures(t), fonts(fnt), system(sys), sounds(snd)
{
	 bLeftMouseButton = false;	// Left button of the mouse is unpressed by default.
}

SdlScreen::~SdlScreen(void)
{
}

void SdlScreen::setScreenSize(float w, float h)
{
	flWidth = w;
	flHeight = h;
}

// Unoptimized version - used in ButtonScreen and its offsprings.
int SdlScreen::drawText(const std::string &txt, GLfloat x, GLfloat y, GLfloat w, GLfloat h,
	TTF_Font *font, Logic &logic, SDL_Color textColor)
{
	int mode;
	SDL_Surface *text;
	GLuint textTexture;
	
	text = TTF_RenderText_Blended(font, txt.c_str(), textColor);// Get text surface.
	if (!text ) {
        std::cerr << "Could not create a string surface: " << txt << std::endl;
        exit(1);
    }
	// Check color format to use in the texture.
	if (text->format->BytesPerPixel == 3) {		// RGB 24bit.
		mode = GL_RGB;
    } 
    else if (text->format->BytesPerPixel == 4) {// RGBA 32bit. 
        mode = GL_RGBA;
    } 
    else {
		std::cerr << "Could not determine pixel format of the surface." << std::endl;
        SDL_FreeSurface(text);
        exit(1);
    }

	// Draw the surface onto the OGL screen using textures.
    glPushMatrix();
	glDisable(GL_LIGHTING);

    glGenTextures(1, &textTexture);
    glBindTexture(GL_TEXTURE_2D, textTexture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create texture bound to the text surface.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text->w, text->h, 
		0, mode, GL_UNSIGNED_BYTE, text->pixels );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTranslatef(x, y, 0.0f);	// Move the texture where it should be.

	// Draw.
    glBegin( GL_QUADS );
	glTexCoord3d(0.0, 1.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord3d(1.0, 1.0, 0.0); glVertex3d(w, 0.0, 0.0);
	glTexCoord3d(1.0, 0.0, 0.0); glVertex3d(w, h, 0.0);
	glTexCoord3d(0.0, 0.0, 0.0); glVertex3d(0.0, h, 0.0);	
	glEnd();
	
	glEnable(GL_LIGHTING);

	glPopMatrix();

    SDL_FreeSurface(text);
    glDeleteTextures(1, &textTexture);

	return 0;
}

void SdlScreen::drawBackgroundTexture(int id, float z)
{
	glPushMatrix();
	glMatrixMode (GL_PROJECTION);	// Switch to the projection matrix.
	glLoadIdentity ();				// Clean up the projection matrix.

	glMatrixMode (GL_MODELVIEW);	// Switch to the modelview matrix.
	glLoadIdentity ();				// Clean up the modelview matrix.

	glDisable(GL_LIGHTING);
	glColor3f(1., 1., 1.);			// White light.
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textures[id]->id);
	
	glTranslatef(-1., -1., z);		// The coordinate of the bottom left corner of the OGL.
	glBegin( GL_QUADS );
	glTexCoord3d(0.0, 1.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord3d(1.0, 1.0, 0.0); glVertex3d(2., 0.0, 0.0);
	glTexCoord3d(1.0, 0.0, 0.0); glVertex3d(2., 2., 0.0);
	glTexCoord3d(0.0, 0.0, 0.0); glVertex3d(0.0, 2., 0.0);	
	glEnd();

	glDisable( GL_TEXTURE_2D );
	glEnable(GL_LIGHTING);

	glPopMatrix();
}

void SdlScreen::doInput(Logic &logic, SDL_Event sdlEvent)
{    
    switch(sdlEvent.type)
    {
	case SDL_MOUSEBUTTONDOWN:
		bLeftMouseButton = true;	// Prevent button captions from flickering.
		handleMouseButtonDown(sdlEvent, logic);
		break;

	case SDL_MOUSEBUTTONUP:
		if(bLeftMouseButton){		// Prevent button captions from flickering.
			bLeftMouseButton = false;
			handleMouseButtonUp(sdlEvent, logic);	
		}
		break;

	case SDL_MOUSEMOTION:
		handleMouseMotion(sdlEvent, logic);
		break;
	
	case SDL_KEYDOWN:
		handleKeyDown(sdlEvent, logic);
		break;

	case SDL_KEYUP:
		handleKeyUp(sdlEvent, logic);
		break;

	case SDL_VIDEORESIZE:
		handleResize(sdlEvent, logic);
		break;			

	case SDL_QUIT:
		logic.bAppRunning = false;
		break;
	}
}

void SdlScreen::handleResize(const SDL_Event& sdle, Logic &l)
{
	glViewport (0, 0, (GLsizei) sdle.resize.w, (GLsizei) sdle.resize.h);
	flWidth = sdle.resize.w;
	flHeight = sdle.resize.h;
	l.flScreenWidth = flWidth;
	l.flScreenHeight = flHeight;
}

// Default implementations of some virtual functions do nothing.
void SdlScreen::handleKeyDown(const SDL_Event& sdle, Logic &l)
{
}

void SdlScreen::handleKeyUp(const SDL_Event& sdle, Logic &l)
{
}

void SdlScreen::handleMouseMotion(const SDL_Event& sdle, Logic &l)
{
}

void SdlScreen::handleMouseButtonDown(const SDL_Event& sdle, Logic &l)
{
}

void SdlScreen::handleMouseButtonUp(const SDL_Event& sdle, Logic &l)
{
}

void SdlScreen::notify(Subject* s)
{
}

//----------------------------------
// ButtonScreen implementation.
ButtonScreen::ButtonScreen(int idExt, float w, float h, SDL_Surface* s, 
	TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
	FMOD::System *sys, std::vector<FMOD::Sound*> snd) :
	SdlScreen(idExt, w, h, s, t, fnt, sys, snd)
{
	bPlayButtonSound = true;
	bMouseOverButton = false;
	bKeyDown = false;
	iPrevFocusButton = -1;
}

ButtonScreen::~ButtonScreen()
{
}

// Press the button under cursor.
int ButtonScreen::pressButton(float x, float y)
{	
	for(map_iter iterator = guiObjects.begin(); iterator != guiObjects.end(); iterator++){
		if( iterator->second->isPressed(x, y) ){
			iterator->second->setPressed(true);
			return iterator->second->getId();
		}
	}

	return -1;
}

// Unpress all the buttons.
void ButtonScreen::setUnpressed()
{	
	for(map_iter iterator = guiObjects.begin(); iterator != guiObjects.end(); iterator++)
		iterator->second->setPressed(false);	
}

// Get the id of the highlighted button.
int ButtonScreen::getHighlightedButton()
{	
	for(map_iter iterator = guiObjects.begin(); iterator != guiObjects.end(); iterator++){
		if( iterator->second->hasFocus() )
			return iterator->second->getId();		
	}

	return -1;
}

// Get the id of the button under cursor.
int ButtonScreen::pickButton(float px, float py)
{	
	for(map_iter iterator = guiObjects.begin(); iterator != guiObjects.end(); iterator++){
		if( iterator->second->isPressed(px, py) ){			
			return iterator->second->getId();
		}
	}

	return -1;
}

// Change logic structure depending on the id parameter of the button.
void ButtonScreen::processButton(std::size_t id, Logic & logic)
{
	switch(id)
	{ 
	case START_BTN:
		logic.bShowStartScreen = false;
		logic.bShowOptionsScreen = false;
		logic.bShowPlayScreen = true;	// Start the game.
		logic.bGamePaused = false;
		logic.bTrain = false;
		logic.notifyObservers();		// Tell registered observers to change their settings.
		break;

	case OPTIONS_BTN:
		logic.bShowStartScreen = false;
		logic.bShowOptionsScreen = true;
		logic.bShowPlayScreen = false;
		logic.bNewOptionsScreen = true;	// Create new buttons with the correct settings.
		logic.notifyObservers();
		break;

	case HOWTO_BTN:
		logic.bShowStartScreen = false;
		logic.bShowOptionsScreen = false;
		logic.bShowHowtoScreen = true;
		break;

	case TRAIN_BTN:
		logic.bShowStartScreen = false;
		logic.bShowOptionsScreen = false;
		logic.bShowPlayScreen = true;
		logic.bGamePaused = false;
		logic.bTrain = true;
		logic.notifyObservers();
		break;

	case BACKGRSND_BTN:					// Switch the background sound On/Off.
		if(logic.bBackgroundSound){
			logic.bBackgroundSound = false;
			((OptionsButton*)guiObjects[id].get())->setCaption(std::string("Off"));
		}
		else{
			logic.bBackgroundSound = true;
			((OptionsButton*)guiObjects[id].get())->setCaption(std::string("On"));
		}
		logic.notifyObservers(); // Tell registered observers to change their settings.
		break;

	case ACTIONSND_BTN:
		if(logic.bActionsSound){
			logic.bActionsSound = false;
			((OptionsButton*)guiObjects[id].get())->setCaption(std::string("Off"));
		}
		else{
			logic.bActionsSound = true;
			((OptionsButton*)guiObjects[id].get())->setCaption(std::string("On"));
		}
		logic.notifyObservers();// Tell registered observers to change their settings.
		break;

	case ROUND_BTN:				// Change the number of cells in x and z directions.
		logic.iSystemSize = logic.iSystemSize % logic.iSystemSizeMax + 2;
		if(logic.iSystemSize < 3)
			logic.iSystemSize = 4;
		((OptionsButton*)guiObjects[id].get())->setCaption(std::to_string((_ULonglong)logic.iSystemSize));
		logic.notifyObservers(); 
		break;

	case BACK_BTN:
		logic.bShowOptionsScreen = false;
		logic.bShowStartScreen = true;
		break;
	}// End switch(id).

	if(bPlayButtonSound)
		::playSound(system, sounds[3], channel);
}

void ButtonScreen::doDrawing(Logic &logic)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffer for new data.

	glPushMatrix();					// Save current matrix.

	glMatrixMode (GL_PROJECTION);	// Switch to the projection matrix.
	glLoadIdentity ();				// Clean up the projection matrix.

	glMatrixMode (GL_MODELVIEW);	// Switch to the modelview matrix.
	glLoadIdentity ();				// Clean up the modelview matrix.

	// Draw the background image.
	drawBackgroundTexture(id, 0.);

	// Draw buttons.
	glEnable(GL_TEXTURE_2D);	
	for(map_iter iterator = guiObjects.begin(); iterator != guiObjects.end(); iterator++)
		iterator->second->draw(fonts[0]);

	// Draw text at the top of the screen.
	std::string txt = "Tetris 3D";
	SDL_Color textColor;
	textColor.b = textColor.g = textColor.r = 0.;
	drawText(txt, -0.92, 0.7, 0.5, 0.23, fonts[0], logic, textColor);

	txt = "(c) Nikolay Prodanov, 2013.";
	drawText(txt, -0.915, 0.64, 0.5, 0.08, fonts[0], logic, textColor);

	glDisable( GL_TEXTURE_2D );
	glFlush();
    glPopMatrix();					// Restore the previous state.
}

void ButtonScreen::handleMouseButtonDown(const SDL_Event& sdle, Logic &logic)
{
	switch(sdle.button.button)
	{
	case SDL_BUTTON_LEFT:
		{
			// Normalize screen coordinates.
			// OpenGL coords (used for rendering) go from -1 to 1 with the origin in the middle of the screen.
			// Clicking coords have (0, 0) in the upper left corner.
			float xnorm = (sdle.button.x - 0.5*flWidth) / (0.5*flWidth);
			float ynorm = (0.5*flHeight - sdle.button.y) / (0.5*flHeight);
			
			pressButton(xnorm, ynorm);
		}
		break;
	}// End switch.
}

void ButtonScreen::handleMouseButtonUp(const SDL_Event& sdle, Logic &logic)
{
	switch(sdle.button.button)
	{
	case SDL_BUTTON_LEFT:
		{
			// Normalize screen coordinates.
			// OpenGL coords (used for rendering) go from -1 to 1 with the origin in the middle.
			// Clicking coords have (0, 0) in the upper left corner.
			float xnorm = (sdle.button.x - 0.5*flWidth) / (0.5*flWidth);
			float ynorm = (0.5*flHeight - sdle.button.y) / (0.5*flHeight);
			
			int btnId = pickButton(xnorm, ynorm);
			if(btnId >= 0){
				guiObjects[btnId]->setPressed(false);	// Unpress button.
				processButton(btnId, logic);			// Perform actions depending on btn id.
			}
		}
		break;
	}// End switch.
	setUnpressed();
}

void ButtonScreen::handleMouseMotion(const SDL_Event& sdle, Logic &logic)
{
	if(bMouseOverButton)
		bMouseOverButton = false;
	else {
		bMouseOverButton = true;		
		float xnorm = (sdle.button.x - 0.5*flWidth) / (0.5*flWidth);
		float ynorm = (0.5*flHeight - sdle.button.y) / (0.5*flHeight);

		int btnId = pickButton(xnorm,ynorm);
		// Handle mouse highlighting (take into account keyboard management of the buttons).
		if( (btnId >= 0) && (btnId != iPrevFocusButton) && (iPrevFocusButton >= 0) ){
			// Unset previously highlighted buttons.
			guiObjects[getHighlightedButton()]->setFocus(false);
			guiObjects[iPrevFocusButton]->setFocus(false);
			// Set new current highlighted button.
			guiObjects[btnId]->setFocus(true);
			iPrevFocusButton = btnId;
		}
	}
}

void ButtonScreen::handleKeyDown(const SDL_Event& sdle, Logic &logic)
{	
	bKeyDown = true;

 	switch(sdle.key.keysym.sym)
	{
	case SDLK_RETURN:		// Press the highlighted button.
		{
			int btnId = getHighlightedButton();			
			if(btnId >= 0)
				guiObjects[btnId]->setPressed(true);
		}
		break;
	}
}

void ButtonScreen::handleKeyUp(const SDL_Event& sdle, Logic &logic)
{	
	map_iter iter;

 	switch(sdle.key.keysym.sym)
	{
	case SDLK_RETURN:		// Press the highlighted button.
		if(bKeyDown)
		{
			int btnId = getHighlightedButton();
			if( btnId >= 0 ){
				guiObjects[btnId]->setPressed(false);
				processButton(btnId, logic);
				bKeyDown = false;
			}
			bKeyDown = false;
		}
		break;

	case SDLK_UP:			// Move button highlighting up.
		if(bKeyDown)
		{
			int btnId = getHighlightedButton();
			if( btnId >= 0 ){
				iter = guiObjects.find(btnId);	// Get iterator to the current element.
				iter->second->setFocus(false);

				if( iter == guiObjects.begin() )
					iter = --(guiObjects.end());
				else					
					--iter;
				iter->second->setFocus(true);
			}
			bKeyDown = false;
		}
		break;

	case SDLK_DOWN:				// Move button highlighting down.
		if(bKeyDown)
		{
			int btnId = getHighlightedButton();
			if( btnId >= 0 ){
				iter = guiObjects.find(btnId);	// Get iterator to the current element.
				iter->second->setFocus(false);

				if( iter == --(guiObjects.end()) )
					iter = guiObjects.begin();
				else					
					++iter;
				iter->second->setFocus(true);
			}
			bKeyDown = false;
		}
		break;
	}
}

void ButtonScreen::notify(Subject* s)
{
	bPlayButtonSound = ((Logic*) s)->bActionsSound;
}

//----------------------------------
// StartScreen implementation.
// In StartScreen buttons are added only once, during the creation of the object.
StartScreen::StartScreen(int idExt, float w, float h, SDL_Surface* s, 
	TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
	FMOD::System *sys, std::vector<FMOD::Sound*> snd):
	ButtonScreen(idExt, w, h, s, t, fnt, sys, snd)
{
	addButtons();
	bLeftMouseButton = false;
	iPrevFocusButton = START_BTN;
}

StartScreen::~StartScreen()
{
}

void StartScreen::addButtons()
{
	float x = -0.5;
	float y = 0.3;
	float w = 1.;
	float h = 0.25;
	std::string name = "Start";

	try {
		// Button 0.
		GuiObject* pGuiObj = new Button(x, y, w, h, START_BTN, name, textures[0]->id);	
		pGuiObj->setFocus(true);
		guiObjects.insert(std::make_pair(START_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));

		// Button 1.
		y -= 1.5*h;
		name = "Options";
		pGuiObj = new Button(x, y, w, h, OPTIONS_BTN, name, textures[0]->id);	// The same texture id.
		guiObjects.insert(std::make_pair(OPTIONS_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));

		// Button 2.
		y -= 1.5*h;
		name = "How to play";
		pGuiObj = new Button(x, y, w, h, HOWTO_BTN, name, textures[0]->id);	// The same texture id.
		guiObjects.insert(std::make_pair(HOWTO_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));

		// Button 3.
		y -= 1.5*h;
		name = "Training";
		pGuiObj = new Button(x, y, w, h, TRAIN_BTN, name, textures[0]->id);	// The same texture id.
		guiObjects.insert(std::make_pair(TRAIN_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));
	} // End try.
	catch(std::bad_alloc& ba) {
		std::cerr << "Failed to create one of the buttons in StartScreen: memory error, " + std::string(ba.what()) << std::endl;		
		exit(1);
	}
}

//----------------------------------
// OptionsScreen implementation.
// In OptionsScreen buttons are added every time this screen is shown, not just during creation.
OptionsScreen::OptionsScreen(int idExt, float w, float h, SDL_Surface* s, 
	TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
	FMOD::System *sys, std::vector<FMOD::Sound*> snd, Logic &logic) :
	ButtonScreen(idExt, w, h, s, t, fnt, sys, snd)
{
	setupNewScreen(logic);			// Create new buttons to use the up-to-date logic values.
}

OptionsScreen::~OptionsScreen()
{
}

void OptionsScreen::setupNewScreen(Logic &logic)
{
	bLeftMouseButton = false;
	addButtons(logic);	
	iPrevFocusButton = BACKGRSND_BTN;
}

void OptionsScreen::addButtons(Logic &logic)
{
	// 'magic numbers'.
	float x = -0.5;
	float y = 0.3;
	float w = 1.;
	float h = 0.25;
	std::string name = "Background Sound";
	std::string caption = " ";

	guiObjects.clear();				// Empty the map of buttons.
	
	try {
		// Button 0.
		if(logic.bBackgroundSound)
			caption = "On";
		else 
			caption = "Off";
		GuiObject* pGuiObj = new OptionsButton(x, y, w, h, 
			BACKGRSND_BTN, name, textures[0]->id, caption);
		pGuiObj->setFocus(true);
		guiObjects.insert(std::make_pair(BACKGRSND_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));

		// Button 1.
		y -= 1.5*h;
		name = "Actions Sound";
		if(logic.bActionsSound)
			caption = "On";
		else 
			caption = "Off";
		pGuiObj = new OptionsButton(x, y, w, h, 
			ACTIONSND_BTN, name, textures[0]->id, caption);	// The same texture id.
		guiObjects.insert(std::make_pair(ACTIONSND_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));

		// Button 2.
		y -= 1.5*h;
		name = "Lateral Size";
		caption = std::to_string((_ULonglong)logic.iSystemSize);
		pGuiObj = new OptionsButton(x, y, w, h, 
			ROUND_BTN, name, textures[0]->id, caption);	// The same texture id.
		guiObjects.insert(std::make_pair(ROUND_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));

		// Button 3.
		y -= 1.5*h;
		name = "Back";
		pGuiObj = new Button(x, y, w, h, BACK_BTN, name, textures[0]->id);	// The same texture id.
		guiObjects.insert(std::make_pair(BACK_BTN, std::tr1::shared_ptr<GuiObject>(pGuiObj)));
	}// End try.
	catch(std::bad_alloc& ba) {
		std::cerr << "Failed to create one of the buttons in OptionsScreen: memory error, " + std::string(ba.what()) << std::endl;		
		exit(1);
	}
}

//----------------------------------
// HowtoScreen implementation.
HowtoScreen::HowtoScreen(int idExt, float w, float h, SDL_Surface* s, 
	TEXTURE_PTR_ARRAY t, TTF_Font** fnt,
	FMOD::System *sys, std::vector<FMOD::Sound*> snd) :
	ButtonScreen(idExt, w, h, s, t, fnt, sys, snd)
{
	bLeftMouseButton = false;
}

HowtoScreen::~HowtoScreen()
{
}

void HowtoScreen::doDrawing(Logic &logic)
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffer for new data.

	glPushMatrix();					// Save current matrix.
	glMatrixMode (GL_PROJECTION);	// Switch to the projection matrix.
	glLoadIdentity ();				// Clean up the projection matrix.
	glMatrixMode (GL_MODELVIEW);	// Switch to the modelview matrix.
	glLoadIdentity ();				// Clean up the modelview matrix.

	// Draw the background image.
	drawBackgroundTexture(id, 0.);

	// Text.
	glEnable( GL_TEXTURE_2D );
	
	SDL_Color textColor;
	textColor.r = 0;
	textColor.g = 0.;
	textColor.b = 0.;
	// 'magic numbers'.
	std::string txt = "Keys (click or space to exit from here)";
	drawText(txt, -0.55, 0.78, 1.1, 0.15, fonts[0], logic, textColor);
	txt = "_______________________________________";
	drawText(txt, -0.55, 0.76, 1.1, 0.15, fonts[0], logic, textColor);

	// 'magic numbers' are below.
	float y = 0.58;
	float h = 0.15;
	float txtX = -0.92;
	float fixedX = 0.25;
	// Keys.
	txt = "Action";
	drawText(txt, txtX, y, 0.2, h, fonts[0], logic, textColor);
	txt = "Key(s)/Mouse";
	drawText(txt, fixedX, y, 0.42, h, fonts[0], logic, textColor);

	SDL_Delay(20);		// Delay to diminish processor load.

	y -= 0.2*h;
	txt = "______";
	drawText(txt, txtX, y, 0.2, h, fonts[0], logic, textColor);
	txt = "____________";
	drawText(txt, fixedX, y, 0.42, h, fonts[0], logic, textColor);	

	y -= 1.3*h;
	txt = "go back to the start screen:";
	drawText(txt, txtX, y, 0.77, h, fonts[0], logic, textColor);
	txt = "b";
	drawText(txt, fixedX, y, 0.043, h, fonts[0], logic, textColor);

	SDL_Delay(20);		// Delay to diminish processor load.

	y -= 1.3*h;
	txt = "pause/unpause:";
	drawText(txt, txtX, y, 0.45, h, fonts[0], logic, textColor);
	txt = "v/space";
	drawText(txt, fixedX, y, 0.23, h, fonts[0], logic, textColor);

	y -= 1.3*h;
	txt = "rotate figure around x(red)/y/z(blue) axes:";
	drawText(txt, txtX, y, 1.14, h, fonts[0], logic, textColor);
	txt = "d/s/a";
	drawText(txt, fixedX, y, 0.16, h, fonts[0], logic, textColor);

	y -= 1.3*h;
	txt = "move figure along x,y,z axes:";
	drawText(txt, txtX, y, 0.85, h, fonts[0], logic, textColor);
	txt = "l/r arrow, shift, b/u arrow";
	drawText(txt, fixedX, y, 0.7, h, fonts[0], logic, textColor);

	SDL_Delay(20);		// Delay to diminish processor load.

	y -= 1.3*h;
	txt = "reset view to the initial position:";
	drawText(txt, txtX, y, 1., h, fonts[0], logic, textColor);
	txt = "=";
	drawText(txt, fixedX, y, 0.05, h, fonts[0], logic, textColor);

	y -= 1.3*h;
	txt = "zoom:";
	drawText(txt, txtX, y, 0.18, h, fonts[0], logic, textColor);
	txt = "m/p";
	drawText(txt, fixedX, y, 0.11, h, fonts[0], logic, textColor);

	SDL_Delay(20);		// Delay to diminish processor load.

	y -= 1.3*h;
	txt = "rotate view around x,y/z axes:";
	drawText(txt, txtX, y, 0.9, h, fonts[0], logic, textColor);
	txt = "mouse(left)/wheel";
	drawText(txt, fixedX, y, 0.51, h, fonts[0], logic, textColor);
	
	txt = "(c) Nikolay Prodanov, 2013. All rights reserved.";
	drawText(txt, -0.5, -0.96, 1., 0.11, fonts[0], logic, textColor);

	SDL_Delay(20);		// Delay to diminish processor load.

	glFlush();
	glDisable( GL_TEXTURE_2D );
    glPopMatrix();					// Restore the previous state.
}

void HowtoScreen::handleKeyDown(const SDL_Event& sdle, Logic &logic)
{	
	// Check keyboard.
 	switch(sdle.key.keysym.sym)
	{
	case SDLK_RETURN:				// Fall through.
	case SDLK_SPACE:
		logic.bShowStartScreen = true;
		logic.bShowHowtoScreen = false;
		break;
	}
}

void HowtoScreen::handleMouseButtonUp(const SDL_Event& sdle, Logic &logic)
{
	switch(sdle.button.button)
	{
	case SDL_BUTTON_LEFT:
		logic.bShowStartScreen = true;
		logic.bShowHowtoScreen = false;
		break;

	case SDL_QUIT:
		logic.bAppRunning = false;
		break;
	}
}

//----------------------------------
// PlayScreen implementation.
PlayScreen::PlayScreen(int idExt, float w, float h, SDL_Surface* s, TEXTURE_PTR_ARRAY t, TTF_Font** fnt, 
	 FMOD::System *sys,std::vector<FMOD::Sound*> snd,
	 std::vector<std::tr1::shared_ptr<RoundParameters> > & rp) :
	 SdlScreen(idExt, w, h, s, t, fnt, sys, snd), roundParams(rp)
{
	iShowMessageCount = 0;
	iMaxShowMessageCount = 200;
	flDeltaAngleViewZ = 3.;		// Mouse wheel rotation.
	flDeltaScale = 0.01;
	bKeyDown = false;

	// Optimize the string output - load string textures only once and then reuse them.
	loadStringTextures();
}

PlayScreen::~PlayScreen()
{
	// Cleanup textures.
	std::size_t i;
	for(i = 0; i < stringTextures.size(); i++)
		glDeleteTextures(1, &(stringTextures[i].get()->id));
}

void PlayScreen::initMembers(const Logic &logic)
{
	// 'magic numbers'.
	iNumOfPlanes = 12;
	iNumOfCellsX = logic.iSystemSize;	// Get lateral (xz) size (number of cells) from options.
	iNumOfCellsZ = logic.iSystemSize;	// Get lateral size from options.
	cubeSize = 1.;
	bottomPlane = 0;
	secondBottomPlane = 0;
	iLowestCurrentPlane = 0;
	createPlanes();						// Create planes of cells.

	// Get a fresh set of cells that keep track of fixed cubes.
	fixedCubes = 
		std::tr1::shared_ptr<FixedCubes>( new FixedCubes(iNumOfPlanes, iNumOfCellsX, iNumOfCellsZ, cubeSize) );

	cubesPerFigure = 4;						// All the figures consist of 4 cubes.
	currentCells.resize(cubesPerFigure);	// Keep track of 4 cells filled with the figure.
	previousCells.resize(cubesPerFigure);
	iCurrentFigureId = -cubesPerFigure;		// Start from the negative to get 0 at 1st iteration.

	iFrameDelayMove = 200;					// Moving speed.
	bKeyDown = false;						// Keyboard keys are unpressed at the beginning.
	iCurRound = logic.iRound - 1;			// There's only one round.

	// 'magic numbers'.
	xViewOld = 0.;
	yViewOld = 0.;
	angleViewX = 10.;
	angleViewY = 10.;				// Initial angle (around y) of the view.
	angleViewYMax = 10.;			// Final initial angle view.
	angleViewZ = 0.;				// Rotation of the view around z axis.
	flZaxisDistance = 15.f;
	flLengthUnit = 4.f;				// This influences the distance from the camera.
									// The smaller the value, the closer the scene to the camera.
	flScaleAll = 1.;				// Scaling factor.
	flScaleMax = 4.;
	flScaleMin = 0.1;
}

void PlayScreen::doInput(Logic &logic, SDL_Event sdlEvent)
{    
    switch(sdlEvent.type)
    {
	case SDL_MOUSEBUTTONDOWN:
		if(!logic.bGamePaused)
			handleMouseButtonDown(sdlEvent, logic);
		break;

	case SDL_MOUSEBUTTONUP:
		if(!logic.bGamePaused)
			handleMouseButtonUp(sdlEvent, logic);
		break;

	case SDL_MOUSEMOTION:
		if(!logic.bGamePaused)
			handleMouseMotion(sdlEvent, logic);
		break;
	
	case SDL_KEYDOWN:		
		handleKeyDown(sdlEvent, logic);			
		break;

	case SDL_KEYUP:
		bKeyDown = false;
		break;

	case SDL_VIDEORESIZE:
		handleResize(sdlEvent, logic);
		break;			

	case SDL_QUIT:
		logic.bAppRunning = false;
		break;
	}
}

void PlayScreen::doDrawing(Logic &logic)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffer for new data.

	// Draw the background image.	
	drawBackgroundTexture(id, 1.);

	// Initialize the screen/scene.
	initResize();
	initView();

	// Draw the axes, grid, fixed cubes, current and next figures.
	drawAxes();
	drawPlanes();			// Grid of cells, some of which are highlighted.
	fixedCubes->draw();
	currentFigure->draw();
	nextFigure->drawAsNext();

	// Draw the score.
	drawScoreAndRoundOptimized(logic);

	glFlush();
}

void PlayScreen::handleMouseButtonDown(const SDL_Event& sdle, Logic &logic)
{
	int objName = 0;
	switch(sdle.button.button)
	{
	case SDL_BUTTON_LEFT:
		xViewOld = sdle.button.x;
		yViewOld = sdle.button.y;
		break;

	case SDL_BUTTON_WHEELUP:
		angleViewZ -= flDeltaAngleViewZ;			// Rotate around the horizontal axis.
		break;

	case SDL_BUTTON_WHEELDOWN:
		angleViewZ += flDeltaAngleViewZ;
		break;
	}// End switch.
}

void PlayScreen::handleMouseMotion(const SDL_Event& sdle, Logic &logic)
{
	switch(sdle.motion.state)
	{		
	case SDL_BUTTON_LEFT:		
		angleViewY += sdle.motion.x - xViewOld;		// Rotate view.			
		xViewOld = sdle.motion.x;
		angleViewX += sdle.motion.y - yViewOld;
		yViewOld = sdle.motion.y;		
		break;
	}
}

void PlayScreen::handleKeyDown(const SDL_Event& sdle, Logic &logic)
{	
	vector_3d dr;
	// Check keyboard.
 	switch(sdle.key.keysym.sym)
	{
	case SDLK_EQUALS:				// Reset view.
		angleViewY = angleViewYMax;
		angleViewZ = 0.;
		angleViewX = 10.;
		flScaleAll = 1.;
		break;

	case SDLK_b:					// Go back to the StartScreen.
		if(!logic.bShowStartScreen) {
			logic.bShowStartScreen = true;
			logic.bNewRound = true;		// Start from scratch.
		}
		logic.notifyObservers();		// Tell observers to change their sound behavior.
		break;

	case SDLK_p:					// Zoom in.
		if(flScaleAll < flScaleMax)
			flScaleAll += flDeltaScale;
		break;

	case SDLK_m:					// Zoom out.
		if(flScaleAll > flScaleMin)
			flScaleAll -= flDeltaScale;
		break;

	case SDLK_v:					// Pause.
		logic.bGamePaused = true;
		logic.bNewRound = false;
		logic.notifyObservers();
		break;

	case SDLK_SPACE:				// Unpause.
		logic.bGamePaused = false;
		logic.notifyObservers();
		break;

	case SDLK_ESCAPE:				// Quit.
		logic.bAppRunning = false;
		break;		

		// Rotation.
		// xy-plane, around z-axis.
	case SDLK_a:
		if(!bKeyDown){				// Prevent from processing multiple times at one pressing.
			bKeyDown = true;
			currentFigure->rotateZ( plane );
			if(bPlaySound)
				::playSound(system, sounds[4], channel);
		}
		break;

		// xz-plane, around y-axis.
	case SDLK_s:
		if(!bKeyDown){				// Prevent from processing multiple times at one pressing.
			bKeyDown = true;
			currentFigure->rotateY( plane );
			if(bPlaySound)
				::playSound(system, sounds[4], channel);
		}
		break;

	case SDLK_d:
		if(!bKeyDown){				// Prevent from processing multiple times at one pressing.
			bKeyDown = true;
			currentFigure->rotateX( plane );
			if(bPlaySound)
				::playSound(system, sounds[4], channel);
		}
		break;

	case SDLK_RSHIFT:				// Accelerate the vertical movement of the figure.
	case SDLK_LSHIFT:				// Fall through.
		if(!bKeyDown){				// Prevent from processing multiple times at one pressing.
			bKeyDown = true;
			int factor = -1;
			currentFigure->moveY(factor, plane);
		}
		break;

	case SDLK_LEFT:
		if(!bKeyDown){			// Prevent from processing multiple times at one pressing.
			bKeyDown = true;
			int factor = -1;
			currentFigure->moveX(factor, plane);
		}
		break;

	case SDLK_RIGHT:
		if(!bKeyDown){			// Prevent from processing multiple times at one pressing.
			bKeyDown = true;
			int factor = 1;
			currentFigure->moveX(factor, plane);
		}
		break;

	case SDLK_UP:
		if(!bKeyDown){			// Prevent from processing multiple times at one pressing.
			bKeyDown = true;
			int factor = -1;
			currentFigure->moveZ(factor, plane);
		}
		break;

	case SDLK_DOWN:
		if(!bKeyDown){			// Prevent from processing multiple times at one pressing.
			bKeyDown = true;
			int factor = 1;
			currentFigure->moveZ(factor, plane);
		}
		break;
	}
}

void PlayScreen::handleResize(const SDL_Event& sdle, Logic &logic)
{
	glViewport (0, 0, (GLsizei) sdle.resize.w, (GLsizei) sdle.resize.h);
	flWidth = sdle.resize.w;
	flHeight = sdle.resize.h;
	logic.flScreenWidth = flWidth;
	logic.flScreenHeight = flHeight;

	initResize();		// Avoid code duplication with setup matrices.
}

void PlayScreen::initResize()
{
	glMatrixMode (GL_PROJECTION);		// Switch to the projection matrix.
	glLoadIdentity ();					// Clean up the projection matrix.
	gluPerspective(calculateAngle(flLengthUnit, flZaxisDistance), 
		flWidth/flHeight, 1.0, 1024.0);	// Set up the projection matrix with the same units in x and y.
	glMatrixMode (GL_MODELVIEW);		// Switch to the modelview matrix.
	glLoadIdentity ();
}

void PlayScreen::initView()
{
	glLoadIdentity ();             // Clear the current matrix.
	// Move camera to the point (0,0,zdistance) in eye coords, look at point (0,0,0), 
	// camera orientation - the normal is along (0,1,0)
	gluLookAt (0.0, 0.0, flZaxisDistance, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(angleViewX, 1.0f, 0.0f, 0.0f);
	glRotatef(angleViewY, 0.0f, 1.0f, 0.0f);
	glRotatef(angleViewZ, 0.0f, 0.0f, 1.0f);
	glScalef(flScaleAll, flScaleAll,flScaleAll);
}

void PlayScreen::drawAxes() const
{
	glPushMatrix();

	glTranslatef(0.75*cubeSize*iNumOfCellsX, 0., 0.5*cubeSize*iNumOfCellsX);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	// x, red
	glColor3f(2.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.25*cubeSize*iNumOfCellsX, 0.f, 0.f);
	// y, green
	glColor3f(0.f, 2.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 0.25*cubeSize*iNumOfPlanes, 0.f);
	// z, blue
	glColor3f(0.f, 0.f, 2.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 0.25*cubeSize*iNumOfCellsZ);
	glEnd();

	glEnable(GL_LIGHTING);

	glPopMatrix();
}

void PlayScreen::doLogic(Logic &logic)
{	
	manageCellsFilling();	// Manage highlighted/unhighlighted cells during drawing.

	// Note: both previous and current highlighted cells are reset if collision occurs.
	checkCollision(logic);	// Figure collision with the fixed cubes/bottom plane.
}

// All the game actions are managed in play() function.
void PlayScreen::play(Logic &logic, SDL_Event sdlEvent)
{
	// New round: resize the screen, change round parameters.
	if(logic.bNewRound){
		logic.bNewRound = false;		
		setScreenSize(logic.flScreenWidth, logic.flScreenHeight);
		initResize();
		// The first current and next figures are created in the setupNewRound().
		setupNewRound(logic);
	}

	if( logic.bNewFigure ){
		logic.bNewFigure = false;
		currentFigure = nextFigure;		// Make the next figure to be the current figure.
		nextFigure = createNewFigure();	// Create the new next figure.

		logic.bRoundFinished = isRoundFinished(logic);
	}

	// Draw the scene.
	doDrawing(logic);	// It is before input/logic to be sure that the figure always exists.
	
	// Process input if the round started and is going on.
	if(!logic.bRoundFinished){
		if(!logic.bGamePaused)
			dropFigure(logic);				// Falling of the Figure with constant velocity.
		doInput(logic, sdlEvent);
		if(!logic.bGamePaused)			// Do logic if the game is not paused.
			doLogic(logic);
	}

	// If round finished, show a msg about whether the user has lost/won the game.
	if(logic.bRoundFinished){
		if(iShowMessageCount < iMaxShowMessageCount)// Show the msg for iMaxShowMessageCount frames.
			++iShowMessageCount;
		else{
			iShowMessageCount = 0;
			logic.bRoundFinished = false;
			logic.bNewRound = true;
		}
	}	
}

void PlayScreen::setupNewRound(Logic &logic)
{	
	// Reset scores.
	logic.iUserScore = 0;

	initMembers(logic);
	logic.notifyObservers();			// Tell the GameApp to use a new round sound.

	// Create the current and the next figures.
	currentFigure = createNewFigure();
	nextFigure = createNewFigure();
}

// Observer pattern method.
void PlayScreen::notify(Subject* s) 
{
	bPlaySound = ((Logic*) s)->bActionsSound;
}

std::tr1::shared_ptr<TEXTURE> PlayScreen::createStringTexture(const std::string &txt, 
	TTF_Font *font, SDL_Color textColor)
{
    SDL_Surface *text;
    GLuint textureid;
    int mode;

	text = TTF_RenderText_Blended(font, txt.c_str(), textColor);
	if (!text ) {
        std::cerr << "Could not create a string surface: " << txt << std::endl;
        exit(1);
    }
	if (text->format->BytesPerPixel == 3) {		// RGB 24bit.
		mode = GL_RGB;
    } 
    else if (text->format->BytesPerPixel == 4) {// RGBA 32bit. 
        mode = GL_RGBA;
    } 
    else {
		std::cerr << "Could not determine pixel format of the surface." << std::endl;
        SDL_FreeSurface(text);
        exit(1);
    }   

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Create one texture name.
    glGenTextures(1, &textureid);

    // Tell OGL to use the generated texture name.
    glBindTexture(GL_TEXTURE_2D, textureid);

    // Read from the SDL surface and put it into an opengl texture.
	gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, text->w, text->h,
                       mode, GL_UNSIGNED_BYTE, text->pixels );

    // Allocate memory for TEXTURE structure and fill it up.
	std::tr1::shared_ptr<TEXTURE> texture(new TEXTURE());
    if (!texture.get()) {
        std::cerr << "Could not create TEXTURE struct for string " << txt << std::endl;
		exit(1);
    }
    
    // Set up TEXTURE structure.
    texture->width = text->w;
    texture->height = text->h;
    texture->id = textureid;    
    
    SDL_FreeSurface(text);
    
    return texture;
}

void PlayScreen::drawStringTexture(int symbolIdx, float x, float y, float z, 
	float w, float h, float angley)
{
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, stringTextures[symbolIdx]->id);
	
	glTranslatef(x, y, z);					// The coordinate where we place the text.
	glRotatef(angley, 0.0f, 1.0f, 0.0f);	// Rotate current string.
	glBegin( GL_QUADS );
	glTexCoord3d(0.0, 1.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord3d(1.0, 1.0, 0.0); glVertex3d(w, 0.0, 0.0);
	glTexCoord3d(1.0, 0.0, 0.0); glVertex3d(w, h, 0.0);
	glTexCoord3d(0.0, 0.0, 0.0); glVertex3d(0.0, h, 0.0);	
	glEnd();

	glDisable( GL_TEXTURE_2D );
	glEnable(GL_LIGHTING);

	glPopMatrix();
}

// Create textures for all the strings that are going to be rendered onto the scene.
void PlayScreen::loadStringTextures()
{
	iNumOfStringTextures = 20;
	SDL_Color textColor;
	textColor.r = 0;
	textColor.g = 0.;
	textColor.b = 1.;
	stringTextures.resize(iNumOfStringTextures);
	
	stringTextures[ZERO] = createStringTexture(std::string("0"), fonts[0], textColor);
	stringTextures[ONE] = createStringTexture(std::string("1"), fonts[0], textColor);
	stringTextures[TWO] = createStringTexture(std::string("2"), fonts[0], textColor);
	stringTextures[THREE] = createStringTexture(std::string("3"), fonts[0], textColor);
	stringTextures[FOUR] = createStringTexture(std::string("4"), fonts[0], textColor);
	stringTextures[FIVE] = createStringTexture(std::string("5"), fonts[0], textColor);
	stringTextures[SIX] = createStringTexture(std::string("6"), fonts[0], textColor);
	stringTextures[SEVEN] = createStringTexture(std::string("7"), fonts[0], textColor);
	stringTextures[EIGHT] = createStringTexture(std::string("8"), fonts[0], textColor);
	stringTextures[NINE] = createStringTexture(std::string("9"), fonts[0], textColor);
	stringTextures[TEN] = createStringTexture(std::string("10"), fonts[0], textColor);
	stringTextures[ELEVEN] = createStringTexture(std::string("11"), fonts[0], textColor);
	stringTextures[TWELVE] = createStringTexture(std::string("12"), fonts[0], textColor);
	stringTextures[THIRTEEN] = createStringTexture(std::string("13"), fonts[0], textColor);
	stringTextures[FOURTEEN] = createStringTexture(std::string("14"), fonts[0], textColor);
	stringTextures[FIFTEEN] = createStringTexture(std::string("15"), fonts[0], textColor);
	
	stringTextures[LAYERS] = createStringTexture(std::string("Layers:"), fonts[0], textColor);
	stringTextures[NEXT_FIGURE] = createStringTexture(std::string("Next Figure:"), fonts[0], textColor);
	stringTextures[YOU_LOST] = createStringTexture(std::string("You lost!"), fonts[0], textColor);
	stringTextures[YOU_WON] = createStringTexture(std::string("You won!"), fonts[0], textColor);
}

void PlayScreen::drawScoreAndRoundOptimized(Logic &logic)
{
	float txtW = 0.7;
	float txtH = 1.1;
	float wordW = 5.5*txtW;
	float x0 = -cubeSize*(0.5*iNumOfCellsX + 5.5*txtW + 3);
	float y0 = 0.4*cubeSize*iNumOfPlanes;
	float x, y;
	float z = 0.;
	float angleY = 0.;

	glPushMatrix();
	glRotatef(angleY, 0.0f, 1.0f, 0.0f);	// Rotate all the string by the same amount.

	// Annihilated layers.
	drawStringTexture(LAYERS, x0, y0, z, 5.*txtW, txtH, 0.);
	if(logic.iUserScore <= FIFTEEN){	// Ensure the secure bounds of the vector.
		x = x0 + 5.5*txtW;
		y = y0;
		drawStringTexture(logic.iUserScore, x, y, z, txtW, txtH, 0.);
	}

	// Next figure or won/lost.
	x = x0;
	y = y0 - 1.5*txtH;	
	if(!logic.bRoundFinished)
		drawStringTexture(NEXT_FIGURE, x, y, z, 8.*txtW, txtH, 0.);	
	else{
		if(logic.iUserScore < logic.iMaxScore)
			drawStringTexture(YOU_LOST, x, y, z, 6.*txtW, 1.4*txtH, 0.);
		else
			drawStringTexture(YOU_WON, x, y, z, 6.*txtW, 1.4*txtH, 0.);
	}

	glPopMatrix();
}

void PlayScreen::manageCellsFilling()
{
	previousCells = currentCells;	// Save currently highlighted cells to the previous cells.
	currentFigure->getCubeIndeces(currentCells);// Define what cells should be highlighted.

	// Find the smallest index of the plane currently filled with the Figure's cube.
	iLowestCurrentPlane = currentCells[0].plane;
	int size = currentCells.size();
	for(int i = 0; i < size; ++i)
		iLowestCurrentPlane = min(iLowestCurrentPlane, currentCells[i].plane);

	resetHighlightedCells(previousCells);	// At first unset the previous cells.
	
	setHighlightedCells(currentCells);		// Then set the filled cells.
}

void PlayScreen::checkCollision(Logic &logic)
{
	// Look at the planes which lie below the currently filled cells.
	// If there is at least one filled cell, then collision occurred.
	std::size_t i;
	int iPlaneBelow;

	// Loop over the planes that are below the currently filled cells.
	for(i = 0; i < currentCells.size(); ++i){
		iPlaneBelow = currentCells[i].plane - 1;// Get the plane under the current cell.
		// If collision occurred.
		if( (iPlaneBelow < 0) || // Bottom plane.
			( (iPlaneBelow >= 0) && (iPlaneBelow < iNumOfPlanes) &&
			plane[iPlaneBelow]->isCellFilled(currentCells[i].x, currentCells[i].z) )
			)
		{
			// Make collision sound.
			if(bPlaySound)
				::playSound(system, sounds[3], channel);

			// Add fixed cubes and fill in the corresponding cells.
			addCubes();

			// Delete the figure.
			currentFigure.reset();
			// Set the flag that says that there is a need for the new figure. 
			logic.bNewFigure = true;
			// Reset the current highlighted cells, don't show them for the deleted figure.
			resetHighlightedCells(currentCells);			

			break;
		}
	}// End for i.

	annihilateLayers(logic);				// Remove completely filled planes of cubes.
}

void PlayScreen::annihilateLayers(Logic& logic)
{
	int index;
	int emptyValue = -1;
	std::size_t i = 0;
	while(i < plane.size()){
		if( plane[i]->isFilled() ){
			// Increase the score - the number of planes annihilated so far.
			++logic.iUserScore;
			// Annihilate the plane.
			// Get indeces of the cubes in this plane and delete these cubes.
			for(int ix = 0; ix < iNumOfCellsX; ix++){
				for(int iz = 0; iz < iNumOfCellsZ; iz++){
					index = plane[i]->getCubeIndex(ix, iz);
					// Remove the cube
					if(index >= 0)
						fixedCubes->eraseCube(index);
					// Say that the cell is empty.
					plane[i]->fillCell(ix, iz, emptyValue);
					// Reset all the highlighted cubes in the bottommost plane.
					plane[0]->resetHighlightedCell(ix, iz);
					if(bPlaySound)
						::playSound(system, sounds[2], channel);
#ifdef _DEBUG
					std::cerr << "Plane " << i << " has been annihilated." << std::endl;
#endif
				}
			}// End annihilate the plane.	

			// Find the first empty plane above the annihilated plane.
			std::size_t emptyPlaneIndex = 0;
			for(std::size_t j = i+1; j < plane.size(); ++j){
				if( plane[j]->isEmpty() ){
					emptyPlaneIndex = j;
#ifdef _DEBUG
					std::cerr << "Empty plane " << emptyPlaneIndex << std::endl;
#endif
					// Use goto here; break can terminate the outer while loop too. 
					// We don't want this.
					goto label;	
				}
			}
label:

			// Shift all the nonempty planes by one plane to the bottom.
			// This involves 2 steps:
			// 1) copy the content of the cells of the upper plane to the current plane.
			// 2) shift the corresponding cubes; 			
			for(int j = i; j < (int)emptyPlaneIndex; ++j){
				if( (j+1) < iNumOfPlanes )
					plane[j]->copyCells( plane[j+1]->getCells() );// 1) copy cells of the upper plane to the current one.
#ifdef _DEBUG
				std::cerr << "plane " << j+1 << " moved to " << j << std::endl;
#endif
				// 2) shift the corresponding cubes.
				for(int ix = 0; ix < iNumOfCellsX; ix++){
					for(int iz = 0; iz < iNumOfCellsZ; iz++){
						index = plane[j]->getCubeIndex(ix, iz);
						// If the cell is not empty, move the cube by one cube size to the bottom.
						if(index >= 0){
							int factor = -1;
							fixedCubes->moveCubeY(factor, index);
						}
						// Reset all the highlighted cubes in the bottommost plane once again.
						plane[0]->resetHighlightedCell(ix, iz);
					}
				}// End shift the cubes.	
			}// End shift nonempty planes.

			--secondBottomPlane;	// Diminish the second highlighted plane.

			// Don't increase count i, start the 'while' loop again from the current plane i, 
			// which has just been shifted.
			
		}// End if plane is filled.
		++i;	// Go to the next upper plane if no annihilation.
	}
}

std::tr1::shared_ptr<Figure> PlayScreen::createNewFigure()
{
	iCurrentFigureId += cubesPerFigure;
	vFigureOrigin = vector_3d(0., 0.5*(iNumOfPlanes - 4), 0.);	// Must be multiple of the cube size.

	// Generate the random figure. 
	int iMinId = 0;
	int iMaxId = 5;
	int iFigId = (float)generateRand((float)iMinId, (float)iMaxId);

	switch( iFigId ){
	case 0:	// Lfig.
		return 
		std::tr1::shared_ptr<Figure>( new Lfigure(cubesPerFigure, vFigureOrigin, iCurrentFigureId, cubeSize, iNumOfCellsX, iNumOfPlanes, iNumOfCellsZ) );
	
	case 1:// Ofig.
		return
		std::tr1::shared_ptr<Figure>( new Ofigure(cubesPerFigure, vFigureOrigin, iCurrentFigureId, cubeSize, iNumOfCellsX, iNumOfPlanes, iNumOfCellsZ) );

	case 2:// Sfig.
		return
		std::tr1::shared_ptr<Figure>( new Sfigure(cubesPerFigure, vFigureOrigin, iCurrentFigureId, cubeSize, iNumOfCellsX, iNumOfPlanes, iNumOfCellsZ) );
	
	case 3:// Ifig.
		return
		std::tr1::shared_ptr<Figure>( new Ifigure(cubesPerFigure, vFigureOrigin, iCurrentFigureId, cubeSize, iNumOfCellsX, iNumOfPlanes, iNumOfCellsZ) );
	
	case 4:// Tfig.
		return
		std::tr1::shared_ptr<Figure>( new Tfigure(cubesPerFigure, vFigureOrigin, iCurrentFigureId, cubeSize, iNumOfCellsX, iNumOfPlanes, iNumOfCellsZ) );

	}// End switch.

	return 
		std::tr1::shared_ptr<Figure>( new Tfigure(cubesPerFigure, vFigureOrigin, iCurrentFigureId, cubeSize, iNumOfCellsX, iNumOfPlanes, iNumOfCellsZ) );
}

// FixedCubes methods.
void PlayScreen::createPlanes()
{
	plane.resize(iNumOfPlanes);

	for(std::size_t i = 0; i < plane.size(); i++){
		plane[i] = 
			std::tr1::shared_ptr<PlaneOfCells>( new PlaneOfCells(i, iNumOfCellsX, iNumOfCellsZ, cubeSize) );
	}
}

void PlayScreen::drawPlanes()
{
	glPushMatrix();

	// Translate the coordinates, such that the origin is at the center of the scene.
	glTranslatef(-0.5*cubeSize*iNumOfCellsX, -0.5*cubeSize*iNumOfPlanes, -0.5*cubeSize*iNumOfCellsZ);

	// Left and right grids.
	for(std::size_t i = 0; i < plane.size(); ++i){
		plane[i]->drawLeftFaces();
		plane[i]->drawRightFaces();
	}

	// Bottom grid.
	if( (bottomPlane >= 0) && (bottomPlane < iNumOfPlanes) )
		plane[bottomPlane]->drawBottomFaces();
	if( (secondBottomPlane >= 0) && (secondBottomPlane < iNumOfPlanes) )
		plane[secondBottomPlane]->drawHighlightedBottomCells();

	glPopMatrix();
}

void PlayScreen::addCubes()
{
	std::vector<CellIndeces> ci;		// Cells in which the cubes of the figure are located.
	ci.resize(4);	
	currentFigure->getCubeIndeces(ci);	// Get indeces of all the figure cubes in the cells.
	
	std::vector<std::tr1::shared_ptr<Cube> > c;
	c = currentFigure->getCubes();	// Cubes of the collided figure.

	// Loop over cubes, add them to the fixed cubes and fill in the corresponding cells.
	for(std::size_t cellId = 0; cellId < ci.size(); cellId++){
		int cubeId = c[cellId]->getId();

		if( cubeId >= 0)
			fixedCubes->insertCube(c[cellId], cubeId);

		if( (ci[cellId].plane >= 0) && (ci[cellId].plane < iNumOfPlanes) )
			plane[ci[cellId].plane]->fillCell(ci[cellId].x, ci[cellId].z, cubeId);
	}

	// Find the topmost nonempty plane.
	int iStart = 0;
	int iEnd = plane.size();
	secondBottomPlane = getTopEmptyPlane(iStart, iEnd);
}

int PlayScreen::getTopEmptyPlane(int iStart, int iEnd)
{
	int i;
	for(i = iStart; i < iEnd; ++i)
		if( (iStart >= 0) && (iEnd <= iNumOfPlanes) )
		if( plane[i]->isEmpty() )
			return i;		

	return i;
}

void PlayScreen::setHighlightedCells(std::vector<CellIndeces>& cells)
{
	int x, z, p;
	std::size_t i;

	for(i = 0; i < currentCells.size(); ++i)
		if( (cells[i].plane >= 0) && (cells[i].plane < iNumOfPlanes) ){
			p = cells[i].plane;
			x = cells[i].x;
			z = cells[i].z;
			if( (x >=0) && (x < iNumOfCellsX) && (z >= 0) && (z < iNumOfCellsZ)
			  ){
				plane[p]->setHighlightedLeftCell(z);
				plane[p]->setHighlightedRightCell(z);
				plane[p]->setHighlightedCell(x, z);
				// Reset the bottom planes.
				if( (bottomPlane >= 0) && (bottomPlane < iNumOfPlanes) )
					plane[bottomPlane]->setHighlightedCell(x, z);
				if( (secondBottomPlane >= 0) && (secondBottomPlane < iNumOfPlanes) )
					plane[secondBottomPlane]->setHighlightedCell(x, z);
			}
		}
}

void PlayScreen::resetHighlightedCells(std::vector<CellIndeces>& cells)
{
	int x, z, p;
	std::size_t i;

	for(i = 0; i < cells.size(); ++i)
		if( (cells[i].plane >= 0) && (cells[i].plane < iNumOfPlanes) ){
			p = cells[i].plane;
			x = cells[i].x;
			z = cells[i].z;
			if( (x >=0) && (x < iNumOfCellsX) && (z >= 0) && (z < iNumOfCellsZ)
			  ){
				  plane[p]->resetHighlightedLeftCell(z);
				  plane[p]->resetHighlightedRightCell(z);
				  plane[p]->resetHighlightedCell(x, z);
				  // Reset the bottom plane.
				  if( (bottomPlane >= 0) && (bottomPlane < iNumOfPlanes) )
					  plane[bottomPlane]->resetHighlightedCell(x, z);
				  if( (secondBottomPlane >= 0) && (secondBottomPlane < iNumOfPlanes) )
					  plane[secondBottomPlane]->resetHighlightedCell(x, z);
			}
		}
}

void PlayScreen::dropFigure(Logic& logic)
{
	static int count = 0;
	int factor = -1;

	count++;	// Number of frames between movements.
	// Move the figure, if necessary.
	if( (count % iFrameDelayMove == 0) && (!logic.bTrain) ){
		count = 0;		
		currentFigure->moveY(factor, plane);		// Fall in y direction.
	}
}

bool PlayScreen::isRoundFinished(Logic& logic)
{
	currentFigure->getCubeIndeces(currentCells);// Define what cells are highlighted.
	// Get the index of the lowest plane of the figure.
	iLowestCurrentPlane = currentCells[0].plane;
	for(std::size_t i = 0; i < currentCells.size(); ++i)
		iLowestCurrentPlane = min(iLowestCurrentPlane, currentCells[i].plane);
	// Check the topmost empty plane. If it is the last one, then user has lost.
	// Because this means that the new figure touches the fixed cubes.
	int iStart = 0;
	int iEnd = plane.size();
	int iTopPlane = getTopEmptyPlane(iStart, iEnd);

	if( iLowestCurrentPlane < iTopPlane )		// If new figure touches the topmost filled plane.
		return true;

	if( logic.iUserScore >= logic.iMaxScore)	// If the user annihilated enough planes.
		return true;

	return false;
}