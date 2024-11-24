//#pragma once
//#include<string>
//#include"MageTypeAbility.h"
//#include<iostream>
//#include"Board.h"
//#include"Player.h"
//#include"utils.h"
//#include<vector>
//class Board;
//namespace base {
//	class MageCard {
//	public:
//		MageCard(MageType type, MageTypeAbility frontAbility, MageTypeAbility backAbility, bool face_up = true);
//
//		virtual ~MageCard()= default;
//
//		virtual std::string_view getTypeName() const;
//		virtual  std::string_view getFrontAbilityName() const;
//		virtual  std::string_view getBackAbilityName()const;
//		virtual void applyAbility(const Coord& coord,Board& board, const Player& player) = 0;
//		
//		
//	protected:
//		MageType m_mage_type;
//		MageTypeAbility m_mage_front_ability;
//		MageTypeAbility m_mage_back_ability;
//		bool m_face_up;
//	};
//	
//	/*inline std::ostream& operator<<(std::ostream& out, const MageCard& card) {
//		out << "Mage Type: " << card.getTypeName() << "\n"
//			<< "Front Ability: " << card.getFrontAbilityName() << "\n"
//			<< "Back Ability: " << card.getBackAbilityName() << "\n";
//		return out;
//	}*/
//}