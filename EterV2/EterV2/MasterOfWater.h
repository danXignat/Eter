#pragma once
#include"MageCard.h"

namespace base {
	class MasterOfWater : public MageCard {
	public:
		MasterOfWater(MageType type, MageTypeAbility front_ability, MageType back_ability);

		void applyAbility() override;
	};
}