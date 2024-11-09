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
import Player;
import GameModeTypes;
import TrainingMode;

int main() {
	base::TrainingMode mode("gigi", "titi");
	mode.gameLoop();
	

	return 0;
}