export module MageCardTypeAbility;
import<cstdint>;
import<string>;
import<unordered_map>;

namespace base {
	export enum class MageCardTypeAbility :uint16_t {
		Burn,
		BurnRowOrColumn,
		Bury,
		Hole,
		BlowAway,
		BlowEter,
		Boat,
		BoatRowOrColumn
	};
	const std::unordered_map<MageCardTypeAbility, std::string_view>mapMageTypeAbilities = {
		{MageCardTypeAbility::Burn,"Remove from play an opponent's card that covers one of your cards"},
		{MageCardTypeAbility::BurnRowOrColumn,"Remove from play a row or a column of 3 or more cards"},
		{MageCardTypeAbility::Bury,"Cover an opponent's card with your card of a lower value"},
		{MageCardTypeAbility::Hole,"Make a hole"},
		{MageCardTypeAbility::BlowAway,"Move your cards to a diff, empty space"},
		{MageCardTypeAbility::BlowEter,"Additional Eter"},
		{MageCardTypeAbility::Boat,"Move ur opponent's card to a diff, empty space"},
		{MageCardTypeAbility::BoatRowOrColumn,"Redefine the shape of the playing field"}
	};
	export std::string_view abilityToString(MageCardTypeAbility ability) {
		return mapMageTypeAbilities.at(ability);

	}
}