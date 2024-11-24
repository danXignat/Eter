#pragma once
#include"MageCard.h"

namespace base {
	class MasterOfFire : public MageCard {
	public:
		MasterOfFire(MageType type, MageTypeAbility front_ability, MageType back_ability);

		void applyAbility()override;
	};
}