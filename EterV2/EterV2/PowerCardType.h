#pragma once

#include<cstdint>
#include<unordered_map>
#include<string_view>
namespace base {
	 enum class PowerCardType : uint8_t {
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
 const std::unordered_map<PowerCardType, std::string_view> mapPowers = {
		{PowerCardType::ControllerExplosion,"ControllerExplosion"},
		{PowerCardType::Destruction,"Destruction"},
		{PowerCardType::Flame,"Flame"},
		{PowerCardType::Fire,"Fire"},
		{PowerCardType::Ash,"Ash"},
		{PowerCardType::Spark,"Spark"},
		{PowerCardType::Squall,"Squall"},
		{PowerCardType::Gale,"Gale"},
		{PowerCardType::Hurricane,"Hurricane"},
		{PowerCardType::Gust,"Gust"},
		{PowerCardType::Mirrage,"Mirrage"},
		{PowerCardType::Storm,"Storm"},
		{PowerCardType::Tide,"Tide"},
		{PowerCardType::Mist,"Mist"},
		{PowerCardType::Wave,"Wave"},
		{PowerCardType::Whirlpool,"Whirlpool"},
		{PowerCardType::Blizzard,"Blizzard"},
		{PowerCardType::Waterfall,"Waterfall" },
		{PowerCardType::Support,"Support"},
		{PowerCardType::Earthquake,"Earthquake"},
		{PowerCardType::Crumble,"Crumble" },
		{PowerCardType::Border,"Border" },
		{PowerCardType::Avalanche,"Avalanche" },
		{PowerCardType::Rock,"Rock" }
 };

 const std::unordered_map<PowerCardType, std::string_view> mapPowerTypeAbilities = {
		{PowerCardType::ControllerExplosion,"Cause a Power Explosion."},
		{PowerCardType::Destruction, " Remove from play the last card played by the opponent."},
		{PowerCardType::Flame," Reveal an opponent's Illusion and next place your card on any space on the playing field."},
		{PowerCardType::Fire,"Return to their owners' hands all visible cards of the same value (at least 2 of them) chosen by you - also yours! In case of stacks remove only the top card."},
		{PowerCardType::Ash," Rise like a Phoenix!  If one of your cards was removed from play, you can immediately play it again."},
		{PowerCardType::Spark,"Take any of your cards that is covered by an opponent's cards and play it onto a different space. In case of stacks, you can choose any of your covered cards."},
		{PowerCardType::Squall,"Return to the opponent's hand any one of his visible cards on the playing f ield"},
		{PowerCardType::Gale," Remove any cards that are covered by others (yours as well) - they are returned to their owners'  hands. "},
		{PowerCardType::Hurricane," Move any one full row / column by one card so that the first card/ stack is moved off the playing f ield. Cards moved outside the playing field return to their owners' hands."},
		{PowerCardType::Gust,"Move any card on the playing field horizontally or vertically onto a neighboring card of a lower value. Affects individual cards not stacks."},
		{PowerCardType::Mirrage," Exchange your Illusion (face-down card) with a different card from your hand (face-down)."},
		{PowerCardType::Storm," Remove from play any stack of minimum 2 cards."},
		{PowerCardType::Tide,"Change positions of 2 different cards or stacks of cards. "},
		{PowerCardType::Mist," Play an Illusion again. You cannot have 2 Illusions at the same time."},
		{PowerCardType::Wave," Move any card/stack to the adjacent empty space, then play a card from your hand to this now empty space."},
		{PowerCardType::Whirlpool,"Move onto empty space two cards from different adjacent spaces in a straight line. The card of lower value goes to the bottom of the stack. "},
		{PowerCardType::Blizzard,"Place this card onto any space and choose a row or column that it will impact. The opponent cannot play any cards onto the chosen row/column during his next turn. Condition - your opponent must have a free space to play a card. Remove this card from play after the next opponent turn. "},
		{PowerCardType::Waterfall," Move all cards in a chosen column into one stack at the bottom of that column, placing them on top of each other, regardless of their values. The card that was visible on the top of the chosen column stays at the top of the new stack. Condition - the chosen column must contain of minimum 3 cards." },
		{PowerCardType::Support,"Value of a chosen card is increased by +1 (cannot be used on a '4' card). Place your marker on the card. If the chosen card is ever covered or returned to your hand, the marker is removed. In case of a tie, the marker (if present on the playing field), is worth +1 point."},
		{PowerCardType::Earthquake,"Remove from play all visible '1' cards"},
		{PowerCardType::Crumble,"Value of a card is decreased by 1 (cannot be used on a '1' card).  Place your marker on the card. If the chosen card is ever covered or returned to your hand, the marker is removed. In case of a tie, the marker (if present on the playing f ield), is worth -1 point." },
		{PowerCardType::Border,"Place this card in any empty space on the playing f ield to establish the border of the play area. Next, play your normal card. Any player may replace the Border card with any of their cards (it does not block the space). Condition - when played,this card must define one of the borders of the playing area." },
		{PowerCardType::Avalanche," Move two neighboring cards/stacs by one space horizontally or vertically. The space that the f irst card moves onto must be empty." },
		{PowerCardType::Rock," Cover any Illusion card with your card, without revealing the value of the Illusion. " }

 };

 inline std::string_view typeToStrings(PowerCardType type) {
	 return mapPowers.at(type);
 }


 inline std::string_view abilityToString(PowerCardType ability) {
	 return mapPowerTypeAbilities.at(ability);
 }
}
