#include <iostream>
#include <conio.h>

#include "GameModeFactory.h"
#include "ExplosionService.h"
#include "Board.h"
#include "Player.h"
#include "Config.h"
#include"ArenaService.h"

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
	using namespace base;

	Config& config = Config::getInstance();		//builder design pattern

	config.setStartPoint({ 10, 5 })
		  .setCardSpacingX(100)
		  .setCardSpacingY(200);

	for (auto& [label, value] : config.getConfig()) {
		if (std::holds_alternative<uint16_t>(value)) {
			std::cout << label << ": " << std::get<uint16_t>(value) << std::endl;
		}
		else if (std::holds_alternative<Coord>(value)) {
			std::cout << label << ": " <<
				std::get<Coord>(value).first << ", " <<
				std::get<Coord>(value).second << std::endl;
		}
	}

	for (const auto& [x, y] : config.getOffsets()) {
		std::cout << x << " " << y << std::endl;
	}
}
void testArenaService() {

	base::Player player1("Player1", color::ColorType::RED, base::GameSizeType::BIG);
	base::Player player2("Player2", color::ColorType::BLUE, base::GameSizeType::BIG);

	base::Board board(base::GameSizeType::BIG, player1, player2);

	board.appendMove({ 10, 5 }, base::CombatCard(base::CombatCardType::FOUR, color::ColorType::BLUE));
	board.appendMove({ 12, 5 }, base::CombatCard(base::CombatCardType::THREE, color::ColorType::RED));
	board.appendMove({ 14, 5 }, base::CombatCard(base::CombatCardType::FOUR, color::ColorType::BLUE));
	board.appendMove({ 16, 5 }, base::CombatCard(base::CombatCardType::FOUR, color::ColorType::BLUE));

	board.render();
	board.sideViewRender();

	base::ArenaService arenaService(base::GameSizeType::BIG, player1, player2);
	arenaService.renderArena(board);
}

int main() {
	system("start powershell -Command \"Get-Content -Path 'log.txt' -Wait\"");

	base::Config::getInstance()
		.setStartPoint({ 10, 5 })
		.setCardSpacingX(2)
		.setCardSpacingY(1);

	base::GameModePtr game_mode{ base::GameModeFactory::get("212", { "titi", "gigi" }) };
	game_mode->run();

	//testConfig();

	//testExplosion();
	//testArenaService();
}

