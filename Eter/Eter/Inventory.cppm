export module Inventory;


import <optional>;
import <iostream>;
import <string>;
import <string_view>;
import <unordered_map>;
import <memory>;
import <array>;
import <vector>;

import CombatCard;
import CombatCardType;
import GameModeTypes;
import Teams;
import Logger;
  


namespace base {
    using CardPtr = std::unique_ptr<CombatCard>;

    export class Inventory {
    public:
        Inventory(teams::Team team);
        bool hasCards() const;
        void _initializeCards(GameModeTypes);
        void _addCard(CombatCardType, uint16_t);


       
    private:
        teams::Team m_team;
        std::unordered_multimap<CombatCardType, CardPtr> m_cards;
        bool m_illusion_used;

    };
}