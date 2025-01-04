#include <iostream>
#include <conio.h>

#include "GameModeFactory.h"
#include "ExplosionService.h"
#include "Board.h"
#include "Player.h"
#include "Config.h"

void testExplosion() {
	base::Player p1{ "Ilie", color::ColorType::RED, base::GameSizeType::BIG};
	base::Player p2{ "Cucu", color::ColorType::BLUE, base::GameSizeType::BIG };
	p1.renderCards();
	p2.renderCards();
	
	base::Board board{ base::GameSizeType::BIG, p1, p2};
	board.appendMove({ 10, 5 }, base::CombatCard(base::CombatCardType::FOUR, color::ColorType::BLUE));
	board.appendMove({ 12, 5 }, base::CombatCard(base::CombatCardType::THREE, color::ColorType::RED));
	board.appendMove({ 14, 5 }, base::CombatCard(base::CombatCardType::FOUR, color::ColorType::BLUE));
	board.appendMove({ 16, 5 }, base::CombatCard(base::CombatCardType::FOUR, color::ColorType::BLUE));
	board.appendMove({ 10, 6 }, base::CombatCard(base::CombatCardType::FOUR, color::ColorType::BLUE));
	board.render();
	board.sideViewRender();

	base::ExplosionService service{board, p1, p2};
	service.renderExplosion();

	while (true) {
		service.setting();
		service.apply();
		system("cls");
		board.render();
		board.sideViewRender();
		service.renderExplosion();
		p1.renderCards();
		p2.renderCards();
	}
}

void testConfig() {
	base::Config::getInstance()		//builder design pattern
		.setStartPoint({ 10, 5 })
		.setCardWidth(2)
		.setCardHeight(1);

	for (auto& [label, value] : base::Config::getInstance().getConfig()) {
		if (std::holds_alternative<uint16_t>(value)) {
			std::cout << label << ": " << std::get<uint16_t>(value) << std::endl;
		}
		else if (std::holds_alternative<base::Coord>(value)) {
			std::cout << label << ": " <<
				std::get<base::Coord>(value).first << ", " <<
				std::get<base::Coord>(value).second << std::endl;
		}
	}
}

int main() {
	/*system("start powershell -Command \"Get-Content -Path 'log.txt' -Wait\"");

	base::GameModePtr game_mode{ base::GameModeFactory::get("212", { "titi", "gigi" }) };
	game_mode->run();*/

	testConfig();

	//testExplosion();
}

