#include "Tetris.h"

typedef enum TileType {
	TT_NONE = 0,
	TT_BK,
	TT_FK,
	TT_MUSIC_OPENED,
	TT_MUSIC_CLOSED,
	TT_PAUSE,
	TT_NUM
} TileType;

SDL_Rect* getTileRect(TileType type) {
	static SDL_Rect rt[TT_NUM] = {
		{BLOCK_IMAGE_WIDTH, 0, BLOCK_IMAGE_WIDTH, BLOCK_IMAGE_HEIGHT},
		{BLOCK_IMAGE_WIDTH * 2, 0, BLOCK_IMAGE_WIDTH, BLOCK_IMAGE_HEIGHT},
		{0, 0, BLOCK_IMAGE_WIDTH, BLOCK_IMAGE_HEIGHT},
		{BLOCK_IMAGE_WIDTH * 3, 0, BLOCK_IMAGE_WIDTH / 2, BLOCK_IMAGE_HEIGHT},
		{BLOCK_IMAGE_WIDTH * 3 + BLOCK_IMAGE_WIDTH / 2, 0,
		 BLOCK_IMAGE_WIDTH / 2, BLOCK_IMAGE_HEIGHT},
		{BLOCK_IMAGE_WIDTH * 4, 0, BLOCK_IMAGE_WIDTH / 2, BLOCK_IMAGE_HEIGHT}};
	return &rt[type];
}

void Tetris::renderGame() const {
	SDL_Rect rtDst = {0, 0, TETRIS_WINDOW_WIDTH, TETRIS_WINDOW_HEIGHT};
	SDL_RenderCopy(pRenderer, gTexture, getTileRect(TT_NONE), &rtDst);

	for (int containRow = 4, windowRow = 0; containRow <= 23;
		 containRow++, windowRow++) {
		for (int containCol = 1, windowCol = 0; containCol <= 10;
			 containCol++, windowCol++) {
			rtDst.x = windowCol * BLOCK_IMAGE_WIDTH;
			rtDst.y = windowRow * BLOCK_IMAGE_HEIGHT;
			rtDst.w = BLOCK_IMAGE_WIDTH;
			rtDst.h = BLOCK_IMAGE_HEIGHT;

			SDL_RenderCopy(pRenderer, gTexture, getTileRect(TT_BK), &rtDst);

			if (Container[containRow][containCol] == 1)
				SDL_RenderCopy(pRenderer, gTexture, getTileRect(TT_FK), &rtDst);
		}

		SDL_Rect rtLineSrc = {0, 0, 5, BLOCK_IMAGE_HEIGHT};
		SDL_Rect rtLineDst = {10 * BLOCK_IMAGE_WIDTH + 3,
							  windowRow * BLOCK_IMAGE_HEIGHT, 5,
							  BLOCK_IMAGE_HEIGHT};
		SDL_RenderCopy(pRenderer, gTexture, &rtLineSrc, &rtLineDst);
	}

	if (GameState == TS_RUN || GameState == TS_PAUSE || GameState == TS_AI) {
		renderBlock(CurrentBlock, 255);
		if (GameState == TS_RUN || GameState == TS_PAUSE)
			renderBlock(UnderDropBlock, 80);
	}

	int Win_y;
	Win_y = renderTextNum(10, "Score", score);
	Win_y = renderTextNum(Win_y + 6, "Hi-Score", hiScore);
	Win_y = renderTextNum(Win_y + 6, "Next", -1);
	Win_y = renderBlocktoXY(Win_y + 30, LoadingBlock, 255);
	Win_y = renderTextNum(Win_y + 8, "Level", level);
	if (GameState == TS_AI) Win_y = renderTextNum(Win_y + 20, "AI-Mod", -1);
}

void Tetris::renderBlock(const Block& _block, const int _alpha) const {
	SDL_Rect rt = {0, 0, BLOCK_IMAGE_WIDTH, BLOCK_IMAGE_HEIGHT};

	for (int blockRow = 0; blockRow <= 3; blockRow++) {
		for (int blockCol = 0; blockCol <= 3; blockCol++) {
			if (TetrisList[_block.type][_block.state][blockRow][blockCol] ==
				1) {
				int containRow = _block.containRow + blockRow;
				int containCol = _block.containCol + blockCol;
				int windowRow = containRow - 4;
				int windowCol = containCol - 1;
				rt.y = windowRow * BLOCK_IMAGE_HEIGHT;
				rt.x = windowCol * BLOCK_IMAGE_WIDTH;
				SDL_SetTextureAlphaMod(gTexture, _alpha);
				SDL_RenderCopy(pRenderer, gTexture, getTileRect(TT_FK), &rt);
				SDL_SetTextureAlphaMod(gTexture, 255);
			}
		}
	}
}
int Tetris::renderBlocktoXY(int _Winy, const Block& _block, int _alpha) const {
	SDL_Rect rt = {0, 0, BLOCK_IMAGE_WIDTH, BLOCK_IMAGE_HEIGHT};

	for (int blockRow = 0; blockRow <= 3; blockRow++) {
		for (int blockCol = 0; blockCol <= 3; blockCol++) {
			if (TetrisList[_block.type][_block.state][blockRow][blockCol] ==
				1) {
				rt.y = _Winy + blockRow * BLOCK_IMAGE_HEIGHT;
				rt.x = TETRIS_WINDOW_WIDTH - BLOCK_IMAGE_WIDTH * 4 +
					   blockCol * BLOCK_IMAGE_WIDTH - 15;
				SDL_SetTextureAlphaMod(gTexture, _alpha);
				SDL_RenderCopy(pRenderer, gTexture, getTileRect(TT_FK), &rt);
				SDL_SetTextureAlphaMod(gTexture, 255);
			}
		}
	}
	return _Winy + BLOCK_IMAGE_HEIGHT * 4;
}

int Tetris::renderTextNum(const int _y, const char* _text,
						  const int _num) const {
	SDL_Rect dst{0, 0, 0, 0};
	TTF_SizeText(gTextFont, _text, &dst.w, &dst.h);
	dst.y = _y;
	dst.x = TETRIS_WINDOW_WIDTH - dst.w - 19;
	SDL_Surface* surface =
		TTF_RenderText_Blended(gTextFont, _text, {0, 0, 0, 255});
	SDL_Texture* texture = SDL_CreateTextureFromSurface(pRenderer, surface);
	SDL_RenderCopy(pRenderer, texture, NULL, &dst);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);

	if (_num < 0) return dst.y + dst.h;

	char numText[12] = "9999999";
	TTF_SizeText(gDisplayFont, numText, &dst.w, &dst.h);
	dst.y = dst.y + dst.h - 10;
	dst.x = TETRIS_WINDOW_WIDTH - dst.w - 5;
	surface =
		TTF_RenderText_Blended(gDisplayFont, numText, {143, 155, 150, 255});
	texture = SDL_CreateTextureFromSurface(pRenderer, surface);
	SDL_RenderCopy(pRenderer, texture, NULL, &dst);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);

	sprintf_s(numText, 12, "%d", _num);
	TTF_SizeText(gDisplayFont, numText, &dst.w, &dst.h);
	dst.y = dst.y;
	dst.x = TETRIS_WINDOW_WIDTH - dst.w - 5;
	surface = TTF_RenderText_Blended(gDisplayFont, numText, {0, 0, 0, 255});
	texture = SDL_CreateTextureFromSurface(pRenderer, surface);
	SDL_RenderCopy(pRenderer, texture, NULL, &dst);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);

	return dst.y + dst.h;
}

SDL_RWops* Tetris::importResourceData(HINSTANCE hInst, LPCWSTR pName,
									  LPCWSTR pType) {
	auto hResInfo = FindResource(hInst, pName, pType);
	auto size = SizeofResource(hInst, hResInfo);
	auto hResData = LoadResource(hInst, hResInfo);
	auto pData = LockResource(hResData);
	auto rwOps = SDL_RWFromConstMem(pData, size);
	return rwOps;
}
void Tetris::loadResources() {
	int res;
	SDL_RWops* rwOps;

	rwOps = importResourceData(hInst, MAKEINTRESOURCE(IDI_ICON1), RT_ICON);
	SDL_Surface* iconSurface = IMG_Load_RW(rwOps, 1);
	SDL_SetWindowIcon(pWin, iconSurface);
	SDL_FreeSurface(iconSurface);

	//  TTF_Font* gDefFont = nullptr;
	// 	TTF_Font* gOptionsFont = nullptr;
	// 	TTF_Font* gDisplayFont = nullptr;
	// 	TTF_Font* gTextFont = nullptr;

	rwOps =
		importResourceData(hInst, MAKEINTRESOURCE(IDR_DEFAULT_FONT), RT_FONT);
	gDefFont = TTF_OpenFontRW(rwOps, 1, 18);

	rwOps = importResourceData(hInst, MAKEINTRESOURCE(IDR_FONT3), RT_FONT);
	gOptionsFont = TTF_OpenFontRW(rwOps, 1, 22);

	rwOps = importResourceData(hInst, MAKEINTRESOURCE(IDR_FONT1), RT_FONT);
	gDisplayFont = TTF_OpenFontRW(rwOps, 1, 48);

	rwOps = importResourceData(hInst, MAKEINTRESOURCE(IDR_FONT2), RT_FONT);
	gTextFont = TTF_OpenFontRW(rwOps, 1, 32);

	//  SDL_Texture* gTexture = nullptr;

	rwOps = importResourceData(hInst, MAKEINTRESOURCE(IDB_PNG1), TEXT("PNG"));
	gTexture = IMG_LoadTexture_RW(pRenderer, rwOps, 1);
	SDL_BlendMode sbm = SDL_BLENDMODE_BLEND;
	res = SDL_GetTextureBlendMode(gTexture, &sbm);

	//  Mix_Music* gBGM = nullptr;
	// 	Mix_Music* gGameOver = nullptr;

	rwOps = importResourceData(hInst, MAKEINTRESOURCE(IDR_MP31), TEXT("MP3"));
	gBGM = Mix_LoadMUS_RW(rwOps, 1);

	rwOps = importResourceData(hInst, MAKEINTRESOURCE(IDR_MP32), TEXT("MP3"));
	gGameOver = Mix_LoadMUS_RW(rwOps, 1);

	//  Mix_Chunk* gMove = nullptr;
	// 	Mix_Chunk* gRotate = nullptr;
	// 	Mix_Chunk* gErase = nullptr;
	// 	Mix_Chunk* gDrop = nullptr;

	rwOps = importResourceData(hInst, MAKEINTRESOURCE(IDR_WAVE1), TEXT("WAVE"));
	gMove = Mix_LoadWAV_RW(rwOps, 1);

	rwOps = importResourceData(hInst, MAKEINTRESOURCE(IDR_WAVE2), TEXT("WAVE"));
	gRotate = Mix_LoadWAV_RW(rwOps, 1);

	rwOps = importResourceData(hInst, MAKEINTRESOURCE(IDR_WAVE3), TEXT("WAVE"));
	gErase = Mix_LoadWAV_RW(rwOps, 1);

	rwOps = importResourceData(hInst, MAKEINTRESOURCE(IDR_WAVE4), TEXT("WAVE"));
	gDrop = Mix_LoadWAV_RW(rwOps, 1);
}

void Tetris::unloadResources() {
	Mix_FreeChunk(gMove);
	Mix_FreeChunk(gRotate);
	Mix_FreeChunk(gErase);
	Mix_FreeChunk(gDrop);
	gMove = nullptr;
	gRotate = nullptr;
	gErase = nullptr;
	gDrop = nullptr;

	Mix_FreeMusic(gBGM);
	Mix_FreeMusic(gGameOver);
	gBGM = nullptr;
	gGameOver = nullptr;

	SDL_DestroyTexture(gTexture);
	gTexture = nullptr;

	TTF_CloseFont(gDefFont);
	TTF_CloseFont(gOptionsFont);
	TTF_CloseFont(gDisplayFont);
	TTF_CloseFont(gTextFont);
	gDefFont = nullptr;
	gOptionsFont = nullptr;
	gDisplayFont = nullptr;
	gTextFont = nullptr;
}
