import <iostream>;
import <string>;
import <unordered_map>;
import <memory>;

import utils;

import Game;
import Logger;
import Board;
import CombatCard;
import CombatCardType;

int main() {
	//system("start powershell -Command \"Get-Content -Path 'log.txt' -Wait\"");
	base::Board board(3);

	board.appendMove(
		{10, 8}, std::make_unique<base::CombatCard>(base::CombatCardType::ETER)
	);
	board.appendMove(
		{ 12, 9 }, std::make_unique<base::CombatCard>(base::CombatCardType::ONE)
	);
	board.appendMove(
		{ 14, 9 }, std::make_unique<base::CombatCard>(base::CombatCardType::ETER)
	);

	board.appendMove(
		{ 18, 9 }, std::make_unique<base::CombatCard>(base::CombatCardType::ETER)
	);
	board.renderBoard();

}