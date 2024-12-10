#include <iostream>
#include <conio.h>

#include "IGameMode.h"
#include "GameModeFactory.h"
#include "typedefs.h"
//#include "MageService.h"
#include "Board.h"
#include"ElementalMode.h"
#include"ElementalService.h"

int main() {
	//system("start powershell -Command \"Get-Content -Path 'log.txt' -Wait\"");

	base::GameModePtr game_mode = base::GameModeFactory::get("212", { "titi", "gigi" });

	game_mode->run();
	 /*1 training
	 2 mage 
	 3 power
	 112 training mode cu explozii si iluzii
	 212 mage mode cu explozii si iluzii
	 312 power mode cu explozii si iluzii*/

}