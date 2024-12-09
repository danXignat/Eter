#pragma once
#include<cstdint>
#include<string>
#include<unordered_map>

namespace base {
	enum class MageTypeAbility :uint16_t {
		Burn,
		BurnRowOrColumn,

		Bury,
		Hole,

		BlowAway,
		BlowEter,

		Boat,
		BoatRowOrColumn,

		None
	};

	enum class MageType : uint16_t {
		Fire,
		Earth,
		Air,
		Water,
		None
	};

	const std::unordered_map<MageType, std::string_view> mapMageTypes = {
		{MageType::Fire, "Fire"},
		{MageType::Earth, "Earth"},
		{MageType::Air, "Air"},
		{MageType::Water, "Water"}
	};

	const std::unordered_map<MageTypeAbility, std::string_view> mapMageMasters = {
		{MageTypeAbility::Burn, "Burn"},
		{MageTypeAbility::BurnRowOrColumn, "BurnRowOrColumn"},
		{MageTypeAbility::Bury, "Bury"},
		{MageTypeAbility::Hole, "Hole"},
		{MageTypeAbility::BlowAway, "BlowAway"},
		{MageTypeAbility::BlowEter, "BlowEter"},
		{MageTypeAbility::Boat, "Boat"},
		{MageTypeAbility::BoatRowOrColumn, "BoatRowOrColumn"}
	};

	const std::unordered_map<MageTypeAbility, std::string_view> mapMageTypeAbilities = {
		{MageTypeAbility::Burn,"Remove from play an opponent's card that covers one of your cards"},
		{MageTypeAbility::BurnRowOrColumn,"Remove from play a row or a column of 3 or more cards"},
		{MageTypeAbility::Bury,"Cover an opponent's card with your card of a lower value"},
		{MageTypeAbility::Hole,"Make a hole"},
		{MageTypeAbility::BlowAway,"Move your cards to a diff, empty space"},
		{MageTypeAbility::BlowEter,"Additional Eter"},
		{MageTypeAbility::Boat,"Move ur opponent's stack to a diff, empty space"},
		{MageTypeAbility::BoatRowOrColumn,"Redefine the shape of the playing field"}
	};

	inline std::string_view typeToStrings(MageType type) {
		return mapMageTypes.at(type);
	}

	inline std::string_view masterToString(MageTypeAbility type) {
		return mapMageMasters.at(type);
	}

	inline std::string_view getMasterDescription(MageTypeAbility ability) {
		return mapMageTypeAbilities.at(ability);
	}

}