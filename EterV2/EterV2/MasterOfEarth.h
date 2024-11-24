#pragma once
#include"MageCard.h"

namespace base {
	class MasterOfEarth : public MageCard {
	public:
		MasterOfEarth(MageType type, MageTypeAbility front_ability, MageType back_ability);

		void applyAbility() override;
	};
}