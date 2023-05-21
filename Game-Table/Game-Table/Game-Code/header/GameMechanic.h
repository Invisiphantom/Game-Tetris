#pragma once
#include "SDL.h"
class GameMechanic {
   public:
	GameMechanic() {}
	~GameMechanic() {}
	virtual void processGameEvent(SDL_Event _evt){}
	virtual void updateGame(float _ms){}
	virtual void renderGame(){}
};
