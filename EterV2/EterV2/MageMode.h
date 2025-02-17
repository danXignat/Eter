#pragma once
#include <string>
#include <optional>

#include "BaseGameMode.h"
#include "WinManager.h"
#include "IllusionService.h"
#include "ExplosionService.h"
#include "MageService.h"
#include "ServiceType.h"
#include "GameModeType.h"

namespace base {
	class MageMode : public BaseGameMode {
	public:
		MageMode(const std::vector<ServiceType>&, const std::pair<std::string, std::string>&);
		void run() override;
		void render();

		MageService& getMageService();

	private:
		MageService m_mage_service;
	};
}