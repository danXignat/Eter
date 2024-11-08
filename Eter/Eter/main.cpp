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
	//system("start powershell -Command \"Get-Content -Path 'log.txt' -Wait\"");

	/*base::Board board(3);

	while (true) {
		system("cls");

		board.renderBoard();

		int16_t x, y;
		std::cin >> x >> y;

		board.appendMove(
			{ x, y }, std::make_unique<base::CombatCard>(base::CombatCardType::ETER)
		);
	}*/
	std::unordered_map < std::string, int > map;

	map["miau"]++;

	std::cout << map["miau"];
}