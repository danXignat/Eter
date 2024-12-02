#pragma once
#include <string>
#include <optional>

#include "BaseGameMode.h"
#include "WinManager.h"
#include "ExplosionService.h"
#include "ElementalService.h"
#include "ServiceType.h"

namespace base {
	class ElementalMode : public BaseGameMode {
	public:
		ElementalMode(const std::vector<ServiceType>&, const std::pair<std::string, std::string>&);
		void run() override;
		void render();

	private:
		std::optional<IllusionService> m_illusion_service;
		std::optional<ExplosionService> m_explosion_service;
		ElementalService m_elemental_service;
	};
}