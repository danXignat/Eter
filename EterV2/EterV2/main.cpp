#include <iostream>

#include "GameModeFactory.h"
#include "logger.h"

int main() {
	base::GameModePtr game_mode = base::GameModeFactory::get("1000000", "titi", "gigi");

	game_mode->run();
}