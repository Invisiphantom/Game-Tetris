#include "Tetris.h"

void Tetris::processGameEvent(const SDL_Event _evt) {
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
		} else if (_evt.key.keysym.sym == SDLK_RETURN) {
			if (GameState == TS_MENU) {
				playTetris();
			}
			if (GameState == TS_RUN || GameState == TS_AI) {
				toggleAIMod();
			}
		} else if (_evt.key.keysym.sym == SDLK_p) {
			if (GameState == TS_RUN || GameState == TS_PAUSE ||
				GameState == TS_AI) {
				toggleRunPause();
			}
		} else if (_evt.key.keysym.sym == SDLK_q) {
			if (GameState == TS_RUN || GameState == TS_PAUSE ||
				GameState == TS_AI) {
				quitTetris();
			}
		}
	}
}
void Tetris::transHextoContainer(int _containRow, unsigned short _Binary) {
	for (int containCol = 0; containCol <= 11; containCol++) {
		bool bit = (_Binary >> containCol) & 0x0001;
		Container[_containRow][containCol] = bit;
	}
}

void Tetris::updateGame(const float ms) {
	if (GameState != TS_PAUSE) tick += ms;
	if (GameState == TS_MENU) {
		memset(Container, 0, sizeof(Container));

		transHextoContainer(4, 0x0000);
		transHextoContainer(5, 0x07DE);
		transHextoContainer(6, 0x0102);
		transHextoContainer(7, 0x011E);
		transHextoContainer(8, 0x0102);
		transHextoContainer(9, 0x011E);

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
			transHextoContainer(10 + row, menuFrames[menuFrameIndex][row]);
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
			transHextoContainer(19 + row, menuLevel[level][row]);

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
				score = 0;
				deadFrameIndex = 4;
				deadFrameDown = 1;
				deadFrameUp = 0;
				GameState = TS_DEAD;
			}
	} else if (GameState == TS_AI) {
		initAIRecommendBlock();
		CurrentBlock = AIBlock;

		while (tick >= speed) {
			moveDownBlock();
			tick -= speed;
		}

		if (score > hiScore) hiScore = score;

		speed = 10;

		for (int containCol = 1; containCol <= 10; containCol++)
			if (Container[3][containCol] == 1) {
				tick = 0.0f;
				score = 0;
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
	if (GameState == TS_RUN || TS_AI)
		GameState = TS_PAUSE;
	else if (GameState == TS_PAUSE)
		GameState = TS_RUN;
}
void Tetris::toggleAIMod() {
	if (GameState == TS_RUN)
		GameState = TS_AI;
	else if (GameState == TS_AI)
		GameState = TS_RUN;
}