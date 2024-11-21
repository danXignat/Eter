#include <iostream>

#include "GameModeFactory.h"

#include "Signal.h"

int main() {
	system("start powershell -Command \"Get-Content -Path 'log.txt' -Wait\"");

	base::GameModePtr game_mode = base::GameModeFactory::get("1000000", "titi", "gigi");

	game_mode->run();
}