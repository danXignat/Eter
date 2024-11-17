module Inventory;

import <iostream>;
import <string>;
import <string_view>;
import <unordered_map>;
import <memory>;

import CombatCard; 
import CombatCardType;
import GameModeTypes;
import Teams;
import Logger;

using namespace logger;


namespace base {
    Inventory::Inventory(teams::Team team) : m_team(team), m_illusion_used(false) {}


    //-------------------------------------------Initialisation------------------------------------

    void Inventory::_addCard(CombatCardType type, uint16_t count) {
        for (uint16_t i = 0; i < count; i++) {
            m_cards.emplace(type, std::make_unique<base::CombatCard>(type, m_team));
        }
    }


    void Inventory::_initializeCards(GameModeTypes mode) {
        using enum CombatCardType;
        switch (mode) {
        case GameModeTypes::Training:
            _addCard(CombatCardType::ONE, 2);
            _addCard(CombatCardType::TWO, 2);
            _addCard(CombatCardType::THREE, 2);
            _addCard(CombatCardType::FOUR, 1);
            break;

        case GameModeTypes::ElementalBattle:
            _addCard(CombatCardType::TWO, 1);
            _addCard(CombatCardType::THREE, 1);
            _addCard(CombatCardType::ETER, 1);
            break;
        }
    }

    bool Inventory::hasCards() const {
        return !m_cards.empty();
    }

  
}