#include "GameModule.h"

void GameModule::initGameModules() {
	int res;
	res = SDL_Init(SDL_INIT_EVERYTHING);
	res = TTF_Init();
	res = IMG_Init(IMG_INIT_PNG);  //  IMG_INIT_JPG | IMG_INIT_WEBP;
	res = Mix_Init(MIX_INIT_MID | MIX_INIT_MP3);
	res = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
						MIX_DEFAULT_CHANNELS, 2048);
	pWin = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
							SDL_WINDOWPOS_CENTERED, winSize.cx, winSize.cy, 0);
	pRenderer = SDL_CreateRenderer(pWin, -1, SDL_RENDERER_ACCELERATED);
}

void GameModule::uninitGameModules() {
	SDL_DestroyRenderer(pRenderer);
	SDL_DestroyWindow(pWin);
	pRenderer = nullptr;
	pWin = nullptr;
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void GameModule::runGame() const{
	int quit = 0;
	SDL_Event evt;

	Uint64 nFrequency, nPrevCounter, nCurrCounter, nElapsedCounter;
	float elapsed = 0.0f, lag = 0.0f, frameMS = 1000.0f / FPS;

	nFrequency = SDL_GetPerformanceFrequency();
	nPrevCounter = SDL_GetPerformanceCounter();

	while (!quit) {
		if (SDL_PollEvent(&evt))
			if (evt.type == SDL_QUIT)
				quit = 1;
			else
				gameMechanic->processGameEvent(evt);
		else {
			nCurrCounter = SDL_GetPerformanceCounter();
			nElapsedCounter = nCurrCounter - nPrevCounter;
			nPrevCounter = nCurrCounter;

			elapsed = (nElapsedCounter * 1000.0f) / nFrequency;	 // ms
			lag += elapsed;

			while (lag >= frameMS) {
				gameMechanic->updateGame(frameMS);
				lag -= frameMS;
			}

			SDL_SetRenderDrawColor(pRenderer, 25, 25, 100, 255);
			SDL_RenderClear(pRenderer);

			gameMechanic->renderGame();

			SDL_RenderPresent(pRenderer);
		}
	}
}