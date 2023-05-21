#include <Windows.h>
#include "GameModule.h"
#include "Tetris.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
				   _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	GameModule GM(hInstance, "TetrisGame", TETRIS_WINDOW_WIDTH, TETRIS_WINDOW_HEIGHT);
	GM.initGameModules();

	Tetris T(hInstance, GM.getpWin(), GM.getpRenderer());
	T.loadResources();

	GM.embedGameMechanic(T);
	GM.runGame();

	T.unloadResources();
	GM.uninitGameModules();
	return 0;
}