#include "Tetris.h"

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
	nextBlock.state = static_cast<BlockState>((nextBlock.state + 1) % TS_NUM);
	if (hitBlock(nextBlock) == false)
		CurrentBlock.state =
			static_cast<BlockState>((CurrentBlock.state + 1) % TS_NUM);
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
bool Tetris::hitBlock(const Block& _block) const {
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