#include "PowerCards.h"

using namespace logger;

namespace base {

    ControllerExplosion::ControllerExplosion() {
        m_ability = PowerCardType::ControllerExplosion;
    }

    void ControllerExplosion::apply(Board& board, const Player& player) {
    }

    ////------------------------------------------ Destruction -------------------------------------------
    Destruction::Destruction() {
        m_ability = PowerCardType::Destruction;
    }

    void Destruction::apply( Board& board, const Player& player) {
        for (auto& [coord, stack] : board) {
            auto top_card =board.getTopCard(coord);
            CombatCard& card = top_card->get();
            if (card.getColor() != player.getColor()) {
                board.removeTopCardAt(coord);
                break;
            }
        }
    }
    ////------------------------------------------ Flame -------------------------------------------
    Flame::Flame() {
        m_ability = PowerCardType::Flame;
    }

    void Flame::apply(Board& board, const Player& player) {
        for (const auto& [coord, stack] : board) {
            auto top_card = board.getTopCard(coord);
            CombatCard& card = top_card->get();
            if (card.isIllusion()) {
                card.flip();
            }
        }
        return;
    }

    Fire::Fire() {
        m_ability = PowerCardType::Fire;
    }

    void Fire::apply(Board& board, const Player& player) {
        return;
    }

    Ash::Ash() {
        m_ability = PowerCardType::Ash;
    }

    void Ash::apply(Board& board, const Player& player) {
    }

    Spark::Spark() {
        m_ability = PowerCardType::Spark;
    }

    void Spark::apply(Board& board, const Player& player) {
    }

    Squall::Squall() {
        m_ability = PowerCardType::Squall;
    }

    void Squall::apply(Board& board, const Player& player) {
    }

    Gale::Gale() {
        m_ability = PowerCardType::Gale;
    }

    void Gale::apply(Board& board, const Player& player) {
    }

    Hurricane::Hurricane() {
        m_ability = PowerCardType::Hurricane;
    }

    void Hurricane::apply(Board& board, const Player& player) {
    }

    Gust::Gust() {
        m_ability = PowerCardType::Gust;
    }

    void Gust::apply(Board& board, const Player& player) {
    }

    Mirrage::Mirrage() {
        m_ability = PowerCardType::Mirrage;
    }

    void Mirrage::apply(Board& board, const Player& player) {
    }

    Storm::Storm() {
        m_ability = PowerCardType::Storm;
    }

    void Storm::apply(Board& board, const Player& player) {
    }

    Tide::Tide() {
        m_ability = PowerCardType::Tide;
    }

    void Tide::apply(Board& board, const Player& player) {
    }

    Mist::Mist() {
        m_ability = PowerCardType::Mist;
    }

    void Mist::apply(Board& board, const Player& player) {
    }

    Wave::Wave() {
        m_ability = PowerCardType::Wave;
    }

    void Wave::apply(Board& board, const Player& player) {
    }

    Whirlpool::Whirlpool() {
        m_ability = PowerCardType::Whirlpool;
    }

    void Whirlpool::apply(Board& board, const Player& player) {
    }

    Blizzard::Blizzard() {
        m_ability = PowerCardType::Blizzard;
    }

    void Blizzard::apply(Board& board, const Player& player) {
    }

    Waterfall::Waterfall() {
        m_ability = PowerCardType::Waterfall;
    }

    void Waterfall::apply(Board& board, const Player& player) {
    }

    Support::Support() {
        m_ability = PowerCardType::Support;
    }

    void Support::apply(Board& board, const Player& player) {
    }

    Earthquake::Earthquake() {
        m_ability = PowerCardType::Earthquake;
    }

    void Earthquake::apply(Board& board, const Player& player) {
    }
        
    Crumble::Crumble() {
        m_ability = PowerCardType::Crumble;
    }

    void Crumble::apply(Board& board, const Player& player) {
    }

    Border::Border() {
        m_ability = PowerCardType::Border;
    }

    void Border::apply(Board& board, const Player& player) {
    }

    Avalanche::Avalanche() {
        m_ability = PowerCardType::Avalanche;
    }

    void Avalanche::apply(Board& board, const Player& player) {
    }

    Rock::Rock() {
        m_ability = PowerCardType::Rock;
    }

    void Rock::apply(Board& board, const Player& player) {
    }

}
