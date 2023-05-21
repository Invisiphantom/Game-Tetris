#include "Tetris.h"

void Tetris::processGameEvent(SDL_Event _evt) {
	if (_evt.type == SDL_KEYDOWN) {
		if (_evt.key.keysym.sym == SDLK_LEFT) {
			if (GameState == TS_RUN) {
				moveLeftBlock();
			}
		} else if (_evt.key.keysym.sym == SDLK_RIGHT) {
			if (GameState == TS_RUN) {
				moveRightBlock();
			}
		} else if (_evt.key.keysym.sym == SDLK_UP) {
			if (GameState == TS_MENU) {
				adjustMenuLevel(1);
			}
			if (GameState == TS_RUN) {
				rotateBlock();
			}
		} else if (_evt.key.keysym.sym == SDLK_DOWN) {
			if (GameState == TS_MENU) {
				adjustMenuLevel(-1);
			}
			if (GameState == TS_RUN) {
				moveDownBlock();
			}
		} else if (_evt.key.keysym.sym == SDLK_SPACE) {
			if (GameState == TS_MENU) {
				playTetris();
			}
			if (GameState == TS_RUN) {
				dropBlock();
			}
		} else if (_evt.key.keysym.sym == SDLK_p) {
			if (GameState == TS_RUN || GameState == TS_PAUSE) {
				toggleRunPause();
			}
		} else if (_evt.key.keysym.sym == SDLK_q) {
			if (GameState == TS_RUN || GameState == TS_PAUSE) {
				quitTetris();
			}
		}
	}
}
void Tetris::BinarytoPixel(int _containRow, unsigned short _Binary) {
	for (int containCol = 0; containCol <= 11; containCol++) {
		bool bit = (_Binary >> containCol) & 0x0001;
		Container[_containRow][containCol] = bit;
	}
}

void Tetris::updateGame(float ms) {
	if (GameState != TS_PAUSE) tick += ms;
	if (GameState == TS_MENU) {
		memset(Container, 0, sizeof(Container));

		BinarytoPixel(4, 0x0000);
		BinarytoPixel(5, 0x07DE);
		BinarytoPixel(6, 0x0102);
		BinarytoPixel(7, 0x011E);
		BinarytoPixel(8, 0x0102);
		BinarytoPixel(9, 0x011E);

		static unsigned short menuFrames[][8] = {
			{0x0010, 0x0018, 0x0010, 0x0000, 0x0000, 0x0002, 0x0766, 0x076E},
			{0x0000, 0x0010, 0x0018, 0x0010, 0x0000, 0x0002, 0x0766, 0x076E},
			{0x0000, 0x0000, 0x0010, 0x0018, 0x0010, 0x0002, 0x0766, 0x076E},
			{0x0000, 0x0000, 0x0000, 0x0010, 0x0018, 0x0012, 0x0766, 0x076E},
			{0x0000, 0x0000, 0x0000, 0x0000, 0x0010, 0x001A, 0x0776, 0x076E},
			{0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0012, 0x077E, 0x077E},
			{0x00C0, 0x0080, 0x0080, 0x0000, 0x0000, 0x0012, 0x077E, 0x077E},
			{0x0000, 0x00C0, 0x0080, 0x0080, 0x0000, 0x0012, 0x077E, 0x077E},
			{0x0000, 0x0000, 0x00C0, 0x0080, 0x0080, 0x0012, 0x077E, 0x077E},
			{0x0000, 0x0000, 0x0000, 0x00C0, 0x0080, 0x0092, 0x077E, 0x077E},
			{0x0000, 0x0000, 0x0000, 0x0000, 0x00C0, 0x0092, 0x07FE, 0x077E},
			{0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00D2, 0x07FE, 0x07FE},
			{0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00D2},
			{0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00D2}};

		for (int row = 0; row < 8; row++)
			BinarytoPixel(10 + row, menuFrames[menuFrameIndex][row]);
		while (tick >= 500) {
			menuFrameIndex = (menuFrameIndex + 1) % 13;
			tick -= 500;
		}

		static unsigned short menuLevel[][5] = {
			{0x03B8, 0x02A8, 0x02A8, 0x02A8, 0x03B8},
			{0x0138, 0x01A8, 0x0128, 0x0128, 0x03B8},
			{0x03B8, 0x0228, 0x03A8, 0x00A8, 0x03B8},
			{0x03B8, 0x0228, 0x03A8, 0x0228, 0x03B8},
			{0x02B8, 0x02A8, 0x03A8, 0x0228, 0x0238},
			{0x03B8, 0x00A8, 0x03A8, 0x0228, 0x03B8},
			{0x03B8, 0x00A8, 0x03A8, 0x02A8, 0x03B8},
			{0x03B8, 0x0228, 0x0228, 0x0228, 0x0238},
			{0x03B8, 0x02A8, 0x03A8, 0x02A8, 0x03B8},
			{0x03B8, 0x02A8, 0x03A8, 0x0228, 0x03B8},
		};

		for (int row = 0; row < 5; row++)
			BinarytoPixel(19 + row, menuLevel[level][row]);

	} else if (GameState == TS_INIT) {
		for (int containRow = 0; containRow <= 24; containRow++) {
			Container[containRow][0] = Container[containRow][11] = 1;
			for (int containCol = 1; containCol <= 10; containCol++)
				Container[containRow][containCol] = 0;
		}
		for (int containCol = 1; containCol <= 10; containCol++)
			Container[24][containCol] = 1;

		CurrentBlock.initRandBlock();
		LoadingBlock.initRandBlock();
		speed = 800;
		tick = 0.0f;
		GameState = TS_RUN;
	} else if (GameState == TS_RUN) {
		initUnderDropBlock();

		while (tick >= speed) {
			moveDownBlock();
			tick -= speed;
		}

		if (score > hiScore) hiScore = score;

		static int speedLevels[] = {1000, 800, 700, 600, 500,
									400,  300, 200, 150, 100};
		speed = speedLevels[level];

		for (int containCol = 1; containCol <= 10; containCol++)
			if (Container[3][containCol] == 1) {
				tick = 0.0f;
				deadFrameIndex = 4;
				deadFrameDown = 1;
				deadFrameUp = 0;
				GameState = TS_DEAD;
			}
	} else if (GameState == TS_DEAD) {
		if (tick >= 50.0f) {
			if (deadFrameDown == true) {
				for (int containCol = 1; containCol <= 10; containCol++)
					Container[deadFrameIndex][containCol] = 1;
				deadFrameIndex++;
				if (deadFrameIndex == 24) {
					deadFrameDown = false;
					deadFrameUp = true;
				}
			} else if (deadFrameUp == true) {
				for (int containCol = 1; containCol <= 10; containCol++)
					Container[deadFrameIndex][containCol] = 0;
				deadFrameIndex--;
				if (deadFrameIndex == 3) {
					deadFrameUp = false;
				}
			} else {
				GameState = TS_MENU;
				menuFrameIndex = 0;
				score = 0;
				tick = 0.0f;
			}
			tick -= 50.0f;
		}
	}
}

void Tetris::playTetris() { GameState = TS_INIT; }
void Tetris::quitTetris() { GameState = TS_MENU; }
void Tetris::adjustMenuLevel(int _num) {
	level += _num;
	if (level < 0)
		level = 9;
	else if (level > 9)
		level = 0;
}
void Tetris::toggleRunPause() {
	if (GameState == TS_RUN)
		GameState = TS_PAUSE;
	else if (GameState == TS_PAUSE)
		GameState = TS_RUN;
}
void Tetris::moveLeftBlock() {
	Block nextBlock = CurrentBlock;
	nextBlock.containCol--;
	if (hitBlock(nextBlock) == false) CurrentBlock.containCol--;
}
void Tetris::moveRightBlock() {
	Block nextBlock = CurrentBlock;
	nextBlock.containCol++;
	if (hitBlock(nextBlock) == false) CurrentBlock = nextBlock;
}
void Tetris::moveDownBlock() {
	Block nextBlock = CurrentBlock;
	nextBlock.containRow++;
	if (hitBlock(nextBlock) == false)
		CurrentBlock.containRow++;
	else {
		mergeBlock();
		eraseLines();
		CurrentBlock = LoadingBlock;
		LoadingBlock.initRandBlock();
	}
}
void Tetris::dropBlock() {
	Block nextBlock = CurrentBlock;
	nextBlock.containRow++;
	while (hitBlock(nextBlock) == false) {
		CurrentBlock.containRow++;
		nextBlock = CurrentBlock;
		nextBlock.containRow++;
	}
	mergeBlock();
	eraseLines();
	CurrentBlock = LoadingBlock;
	LoadingBlock.initRandBlock();
}
void Tetris::initUnderDropBlock() {
	UnderDropBlock = CurrentBlock;
	do UnderDropBlock.containRow++;
	while (hitBlock(UnderDropBlock) == false);
	UnderDropBlock.containRow--;
}

void Tetris::rotateBlock() {
	Block nextBlock = CurrentBlock;
	nextBlock.state = BlockState((nextBlock.state + 1) % BS_NUM);
	if (hitBlock(nextBlock) == false)
		CurrentBlock.state = BlockState((CurrentBlock.state + 1) % BS_NUM);
}

void Tetris::mergeBlock() {
	auto blockBits = TetrisList[CurrentBlock.type][CurrentBlock.state];
	for (int blockRow = 0, containRow = CurrentBlock.containRow;
		 blockRow < BLOCK_HEIGHT; blockRow++, containRow++)
		for (int blockCol = 0, containCol = CurrentBlock.containCol;
			 blockCol < BLOCK_WIDTH; blockCol++, containCol++) {
			if (blockBits[blockRow][blockCol] == 1)
				Container[containRow][containCol] = 1;
		}
}
bool Tetris::hitBlock(const Block& _block) {
	auto blockBits = TetrisList[_block.type][_block.state];
	for (int blockRow = 0, containRow = _block.containRow;
		 blockRow < BLOCK_HEIGHT; blockRow++, containRow++)
		for (int blockCol = 0, containCol = _block.containCol;
			 blockCol < BLOCK_WIDTH; blockCol++, containCol++) {
			if (containCol >= 0 && Container[containRow][containCol] == 1 &&
				blockBits[blockRow][blockCol] == 1)
				return true;
		}
	return false;
}

void Tetris::eraseLines() {
	int line = 23;
	int afterLine = 23;
	int eraseLine = 0;
	bool FullLine = 1;
	while (line >= 4) {
		for (int containCol = 1; containCol <= 10; containCol++)
			if (Container[line][containCol] == 0) FullLine = 0;
		if (FullLine == 0)
			afterLine--;
		else
			eraseLine++;
		line--;
		if (afterLine != line)
			for (int containCol = 1; containCol <= 10; containCol++)
				Container[afterLine][containCol] = Container[line][containCol];
		FullLine = 1;
	}
	while (afterLine >= 4) {
		afterLine--;
		for (int containCol = 1; containCol <= 10; containCol++)
			Container[afterLine][containCol] = 0;
	}
	score += eraseLine * 100;
}

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

void Tetris::renderGame() {
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

	if (GameState == TS_RUN || GameState == TS_PAUSE) {
		renderBlock(CurrentBlock, 255);
		renderBlock(UnderDropBlock, 80);
	}

	int Win_y;
	Win_y = renderTextNum(10, "Score", score);
	Win_y = renderTextNum(Win_y + 6, "Hi-Score", hiScore);
	Win_y = renderTextNum(Win_y + 6, "Next", -1);
	Win_y = renderBlocktoXY(Win_y + 30, LoadingBlock, 255);
	Win_y = renderTextNum(Win_y + 8, "Level", level);
}

void Tetris::renderBlock(Block _block, int _alpha) {
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
int Tetris::renderBlocktoXY(int _Winy, Block _block, int _alpha) {
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

int Tetris::renderTextNum(int _y, const char* _text, int _num) {
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
