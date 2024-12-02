#pragma once
#include <cstdint>

namespace base {
	enum class CombatCardType : uint16_t {
		BORDER,
		ONE,
		TWO,
		THREE,
		FOUR,
		ETER,
		HOLE,
		NONE
	};

	inline CombatCardType charToCombatCard(char ch) {
		switch (ch) {
			using enum CombatCardType;
		case '1':
			return ONE;
		case '2':
			return TWO;
		case '3':
			return THREE;
		case '4':
			return FOUR;
		case 'E':
			return ETER;
		case 'H':
			return HOLE;
		default:
			return NONE;
		}
	}

	inline char combatCardToChar(CombatCardType type) {
		switch (type) {
			using enum CombatCardType;
		case ONE: 
			return '1';
		case TWO:
			return '2';
		case THREE:
			return '3';
		case FOUR:
			return '4';
		case ETER: 
			return 'E';
		case HOLE:
			return 'H';
		default: 
			return 'N';
		}
	}

}