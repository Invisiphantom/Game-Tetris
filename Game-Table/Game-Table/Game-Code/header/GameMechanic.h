#pragma once
#include "SDL.h"
class GameMechanic {
   public:
	GameMechanic() {}
	virtual ~GameMechanic() {}
	virtual void processGameEvent(const SDL_Event _evt) = 0;
	virtual void updateGame(const float _ms) = 0;
	virtual void renderGame() const = 0;
};
