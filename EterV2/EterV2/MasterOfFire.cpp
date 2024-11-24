//#include"MasterOfFire.h"
//
//namespace base {
//	
//	MasterOfFire::MasterOfFire():
//		MageCard(MageType::Fire,MageTypeAbility::Burn,MageTypeAbility::BurnRowOrColumn){
//	}
//
//	void MasterOfFire::applyAbility(const Coord& coord, Board& board, const Player& player) {
//		if (m_face_up) {
//			board.removeTopCardAt(coord, player);
//		}
//		else {
//			board.removeColumn(coord, player);
//		}
//		
//	}
//	 
//}