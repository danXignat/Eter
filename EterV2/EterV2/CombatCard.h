#pragma once

#include <iostream>
#include <cstdint>

#include "colors.h"

namespace base {
	enum class CombatCardType : uint16_t {
		NONE =0,
		ONE = 1,
		TWO = 2,
		THREE = 3,
		FOUR = 4,
		ETER = 5
	};

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

		void revealIllusion();
		void setIllusion();
		bool isIllusion() const;
		bool attemptCover(const CombatCard& opponentCard) const;

		bool operator<(const CombatCard& other) const;
		bool operator==(const CombatCard& other) const;

		friend std::ostream& operator<<(std::ostream& out, const CombatCard& other);

	private:
		CombatCardType m_type;
		color::ColorType m_color;
		bool m_illusion;
	};
}