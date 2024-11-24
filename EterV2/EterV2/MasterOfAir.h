#pragma once
#include"MageCard.h"

namespace base {
	class MasterOfAir : public MageCard {
	public:
		MasterOfAir(MageType type, MageTypeAbility front_ability, MageType back_ability);

		void applyAbility() override;
	};
}