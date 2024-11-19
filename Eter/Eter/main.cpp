#include <optional>
#include <conio.h>
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
import ExplosionCard;

int main() {
	system("start powershell -Command \"Get-Content -Path 'log.txt' -Wait\"");

	//base::TrainingMode mode("gigi", "titi");
	//mode.gameLoop();
	//
	base::Explosion exp(3);

	exp.render();

	while (true) {
		if (_kbhit()) {
			char ch = _getch();

			if (ch == -32) {
				ch = _getch();

				switch (ch) {
				case 77:
					exp.rotateRight();
					exp.render();
					break;

				case 75:
					exp.rotateLeft();
					exp.render();
					break;

				default:
					break;
				}
			}
		}
	}

	return 0;
}