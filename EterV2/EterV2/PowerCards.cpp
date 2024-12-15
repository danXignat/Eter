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
        for (const auto& [coord, stack] : board) {
            auto top_card = board.getTopCard(coord);
            const CombatCard& card = stack.back();
            if (card.getColor() != player.getColor()) {
                Logger::log(Level::INFO, "Destruction power destroyed the last card");
                board.popTopCardAt(coord);
                break;
            }
        }
    }
    ////------------------------------------------ Flame -------------------------------------------
    Flame::Flame() {
        m_ability = PowerCardType::Flame;
    }

    void Flame::apply(Board& board, Player& player) {

        flipIllusion(board, player);
        Logger::log(Level::INFO, "It's your turn, place a card");

        Coord new_coord;
        std::cin >> new_coord.first >> new_coord.second;
        char card_type;
        std::cin >> card_type;

        CombatCard selected_card = player.getCard(charToCombatCard(card_type));

        if (board.isValidPlaceCard(new_coord, selected_card)) {
            board.appendMove(new_coord, std::move(selected_card));
            Logger::log(Level::INFO, "Flame power was used. Card placed at ({}, {})",
                new_coord.first, new_coord.second);
        }
        else {
            Logger::log(Level::WARNING, "Invalid move!");
        }
    }

    void Flame::flipIllusion(Board& board, const Player& player) {
        for (const auto& [coord, stack] : board) {
            const auto& top_card = board.getTopCard(coord);
            CombatCard& card = top_card->get();
            if (card.getColor() != player.getColor() && card.isIllusion()) {
                card.flip();
                Logger::log(Level::INFO, "The opponent's Illusion has been revealed");
            }
            else {
                Logger::log(Level::WARNING, "The card does not meet the conditions");
            }
        }
    }

    ////------------------------------------------ Fire -------------------------------------------

    Fire::Fire() {
        m_ability = PowerCardType::Fire;

    }

    void Fire::apply(Board& board, Player& player) {
        std::vector<Coord>duplicateCoord = getDuplicateCards(board, player);
        if (duplicateCoord.size() > 1) {
            for (const auto& coord : duplicateCoord) {
                auto top_card = board.getTopCard(coord);
                CombatCard& card = top_card->get();
                player.addCard(std::move(card));
                board.removeTopCardAt(coord);

                Logger::log(Level::INFO, "Fire power removed top cards and put then back in the player's hand");
            }
        }
        else {
            Logger::log(Level::WARNING, "No visible cards with the same value on the board");
        }
    }

    std::vector<Coord>Fire::getDuplicateCards(Board& board, const Player& player) {
        std::vector<Coord>choices;
        for (const auto& [coord, stack] : board) {
            const auto& [x, y] = coord;


            bool is_player_card = stack[stack.size() - 1].getColor() == player.getColor();
            if (is_player_card && !stack.back().isIllusion()) {
                choices.emplace_back(x, y);
            }
        }
        return choices;
    }

    ////------------------------------------------ Ash -------------------------------------------
    Ash::Ash() {
        m_ability = PowerCardType::Ash;
    }

    void Ash::apply(Board& board, Player& player) {
        if (player.hasUsedCards()) {
            char card_type;
            std::cin >> card_type;
            const CombatCardType& card_type_enum = charToCombatCard(card_type);
            CombatCard card = player.getUsedCard(card_type_enum);
            Coord coord;
            std::cin >> coord.first >> coord.second;
            if (board.isValidPlaceCard(coord, card)) {
                board.appendMove(coord, std::move(card));
                Logger::log(Level::INFO, "Ash power card was used");
            }
            else {
                Logger::log(Level::WARNING, "Invalid place");
            }
        }
        else {
            Logger::log(Level::WARNING, "You don't have any removed cards");
        }
    }


    ////------------------------------------------ Spark -------------------------------------------
    Spark::Spark() {
        m_ability = PowerCardType::Spark;
    }

    void Spark::apply(Board& board, Player& player) {
        std::vector<std::pair<Coord, CombatCardType>>choices = coverCards(board, player);

        if (choices.empty()) {
            Logger::log(Level::WARNING, "No covered cards");
            return;
        }
        Logger::log(Level::INFO, "It's your turn, place a card");

        for (auto& choice : choices) {
            const Coord& coord = choice.first;
            const CombatCardType& card = choice.second;
            Logger::log(Level::INFO, "Card found at ({}, {})", coord.first, coord.second);
            Logger::log(Level::INFO, "Card:{}", combatCardToChar(card));
        }
        Logger::log(Level::INFO, "It's your turn");
        Coord coord_from;
        Coord new_coord;
        char card_type_char;

        std::cout << "Move from coordinates:" << "\n";
        std::cin >> coord_from.first >> coord_from.second;

        std::cout << "Covered card:" << "\n";
        std::cin >> card_type_char;

        std::cout << "To coordinates:" << "\n";
        std::cin >> new_coord.first >> new_coord.second;


        CombatCardType card_type = charToCombatCard(card_type_char);
        CombatCard card(card_type, player.getColor());

        bool valid_card = false;
        bool valid_coord = false;
        for (const auto& choice : choices) {
            if (choice.first == coord_from) {
                valid_coord = true;
            }
            if (choice.second == card_type)
            {
                valid_card = true;
            }
        }
        if (valid_card && valid_coord) {
            board.removeCardFromStackAt(coord_from, card);
            board.appendMove(new_coord, std::move(card));
        }
        if (!valid_card) {
            Logger::log(Level::WARNING, "Invalid card selected.");
            return;
        }
        if (!valid_coord) {
            Logger::log(Level::WARNING, "Invalid coordinates.");
            return;
        }
    }

    std::vector<std::pair<Coord, CombatCardType>>Spark::coverCards(const Board& board, const Player& player) {
        std::vector<std::pair<Coord, CombatCardType>>choices;
        for (const auto& [coord, stack] : board) {
            if (stack.empty()) {
                continue;
            }
            for (size_t i = 0; i < stack.size() - 1; ++i) {
                const CombatCard& card = stack[i];
                bool player_card = card.getColor() == player.getColor();

                if (player_card) {
                    CombatCardType card_type = card.getType();
                    choices.emplace_back(coord, card_type);
                }
            }
        }
        return choices;
    }

    ////------------------------------------------ Squall -------------------------------------------
    Squall::Squall() {
        m_ability = PowerCardType::Squall;
    }

    void Squall::apply(Board& board, Player& player) {
        auto visible_cards = opponentCards(board, player);
        std::cout << "Select the coordinates where is the card that you want to return to the opponent's hand" << '\n';


        Coord coord;
        std::cin >> coord.first >> coord.second;

        for (auto& pair : visible_cards) {
            Coord available_coord = pair.first;
            CombatCard& card = pair.second;
            if (available_coord != coord) {
                Logger::log(Level::WARNING, "No visible card at these coordinates");
            }
            else {
                player.addCard(std::move(card));
                board.removeTopCardAt(coord);
                Logger::log(Level::INFO, "Squall ability: Returned a visible card to the opponent's hand.");
            }
        }
    }

    std::vector<std::pair<Coord, CombatCard>> Squall::opponentCards(Board& board, const Player& player) const {
        std::vector<std::pair<Coord, CombatCard>> visible_cards;
        for (const auto& [coord, stack] : board) {
            const auto& top_card = board.getTopCard(coord);
            const CombatCard& card = top_card->get();

            if (card.getColor() != player.getColor()) {
                visible_cards.emplace_back(coord, std::move(top_card->get()));
            }
        }
        return visible_cards;
    }

    ////------------------------------------------ Gale -------------------------------------------
    Gale::Gale() {
        m_ability = PowerCardType::Gale;
    }

    void Gale::apply(Board& board, Player& player) {

        for (auto& [coord, stack] : board) {
            if (stack.size() > 1) {
                for (int i = 0;i < stack.size() - 1;i++) {
                    const CombatCardType& card_type = stack[i].getType();
                    auto card = player.getCard(card_type);
                    if (player.getColor() == card.getColor()) {
                        board.removeCardFromStackAt(coord, card);
                        player.addCard(std::move(card));
                        Logger::log(Level::INFO, "Moved card from board to player hand.");
                    }
                    else {
                        board.removeCardFromStackAt(coord, card);
                        player.addCard(std::move(card));
                    }
                }
            }
        }
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
        if (getIllusion(board, player)) {
            std::cout << "Choose the coordinates for the new illusion and the new illusion" << std::endl;
            Coord coord;
            std::cin >> coord.first >> coord.second;

            char card_type;
            std::cin >> card_type;
            auto card = player.getCard(charToCombatCard(card_type));
            card.flip();
            board.appendMove(coord, std::move(card));
            Logger::log(Level::INFO, "Mirrage power card was used");
        }
    }

    bool Mirrage::getIllusion(Board& board, Player& player) {
        bool foundIllusion = false;
        for (const auto& [coord, stack] : board) {
            auto top_card = board.getTopCard(coord);
            CombatCard& card = top_card->get();
            if (card.isIllusion() && card.getColor() == player.getColor()) {
                card.flip();
                player.addCard(std::move(card));
                CombatCard& card_value = card;
                board.removeTopCardAt(coord);
                board.availableSpaces();
                Logger::log(Level::INFO, "You successfully received the illusion back");
                foundIllusion = true;
                break;
            }
        }
        if (!foundIllusion) {
            Logger::log(Level::INFO, "You don't have any illusion on the board");
        }
        return foundIllusion;
    }


    ////------------------------------------------ Storm -------------------------------------------
    Storm::Storm() {
        m_ability = PowerCardType::Storm;
    }

    void Storm::apply(Board& board, Player& player) {
        std::vector<Coord> stacksToRemove;

        for (const auto& [coord, stack] : board) {
            if (stack.size() > 1) {
                stacksToRemove.push_back(coord);
            }
        }
        Coord new_coord;
        std::cin >> new_coord.first >> new_coord.second;

        if (std::find(stacksToRemove.begin(), stacksToRemove.end(), new_coord) == stacksToRemove.end()) {
            Logger::log(Level::WARNING, "No stack at these coordinates");
        }
        else {
            board.popStack(new_coord);
            Logger::log(Level::INFO, "Storm power card remove the stack");
        }
    }

    ////------------------------------------------ Tide -------------------------------------------
    Tide::Tide() {
        m_ability = PowerCardType::Tide;
    }

    void Tide::apply(Board& board, Player& player) {
        std::vector<Coord> stacks = getStacks(board);
        if (stacks.size() > 1) {

            std::cout << "Stack at:" << std::endl;
            for (const auto& [x, y] : stacks) {
                std::cout << std::format("({}, {})", x, y) << std::endl;
            }
            Coord coord_from;
            Coord coord_to;
            std::cout << "Coordinates of the first stack" << std::endl;
            std::cin >> coord_from.first >> coord_from.second;

            if (std::find(stacks.begin(), stacks.end(), coord_from) == stacks.end()) {
                Logger::log(Level::WARNING, "No stack at these coordinates");
            }

            std::cout << "Coordinates of the second stack" << std::endl;
            std::cin >> coord_to.first >> coord_to.second;

            if (std::find(stacks.begin(), stacks.end(), coord_to) == stacks.end()) {
                Logger::log(Level::WARNING, "No stack at these coordinates");
            }

            board.swapStacks(coord_from, coord_to);
        }

        Logger::log(Level::INFO, "Tide power card was used");
    }

    std::vector<Coord>Tide::getStacks(const Board& board) {
        std::vector<Coord>stacks_coord;
        for (const auto& [coord, stack] : board) {
            if (stack.size() > 1) {
                stacks_coord.emplace_back(coord);
            }
            else {
                Logger::log(Level::WARNING, "There are not 2 stacks on the board ");
            }
        }
        return stacks_coord;
    }


    ////------------------------------------------ Mist -------------------------------------------
    Mist::Mist() {
        m_ability = PowerCardType::Mist;

    }

    void Mist::apply(Board& board, Player& player) {
        WinManager win_manager(board);
        IllusionService illusionService(board, win_manager);
        for (const auto& [coord, stack] : board) {
            if (stack.back().isIllusion()) {
                Logger::log(Level::WARNING, "Player already has an illusion");
            }
            else {
                Logger::log(Level::INFO, "You can play your new illusion");


                Coord new_coord;
                std::cin >> new_coord.first >> new_coord.second;

                char card_type;
                std::cin >> card_type;
                CombatCard card = player.getCard(charToCombatCard(card_type));

                illusionService.getNewIllusion(std::move(card));
                if (illusionService.isValidPlaceCard(new_coord, std::move(card))) {
                    illusionService.placeIllusion(coord, std::move(card));
                }
            }
        }
    }
    bool Mist::hasIllusion(Board& board, IllusionService& illusionService, Player& player) {
        for (const auto& [coord, stack] : board) {
            if (stack.back().isIllusion()) {
                Logger::log(Level::WARNING, "Player already has an illusion");
                return false;
            }
            else {
                Logger::log(Level::INFO, "You can play your new illusion");


                Coord new_coord;
                std::cin >> new_coord.first >> new_coord.second;

                char card_type;
                std::cin >> card_type;
                CombatCard card = player.getCard(charToCombatCard(card_type));

                illusionService.getNewIllusion(std::move(card));
                if (illusionService.isValidPlaceCard(new_coord,std::move(card))) {
                    illusionService.placeIllusion(coord, std::move(card));
                }
            }
        }
        return true;
    }
    

    ////------------------------------------------ Wave -------------------------------------------
    Wave::Wave() {
        m_ability = PowerCardType::Wave;
    }

    void Wave::apply(Board& board, Player& player) {
        std::vector<Coord>coord_from = validStacks(board);


        Coord old_coord;
        std::cout << "The coordinates from where you want to move the stack:";
        std::cin >> old_coord.first >> old_coord.second;
        if (std::ranges::find(coord_from, old_coord) == coord_from.end()) {
            Logger::log(Level::WARNING, "No stack at these coordinates");
        }

        Coord new_coord;
        std::cout << "The coordinates where you want to move the stack:";
        std::cin >> new_coord.first >> new_coord.second;

            board.moveStack(old_coord, new_coord);
            char card_type;
            std::cout << "Play a card on the new empty position";
             std::cin >> card_type;
             CombatCard card = player.getCard(charToCombatCard(card_type));
             if (board.isValidPlaceCard(old_coord, card)) {
                 board.appendMove(old_coord, std::move(card));
             }
    }

    std::vector<Coord>Wave::validStacks(Board& board) const {
        std::vector<Coord> stackCoord;
        for (const auto& [coord, stack] : board) {
            if (board.isValidRemoveStack(coord)) {
                stackCoord.push_back(coord);
            }
            return stackCoord;
        }
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
        m_ability = PowerCardType::Support;//to return
    }

    void Support::apply(Board& board, Player& player) {
        std::vector<Coord>validCards = CoordCardType(board, player);

        if (validCards.empty()) {
            Logger::log(Level::WARNING, "No valid cards to apply the power card");
            return;
        }

        Coord new_coord;
        std::cout << "Enter the coordinates of the card:";
        std::cin >> new_coord.first >> new_coord.second;

        if (std::find(validCards.begin(), validCards.end(), new_coord) == validCards.end()) {
            Logger::log(Level::WARNING, "No card at there coordinates");
        }
        auto top_card = board.getTopCard(new_coord);
        CombatCard& card = top_card->get();
        CombatCardType currentType = card.getType();

        CombatCardType newType = currentType;
        switch (currentType) {
        case CombatCardType::ONE:
            newType = CombatCardType::TWO;
            break;
        case CombatCardType::TWO:
            newType = CombatCardType::THREE;
            break;
        case CombatCardType::THREE:
            newType = CombatCardType::FOUR;
            break;
        default:
            Logger::log(Level::WARNING, "Card cannot be incremented beyond FOUR");
            return;
        }
        CombatCard new_card(newType, card.getColor());
        board.popTopCardAt(new_coord);
        board.appendMove(new_coord, std::move(new_card));

    }
    std::vector<Coord> Support::CoordCardType(Board& board, const Player& player) const {
        std::vector<Coord> coordCards;

        for (const auto& [coord, stack] : board) {
            if (stack.empty()) continue;
            const CombatCard& card = stack.back();

            if (!card.isIllusion() &&
                card.getColor() == player.getColor() &&
                (card.getType() == CombatCardType::ONE ||
                    card.getType() == CombatCardType::TWO ||
                    card.getType() == CombatCardType::THREE)) {
                coordCards.emplace_back(coord);
            }
        }

        return coordCards;
    }

    ////------------------------------------------ Earthquake -------------------------------------------
    Earthquake::Earthquake() {
        m_ability = PowerCardType::Earthquake;
    }

    void Earthquake::apply(Board& board, Player& player) {
        std::vector<Coord> coords_to_remove;
        for (const auto& [coord, stack] : board) {
            const auto& top_card = board.getTopCard(coord);
                const CombatCard& card = top_card->get();
                if (card.getType() == CombatCardType::ONE) {
                  
                    coords_to_remove.emplace_back(coord);
            }
        }

        for (const auto& coord : coords_to_remove) {
            board.popTopCardAt(coord);
        }

        if (coords_to_remove.empty()) {
            Logger::log(Level::WARNING, "No visible cards with number 1");
        }
        else {
            Logger::log(Level::INFO, "Earthquake power card removed all the visible '1' cards");
        }
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
        std::vector<Coord>illusionCoord = getIllusionCoords(board);
        if (!illusionCoord.empty()&& player.hasCards()) {

            std::cout << "Illusion coordinates: " << std::endl;

            Coord coord;
            std::cin >> coord.first >> coord.second;

            if (std::find(illusionCoord.begin(), illusionCoord.end(), coord) == illusionCoord.end()) {
                Logger::log(Level::WARNING, "No illusion at these coordinates");
            }

            std::cout << "Choose a card to cover the illusion" << std::endl;
            char card_type;
            std::cin >> card_type;
            auto card = player.getCard(charToCombatCard(card_type));

            board.appendMove(coord, std::move(card));
        }
        Logger::log(Level::INFO, "Rock power card was used");
    }

    std::vector<Coord>Rock::getIllusionCoords(const Board& board) const {
        std::vector<Coord>illusionCoords;
        for (const auto& [coord, stack] : board) {
            if (!stack.empty() && stack.back().isIllusion()) {
                Logger::log(Level::INFO, "Illusion card at ({}, {})", coord.first, coord.second);
                illusionCoords.emplace_back(coord);
            }
            else {
                Logger::log(Level::WARNING, "No illusions on the board");
            }
        }
        return illusionCoords;
    }

}
