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
		void _handleSpecialEvent(const InputHandler&) override;
	private:
		std::optional<IllusionService> m_illusion_service;
		std::optional<ExplosionService> m_explosion_service;
		MageService m_mage_service;
	};
}