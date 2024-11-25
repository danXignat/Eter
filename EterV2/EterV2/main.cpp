#include <iostream>
#include <conio.h>

#include "GameMode.h"
#include "GameModeFactory.h"
#include "typedefs.h"
#include "MageService.h"
#include "Board.h"

int main() {
	/*system("start powershell -Command \"Get-Content -Path 'log.txt' -Wait\"");

	base::GameModePtr game_mode = base::GameModeFactory::get("1110000", "titi", "gigi");

	game_mode->run();*/

	
	base::MageService service;

	service.selectMages();
	

	/*
	base::Board board{ 3 };

	board.appendMove({ 10, 5 }, base::CombatCard(base::CombatCardType::FOUR, color::ColorType::BLUE));
	board.appendMove({ 12, 6 }, base::CombatCard(base::CombatCardType::FOUR, color::ColorType::BLUE));
	board.appendMove({ 12, 5 }, base::CombatCard(base::CombatCardType::FOUR, color::ColorType::BLUE));

	for (const auto& [coord, stack] : board) {
		auto& [x, y] = coord;

		std::cout << x << " " << y << " " << stack.back() << std::endl;
	}
	*/
}