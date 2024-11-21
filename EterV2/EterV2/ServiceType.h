#pragma once
#include <cstdint>

namespace base {
	enum class ServiceType : uint16_t {
		NONE, 
		ILLUSION,
		EXPLOSION,
		MAGE,
		ELEMENTAL,
		ARENA,
		TIMED
	};

	inline ServiceType charToService(char ch) {
		switch (ch)
		{
			using enum ServiceType;
		case 'i':
			return ILLUSION;
		case 'e':
			return EXPLOSION;
		case 'm':
			return MAGE;
		case 'p':
			return ELEMENTAL;
		case 'a':
			return ARENA;
		case 't':
			return TIMED;
		default:
			return NONE;
		}
	}
}