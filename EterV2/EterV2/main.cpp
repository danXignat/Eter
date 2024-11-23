#include <iostream>
#include <conio.h>

#include "GameModeFactory.h"

#include "Signal.h"

#include "ExplosionCard.h"

int main() {
	system("start powershell -Command \"Get-Content -Path 'log.txt' -Wait\"");

	base::GameModePtr game_mode = base::GameModeFactory::get("1110000", "titi", "gigi");

	game_mode->run();

}