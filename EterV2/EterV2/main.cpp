#include <iostream>
#include <conio.h>

#include "GameModeFactory.h"
#include "Board.h"

int main() {
	system("start powershell -Command \"Get-Content -Path 'log.txt' -Wait\"");

	base::GameModePtr game_mode{ base::GameModeFactory::get("200", { "titi", "gigi" }) };
	 /*1 training
	 2 mage 
	 3 power
	 112 training mode cu explozii si iluzii
	 212 mage mode cu explozii si iluzii
	 312 power mode cu explozii si iluzii*/

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