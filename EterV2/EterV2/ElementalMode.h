#pragma once
#include <string>
#include <optional>

#include "GameMode.h"
#include "WinManager.h"
#include "ExplosionService.h"
#include "ElementalService.h"

namespace base {
	class ElementalMode : public GameMode {
	public:
		ElementalMode(bool, bool, const std::string&, const std::string&);
		void run();

	protected:
		void render();

	private:
		Board m_board;
		WinManager m_win_manager;

		std::optional<IllusionService> m_illusion_service;
		std::optional<ExplosionService> m_explosion_service;
		ElementalService m_elemental_service;
	};
}