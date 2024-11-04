export module CombatCardType;

import <cstdint>;

namespace base {
	export enum class CombatCardType : uint8_t {
		Eter,
		One,
		Two,
		Three,
		Four
	};
}