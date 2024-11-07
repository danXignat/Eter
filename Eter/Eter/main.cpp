import <iostream>;
import <string>;
import <unordered_map>;
import <memory>;
import <unordered_set>;

import utils;

import Game;
import Logger;
import Board;
import CombatCard;
import CombatCardType;
import TrainingMode;

int main() {
	//system("start powershell -Command \"Get-Content -Path 'log.txt' -Wait\"");

	modes::TrainingMode trainingInstance("Player1", "Player2");

	bool win = false;
	trainingInstance.gameLoop(win);




}