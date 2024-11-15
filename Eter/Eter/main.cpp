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
	system("start powershell -Command \"Get-Content -Path 'log.txt' -Wait\"");

	/*base::TrainingMode mode("gigi", "titi");
	mode.gameLoop();
	*/
    try {
    
        base::TrainingMode trainingMode("Player1", "Player2");
        trainingMode.gameLoop();

    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }
	return 0;
}

