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

int main() {
	system("start powershell -Command \"Get-Content -Path 'log.txt' -Wait\"");
	base::Board board(3);


	while (true) {
		system("cls");

		board.renderBoard();

		int x, y;
		char ch;
		base::CombatCardType type;
		std::cin >> x >> y >> ch;

		switch (ch)
		{
			using enum base::CombatCardType;
		case '1':
			type = ONE;
			break;
		case '2':
			type = TWO;
			break;
		case '3':
			type = THREE;
			break;
		case 'E':
			type = ETER;
			break;
		default:
			break;
		}

		board.appendMove(
			{ x, y }, std::make_unique<base::CombatCard>(type)
		);

	}
}