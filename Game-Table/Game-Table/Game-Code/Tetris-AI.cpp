#include "Tetris.h"

void Tetris::initAIMergeContainer(const Block& _block) {
	for (int row = 0; row < CONTAINER_HEIGHT; row++)
		for (int col = 0; col < CONTAINER_WIDTH; col++)
			AITestContainer[row][col] = Container[row][col];

	auto blockBits = TetrisList[_block.type][_block.state];
	for (int blockRow = 0, containRow = _block.containRow;
		 blockRow < BLOCK_HEIGHT; blockRow++, containRow++)
		for (int blockCol = 0, containCol = _block.containCol;
			 blockCol < BLOCK_WIDTH; blockCol++, containCol++) {
			if (blockBits[blockRow][blockCol] == 1)
				AITestContainer[containRow][containCol] = 1;
		}
}

double Tetris::getLandingHeight(const Block& _dropTempAIBlock) const {
	return (CONTAINER_HEIGHT - 2) -
		   (_dropTempAIBlock.containRow +
			TetrisBottomPos[_dropTempAIBlock.type][_dropTempAIBlock.state]) +
		   0.5 * TetrisHeight[_dropTempAIBlock.type][_dropTempAIBlock.state];
}

int Tetris::getEliminateRows() const {
	int eraseLine = 0;
	bool FullLine = 1;
	for (int line = 23; line >= 4; line--) {
		for (int containCol = 1; containCol <= 10; containCol++)
			if (Container[line][containCol] == 0) FullLine = 0;
		if (FullLine == 1) eraseLine++;
		FullLine = 1;
	}
	return eraseLine;
}

int Tetris::getRowTransitions() const {
	int tranTimes = 0;
	for (int row = 23; row >= 4; row--) {
		bool tranSign = 1;
		for (int col = 1; col <= 11; col++) {
			if (AITestContainer[row][col] != tranSign) {
				tranTimes++;
				tranSign = !tranSign;
			}
		}
	}
	return tranTimes;
}

int Tetris::getColumnTransitions() const {
	int tranTimes = 0;
	for (int col = 1; col <= 10; col++) {
		bool tranSign = 1;
		for (int row = 4; row <= 23; row++) {
			if (AITestContainer[row][col] != tranSign) {
				tranTimes++;
				tranSign = !tranSign;
			}
		}
	}
	return tranTimes;
}

int Tetris::getNumberOfHoles() const {
	int holes = 0;
	bool rowHoles[CONTAINER_WIDTH];
	bool prevRow[CONTAINER_WIDTH];
	for (int col = 0; col <= 11; col++) rowHoles[col] = 0;
	for (int col = 0; col <= 11; col++) prevRow[col] = AITestContainer[4][col];

	for (int row = 5; row <= 23; row++) {
		for (int col = 0; col <= 11; col++)
			if (Container[row][col] == 0 &&
				(prevRow[col] == 1 || rowHoles[col] == 1))
				rowHoles[col] = 1;
			else
				rowHoles[col] = 0;
		for (int col = 1; col <= 10; col++)
			if (rowHoles[col] == 1) holes++;
		for (int col = 0; col <= 11; col++) prevRow[col] = Container[row][col];
	}
	return holes;
}

int Tetris::getWellSums() const {
	int wellSums = 0;
	for (int col = 1; col <= 10; col++) {
		for (int row = 4; row <= 23; row++) {
			if (AITestContainer[row][col] == 0 &&
				AITestContainer[row][col - 1] == 1 &&
				AITestContainer[row][col + 1] == 1) {
				wellSums++;
				for (int extendRow = row + 1; extendRow <= 23; extendRow++) {
					if (AITestContainer[extendRow][col] == 0)
						wellSums++;
					else
						break;
				}
			}
		}
	}
	return wellSums;
}

double Tetris::evaluateScore(const Block& _tempAIBlock) {
	double AIScore = 0.0;
	Block dropTempAIBlock = _tempAIBlock;
	while (hitBlock(dropTempAIBlock) == false) dropTempAIBlock.containRow++;
	dropTempAIBlock.containRow--;
	initAIMergeContainer(dropTempAIBlock);
	AIScore += -4.500158825082766 * getLandingHeight(dropTempAIBlock);
	AIScore += 3.4181268101392694 * getEliminateRows();
	AIScore += -3.2178882868487753 * getRowTransitions();
	AIScore += -9.348695305445199 * getColumnTransitions();
	AIScore += -7.899265427351652 * getNumberOfHoles();
	AIScore += -3.3855972247263626 * getWellSums();
	return AIScore;
}

void Tetris::initAIRecommendBlock() {
	double maxAIScore = -DBL_MAX;
	Block tempAIBlock = CurrentBlock;
	for (tempAIBlock.containCol = -1; tempAIBlock.containCol <= 8;
		 tempAIBlock.containCol++)
		for (tempAIBlock.state = static_cast<BlockState>(0);
			 tempAIBlock.state < BS_NUM;
			 tempAIBlock.state =
				 static_cast<BlockState>(tempAIBlock.state + 1)) {
			if (hitBlock(tempAIBlock) == false) {
				double AIscore = evaluateScore(tempAIBlock);
				if (AIscore > maxAIScore) {
					maxAIScore = AIscore;
					AIBlock = tempAIBlock;
				}
			}
		}
	while (hitBlock(AIBlock) == false) AIBlock.containRow++;
	AIBlock.containRow--;
}
