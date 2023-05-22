#pragma once

#include <Windows.h>

#include "GameMechanic.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

class GameModule {
   private:
	HINSTANCE hInst = 0;
	SDL_Window* pWin = nullptr;
	SDL_Renderer* pRenderer = nullptr;

	const char* title;
	unsigned int FPS = 0u;
	SIZE winSize = {0l, 0l};

	GameMechanic* gameMechanic;

   public:
	explicit GameModule(HINSTANCE _hInst, const char* _title,
						LONG _winSizeWidth, LONG _winSizeHeight, unsigned int _FPS)
		: hInst(_hInst),
		  title(_title),
		  winSize({_winSizeWidth, _winSizeHeight}),
		  FPS(_FPS) {}
	~GameModule() {}
	GameModule(const GameModule&) = delete;
	GameModule(const GameModule&&) = delete;
	GameModule& operator&=(const GameModule&) = delete;
	GameModule& operator&=(const GameModule&&) = delete;

	void initGameModules();
	void uninitGameModules();
	void runGame() const;

	SDL_Window* getpWin() { return pWin; }
	SDL_Renderer* getpRenderer() { return pRenderer; }

	void embedGameMechanic(GameMechanic& _GameMechanic) {
		gameMechanic = &_GameMechanic;
	}
};