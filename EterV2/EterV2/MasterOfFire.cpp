#include"MasterOfFire.h"

namespace base {

	MasterOfFireFront::MasterOfFireFront() {
		m_type = MageType::Fire;
		m_ability = MageTypeAbility::Burn;
	}

	void MasterOfFireFront::apply(const Coord& coord, Board& board, const Player& player) {
		
	}
	

	MasterOfFireBack::MasterOfFireBack() {
		m_type = MageType::Fire;
		m_ability = MageTypeAbility::BurnRowOrColumn;
	}

	void MasterOfFireBack::apply(const Coord& coord, Board& board, const Player& player) {

	}

    // MasterOfEarthFront
    MasterOfEarthFront::MasterOfEarthFront() {
        m_type = MageType::Earth;
        m_ability = MageTypeAbility::Bury;
    }

    void MasterOfEarthFront::apply(const Coord& coord, Board& board, const Player& player) {
        // TODO: Implement the ability logic for Bury
    }

    // MasterOfEarthBack
    MasterOfEarthBack::MasterOfEarthBack() {
        m_type = MageType::Earth;
        m_ability = MageTypeAbility::Hole;
    }

    void MasterOfEarthBack::apply(const Coord& coord, Board& board, const Player& player) {
        // TODO: Implement the ability logic for Hole
    }

    // MasterOfAirFront
    MasterOfAirFront::MasterOfAirFront() {
        m_type = MageType::Air;
        m_ability = MageTypeAbility::BlowAway;
    }

    void MasterOfAirFront::apply(const Coord& coord, Board& board, const Player& player) {
        // TODO: Implement the ability logic for BlowAway
    }

    // MasterOfAirBack
    MasterOfAirBack::MasterOfAirBack() {
        m_type = MageType::Air;
        m_ability = MageTypeAbility::BlowEter;
    }

    void MasterOfAirBack::apply(const Coord& coord, Board& board, const Player& player) {
        // TODO: Implement the ability logic for BlowEter
    }

    // MasterOfWaterFront
    MasterOfWaterFront::MasterOfWaterFront() {
        m_type = MageType::Water;
        m_ability = MageTypeAbility::Boat;
    }

    void MasterOfWaterFront::apply(const Coord& coord, Board& board, const Player& player) {
        // TODO: Implement the ability logic for Boat
    }

    // MasterOfWaterBack
    MasterOfWaterBack::MasterOfWaterBack() {
        m_type = MageType::Water;
        m_ability = MageTypeAbility::BoatRowOrColumn;
    }

    void MasterOfWaterBack::apply(const Coord& coord, Board& board, const Player& player) {
        // TODO: Implement the ability logic for BoatRowOrColumn
    }

}