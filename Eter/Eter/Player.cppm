export module Player;

import CombatCard;
import CombatCardType;

import <string>;
import <string_view>;
import <unordered_map>;
import <memory>;
import <iostream>;

export enum class GameMode {
    TrainingMode,
    MageDuel
};

namespace player {
    export class Player {
    public:
        using CardPtr = std::unique_ptr<base::CombatCard>;

        Player(std::string_view name, GameMode mode);
        ~Player();

        std::string_view getName() const;
         std::unordered_multimap<base::CombatCardType, CardPtr> getCard() const;
        void addCard(std::unique_ptr<base::CombatCard> card);
        void showCards() const;
        bool hasCard() const;
        std::unique_ptr<base::CombatCard> eraseCard(base::CombatCardType type);

    private:
        void _initializeCards(GameMode mode);

    private:
        std::string m_name;
        std::unordered_multimap<base::CombatCardType, CardPtr> m_cards;
    };
}
