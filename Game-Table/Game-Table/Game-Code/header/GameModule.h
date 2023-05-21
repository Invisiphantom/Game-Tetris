#pragma once

#include <Windows.h>

#include <string>

#include "GameMechanic.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#define DEFAULT_FPS 60

class GameModule {
   private:
	HINSTANCE hInst = 0;
	SDL_Window* pWin = nullptr;
	SDL_Renderer* pRenderer = nullptr;

	std::string title;
	SIZE winSize = {0l, 0l};
	unsigned int FPS = 0u;

	GameMechanic* gMechanic;

   public:
	explicit GameModule(HINSTANCE _hInst, std::string _title,
						LONG _winSizeWidth, LONG _winSizeHeight)
		: hInst(_hInst),
		  title(_title),
		  winSize({_winSizeWidth, _winSizeHeight}),
		  FPS(DEFAULT_FPS) {}
	~GameModule() {}

	void initGameModules();
	void uninitGameModules();
	void runGame();

	SDL_Window* getpWin() { return pWin; }
	SDL_Renderer* getpRenderer() { return pRenderer; }

	void embedGameMechanic(GameMechanic& _GameMechanic) {
		gMechanic = &_GameMechanic;
	}
};