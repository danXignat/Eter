#include <iostream>
#include <conio.h>

#include "IGameMode.h"
#include "GameModeFactory.h"
#include "typedefs.h"
#include "MageService.h"
#include "Board.h"
#include"ElementalMode.h"

int main() {
	//system("start powershell -Command \"Get-Content -Path 'log.txt' -Wait\"");

	base::GameModePtr game_mode = base::GameModeFactory::get("312", "titi", "gigi");

	game_mode->run();
	
	/*base::MageService service;

	service.selectMages();*/
	

	/*base::Board board{ 3 };

	board.appendMove({ 10, 5 }, base::CombatCard(base::CombatCardType::FOUR, color::ColorType::BLUE));
	board.appendMove({ 12, 5 }, base::CombatCard(base::CombatCardType::FOUR, color::ColorType::BLUE));
	board.appendMove({ 14, 5 }, base::CombatCard(base::CombatCardType::FOUR, color::ColorType::BLUE));
	board.renderBoard();

	board.removeRow(5);

	std::cin.get();

	system("cls");
	board.renderBoard();*/
}