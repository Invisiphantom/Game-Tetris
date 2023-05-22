#pragma once

#include <Windows.h>

#include <random>

#include "GameMechanic.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "resource.h"

#define BLOCK_WIDTH 4
#define BLOCK_HEIGHT 4

#define BLOCK_IMAGE_WIDTH 38
#define BLOCK_IMAGE_HEIGHT 38

#define CONTAINER_WIDTH (1 + 10 + 1)
#define CONTAINER_HEIGHT (4 + 20 + 1)

#define TETRIS_WINDOW_WIDTH ((CONTAINER_WIDTH + 3) * BLOCK_IMAGE_WIDTH)
#define TETRIS_WINDOW_HEIGHT ((CONTAINER_HEIGHT - 5) * BLOCK_IMAGE_HEIGHT)

enum BlockType { BT_O = 0, BT_I, BT_S, BT_Z, BT_L, BT_J, BT_T, BT_NUM };
enum BlockState { BS_W = 0, BS_A, BS_S, BS_D, BS_NUM };
static const int TetrisList[BT_NUM][BS_NUM][4][4] = {
	{{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
	 {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
	 {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
	 {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}},
	{{{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
	 {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}}},
	{{{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}},
	 {{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}},
	 {{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}},
	 {{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}}},
	{{{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
	 {{0, 0, 0, 1}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}},
	 {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
	 {{0, 0, 0, 1}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}}},
	{{{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 1, 0, 0}, {0, 0, 0, 0}},
	 {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
	 {{0, 0, 0, 1}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}}},
	{{{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}},
	 {{0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
	 {{0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}}},
	{{{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}},
	 {{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}},
	 {{0, 0, 1, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}}}};

class Block {
   public:
	int containRow = 3;
	int containCol = 4;

	BlockType type = BT_O;
	BlockState state = BS_W;

   public:
	void initRandBlock() {
		std::random_device rd;
		std::mt19937 gen(rd());
		containRow = 3;
		containCol = 4;
		type = (BlockType)(gen() % BT_NUM);
		state = (BlockState)(gen() % BS_NUM);
	}
};

class Tetris : public GameMechanic {
	enum TetrisState { TS_MENU = 0, TS_INIT, TS_RUN, TS_PAUSE, TS_DEAD, TS_NUM };

   private:
	HINSTANCE hInst = 0;
	SDL_Window* pWin = nullptr;
	SDL_Renderer* pRenderer = nullptr;

	TTF_Font* gDefFont = nullptr;
	TTF_Font* gOptionsFont = nullptr;
	TTF_Font* gDisplayFont = nullptr;
	TTF_Font* gTextFont = nullptr;

	SDL_Texture* gTexture = nullptr;

	Mix_Music* gBGM = nullptr;
	Mix_Music* gGameOver = nullptr;

	Mix_Chunk* gMove = nullptr;
	Mix_Chunk* gRotate = nullptr;
	Mix_Chunk* gErase = nullptr;
	Mix_Chunk* gDrop = nullptr;

	bool Container[CONTAINER_HEIGHT][CONTAINER_WIDTH];
	TetrisState GameState = TS_MENU;
	Block LoadingBlock;
	Block CurrentBlock;
	Block UnderDropBlock;
	int menuFrameIndex = 0;
	int deadFrameIndex = 0;
	bool deadFrameDown;
	bool deadFrameUp;
	float tick = 0.0f;
	int level = 1;
	int speed = 0;
	int score = 0;
	int hiScore = 0;

   private:
	SDL_RWops* importResourceData(HINSTANCE, LPCWSTR, LPCWSTR);

	void playTetris();
	void quitTetris();
	void toggleRunPause();
	void adjustMenuLevel(int _num);
	bool hitBlock(const Block& _block) const;
	void moveLeftBlock();
	void moveRightBlock();
	void moveDownBlock();
	void dropBlock();
	void initUnderDropBlock();
	void rotateBlock();
	void mergeBlock();
	void eraseLines();

	void transHextoContainer(int _containRow, unsigned short _Hex);
	void renderBlock(const Block& _block, const int _alpha) const;
	int renderBlocktoXY(int _Winy, const Block& _block, int _alpha) const;
	int renderTextNum(const int _y, const char* _text, const int _num) const;

   public:
	explicit Tetris(HINSTANCE _hInst, SDL_Window* _pWin,
					SDL_Renderer* _pRenderer)
		: hInst(_hInst),
		  pWin(_pWin),
		  pRenderer(_pRenderer),
		  GameState(TS_MENU) {}
	~Tetris() {}
	Tetris(const Tetris&) = delete;
	Tetris(const Tetris&&) = delete;
	Tetris& operator&=(const Tetris&) = delete;
	Tetris& operator&=(const Tetris&&) = delete;

	void loadResources();
	void unloadResources();

	void processGameEvent(const SDL_Event _evt);
	void updateGame(const float ms);
	void renderGame() const;
};
