#include"MasterOfFire.h"

namespace base {
	
	MasterOfFire::MasterOfFire(MageType type, MageTypeAbility front_ability, MageType back_ability):
		MageCard(MageType::Fire,MageTypeAbility::Burn,MageTypeAbility::BurnRowOrColumn){
	}

	void MasterOfFire::applyAbility() {
	}
	 
}