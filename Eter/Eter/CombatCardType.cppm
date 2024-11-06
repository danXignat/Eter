export module CombatCardType;

import <cstdint>;

namespace base {
	export enum class CombatCardType : uint8_t {
		ONE = 1,
		TWO = 2,
		THREE = 3,
		ETER = 4
	};
}