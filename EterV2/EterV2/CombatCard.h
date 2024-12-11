#pragma once

#include <iostream>
#include <cstdint>
#include <optional>

#include "CombatCardType.h"
#include "colors.h"

namespace base {
	class CombatCard {
	public:
		CombatCard();
		CombatCard(CombatCardType type,	color::ColorType color);
		CombatCard(const CombatCard& other) = delete;
		CombatCard& operator=(const CombatCard& other) = delete;
		CombatCard(CombatCard&& other) noexcept;
		CombatCard& operator=(CombatCard&& other) noexcept;

		CombatCardType getType() const;
		color::ColorType getColor() const;

		void flip();
		bool isIllusion() const;

		bool operator<(const CombatCard& other) const;
		bool operator>=(const CombatCard& other) const;
		bool operator==(const CombatCard& other) const;

		friend std::ostream& operator<<(std::ostream& out, const CombatCard& other);

	private:
		CombatCardType m_type;
		color::ColorType m_color;
		bool m_illusion;
	};

	using CombatCardOpt = std::optional<CombatCard>;
}