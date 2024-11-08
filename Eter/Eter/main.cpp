#include <optional>
import <iostream>;
import <string>;
import <unordered_map>;
import <unordered_set>;
import <memory>;
import <format>;

import utils;

import Game;
import Logger;
import Board;
import CombatCard;
import CombatCardType;
import TrainingMode;
import Player;
import GameModeTypes;

using namespace base;

int main() {


	TrainingMode trainingInstance("player1", "player2");
	trainingInstance.gameLoop(true);

	return 0;
}