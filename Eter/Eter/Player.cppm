export module Player;

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

namespace base {
    export enum class PlayerType {
        ONE,
        TWO
    };

    using CardPtr = std::unique_ptr<CombatCard>;

    export class Player {
    public:
        Player(std::string_view, GameModeTypes, teams::Team);
        ~Player();

        std::string getName() const;
        void setName(std::string_view name);
        bool hasCards() const;
        teams::Team getTeam() const;
        std::optional<CardPtr> getCard(CombatCardType);
        void addCard(std::unique_ptr<CombatCard>&&);

    private:
        void _initializeCards(GameModeTypes);
        void _addCard(CombatCardType, uint16_t);

    private:
        teams::Team m_team;
        std::string m_name;
        std::unordered_multimap<CombatCardType, CardPtr> m_cards;
    };
}
