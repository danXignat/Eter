#include "PowerCards.h"

using namespace logger;

namespace base {

    ////------------------------------------------ ControllerExplosion -------------------------------------------
    ControllerExplosion::ControllerExplosion() {
        m_ability = PowerCardType::ControllerExplosion;
    }

    void ControllerExplosion::apply(Board& board, Player& player) {
    }

    ////------------------------------------------ Destruction -------------------------------------------
    Destruction::Destruction() {
        m_ability = PowerCardType::Destruction;
    }
  
    void Destruction::apply(Board& board, Player& player) {
        for (auto& [coord, stack] : board) {
            auto top_card = board.getTopCard(coord);
            CombatCard& card = top_card->get();
            if (card.getColor() != player.getColor()) {
                Logger::log(Level::INFO, "Destruction power destroyed the last card");
                board.removeTopCardAt(coord);
                board.availableSpaces();
                break;
            }
        }
    }
    ////------------------------------------------ Flame -------------------------------------------
    Flame::Flame() {
        m_ability = PowerCardType::Flame;
    }

    void Flame::apply(Board& board, Player& player) {
        for (const auto& [coord, stack] : board) {
            auto top_card = board.getTopCard(coord);
            CombatCard& card = top_card->get();
            if (card.getColor() != player.getColor() && card.isIllusion()) {
                card.flip();
                Logger::log(Level::INFO, "The opponent's Illusion has been revealed");
              
            }
            Logger::log(Level::INFO, "Flame power was used");

        }
    }

    ////------------------------------------------ Flame -------------------------------------------

    Fire::Fire() {
        m_ability = PowerCardType::Fire;
    }

    void Fire::apply(Board& board, Player& player) {
        return;
    }


    ////------------------------------------------ Ash -------------------------------------------
    Ash::Ash() {
        m_ability = PowerCardType::Ash;
    }

    void Ash::apply(Board& board, Player& player) {
    }


    ////------------------------------------------ Spark -------------------------------------------
    Spark::Spark() {
        m_ability = PowerCardType::Spark;
    }

    void Spark::apply(Board& board, Player& player) {
    }

    ////------------------------------------------ Squall -------------------------------------------
    Squall::Squall() {
        m_ability = PowerCardType::Squall;
    }
    //std::vector<std::pair<Coord, CombatCard>> Squall::opponentCards(Board& board, Player& opponent) {
    //    std::vector<std::pair<Coord, CombatCard>> visible_cards;
    //    for (auto& [coord, stack] : board) {
    //        auto top_card = board.getTopCard(coord);

    //        if (top_card && top_card->get().getColor() == opponent.getColor()) {
    //            visible_cards.emplace_back(coord, top_card->get());
    //        }
    //    }

    //    return visible_cards;
    //}

    void Squall::apply(Board& board, Player& player) {
    /*    auto visible_cards = opponentCards(board, player);
        std::cout << "Select a card to return to the opponent's hand" << '\n';

        for (auto& [coord, stack] : visible_cards) {
            std::cout << "Card at " << coord.second << ", " << coord.first << ":" << '\n';
        }
        uint8_t choice;
        std::cout << "Enter the number of the card you want to return: ";
        std::cin >> choice;
        
        auto& [coord, card] = visible_cards[choice - 1];
        board.removeTopCardAt(coord);
        Logger::log(Level::INFO, "Squall ability: Returned a visible card to the opponent's hand.")*/;
    }

    ////------------------------------------------ Gale -------------------------------------------
    Gale::Gale() {
        m_ability = PowerCardType::Gale;
    }

    void Gale::apply(Board& board, Player& player) {
    }


    ////------------------------------------------ Hurricane -------------------------------------------
    Hurricane::Hurricane() {
        m_ability = PowerCardType::Hurricane;
    }

    void Hurricane::apply(Board& board, Player& player) {
    }


    ////------------------------------------------ Gust -------------------------------------------
    Gust::Gust() {
        m_ability = PowerCardType::Gust;
    }

    void Gust::apply(Board& board, Player& player) {
    }


    ////------------------------------------------ Mirrage -------------------------------------------
    Mirrage::Mirrage() {
        m_ability = PowerCardType::Mirrage;
    }

    void Mirrage::apply(Board& board, Player& player) {
    }

    ////------------------------------------------ Storm -------------------------------------------
    Storm::Storm() {
        m_ability = PowerCardType::Storm;
    }

    void Storm::apply(Board& board, Player& player) {
    }

    ////------------------------------------------ Tide -------------------------------------------
    Tide::Tide() {
        m_ability = PowerCardType::Tide;
    }

    void Tide::apply(Board& board, Player& player) {
    }


    ////------------------------------------------ Mist -------------------------------------------
    Mist::Mist() {
        m_ability = PowerCardType::Mist;
    }

    void Mist::apply(Board& board, Player& player) {
    }


    ////------------------------------------------ Wave -------------------------------------------
    Wave::Wave() {
        m_ability = PowerCardType::Wave;
    }

    void Wave::apply(Board& board, Player& player) {
    }

    ////------------------------------------------ Whirlpool -------------------------------------------
    Whirlpool::Whirlpool() {
        m_ability = PowerCardType::Whirlpool;
    }

    void Whirlpool::apply(Board& board, Player& player) {
    }

    ////------------------------------------------ Blizzard -------------------------------------------
    Blizzard::Blizzard() {
        m_ability = PowerCardType::Blizzard;
    }

    void Blizzard::apply(Board& board, Player& player) {
    }

    ////------------------------------------------ Waterfall -------------------------------------------
    Waterfall::Waterfall() {
        m_ability = PowerCardType::Waterfall;
    }

    void Waterfall::apply(Board& board, Player& player) {
    }


    ////------------------------------------------ Support -------------------------------------------
    Support::Support() {
        m_ability = PowerCardType::Support;
    }

    void Support::apply(Board& board, Player& player) {
    }


    ////------------------------------------------ Earthquake -------------------------------------------
    Earthquake::Earthquake() {
        m_ability = PowerCardType::Earthquake;
    }

    void Earthquake::apply(Board& board, Player& player) {
    }
        

    ////------------------------------------------ Crumble -------------------------------------------
    Crumble::Crumble() {
        m_ability = PowerCardType::Crumble;
    }

    void Crumble::apply(Board& board, Player& player) {
    }


    ////------------------------------------------ Border -------------------------------------------
    Border::Border() {
        m_ability = PowerCardType::Border;
    }

    void Border::apply(Board& board, Player& player) {
    }


    ////------------------------------------------ Avalanche -------------------------------------------
    Avalanche::Avalanche() {
        m_ability = PowerCardType::Avalanche;
    }

    void Avalanche::apply(Board& board, Player& player) {
    }


    ////------------------------------------------ Rock -------------------------------------------
    Rock::Rock() {
        m_ability = PowerCardType::Rock;
    }

    void Rock::apply(Board& board, Player& player) {
    }

}
