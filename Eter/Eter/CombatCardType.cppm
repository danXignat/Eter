export module CombatCardType;

import <cstdint>;

namespace base {
	export enum class CombatCardType : uint8_t {
		ETER,
		ONE,
		TWO,
		THREE,
		FOUR
	};
}