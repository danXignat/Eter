export module ElementalPowerCardType;

import<cstdint>;
import<unordered_map>;
import<string>;

namespace base {

	export enum class ElementalPowerCardType : uint8_t {

		ControllerExplosion,
		Destruction,
		Flame,
		Fire,
		Ash,
		Spark,
		Squall,
		Gale,
		Hurricane,
		Gust,
		Mirrage,
		Storm,
		Tide,
		Mist,
		Wave,
		Whirlpool,
		Blizzard,
		Waterfall,
		Support,
		Earthquake,
		Crumble,
		Border,
		Avalanche,
		Rock
	};
	export const std::unordered_map<ElementalPowerCardType, std::string_view> mapPowers = {

		{ElementalPowerCardType::ControllerExplosion,"ControllerExplosion"},
		{ElementalPowerCardType::Destruction,"Destruction"},
		{ElementalPowerCardType::Flame,"Flame"},
		{ElementalPowerCardType::Fire,"Fire"},
		{ElementalPowerCardType::Ash,"Ash"},
		{ElementalPowerCardType::Spark,"Spark"},
		{ElementalPowerCardType::Squall,"Squall"},
		{ElementalPowerCardType::Gale,"Gale"},
		{ElementalPowerCardType::Hurricane,"Hurricane"},
		{ElementalPowerCardType::Gust,"Gust"},
		{ElementalPowerCardType::Mirrage,"Mirrage"},
		{ElementalPowerCardType::Storm,"Storm"},
		{ElementalPowerCardType::Tide,"Tide"},
		{ElementalPowerCardType::Mist,"Mist"},
		{ElementalPowerCardType::Wave,"Wave"},
		{ElementalPowerCardType::Whirlpool,"Whirlpool"},
		{ElementalPowerCardType::Blizzard,"Blizzard"},
		{ElementalPowerCardType::Waterfall,"Waterfall" },
		{ElementalPowerCardType::Support,"Support"},
		{ElementalPowerCardType::Earthquake,"Earthquake"},
		{ElementalPowerCardType::Crumble,"Crumble" },
		{ElementalPowerCardType::Border,"Border" },
		{ElementalPowerCardType::Avalanche,"Avalanche" },
		{ElementalPowerCardType::Rock,"Rock" }

	};
}