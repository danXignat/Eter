#pragma once
#include <string>
#include <optional>

#include "BaseGameMode.h"
#include "WinManager.h"
#include "ExplosionService.h"
#include "ElementalService.h"
#include "ServiceType.h"
#include "GameModeType.h"

namespace base {
	class ElementalMode : public BaseGameMode {
	public:
		ElementalMode(const std::vector<ServiceType>&, const std::pair<std::string, std::string>&);
		void run() override;
		void render();
		bool usePower();

	private:
		ElementalService m_elemental_service;
	};
}