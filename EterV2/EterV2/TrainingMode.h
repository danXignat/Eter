#pragma once
#include <string>
#include <optional>

#include "BaseGameMode.h"
#include "IllusionService.h"
#include "ExplosionService.h"
#include "WinManager.h"
#include "ServiceType.h"
#include "Player.h"
#include "typedefs.h"


namespace base {
	class TrainingMode : public BaseGameMode {
	public:
		TrainingMode(const std::vector<ServiceType>&, const std::pair<std::string, std::string>&);
		void run();
		void render();
	};
}