#include "PowerCards.h"

using namespace logger;

namespace base {

    ////------------------------------------------ ControllerExplosion -------------------------------------------
    ControllerExplosion::ControllerExplosion() {
        m_ability = PowerCardType::ControllerExplosion;
    }

    void ControllerExplosion::apply(Board& board, Player& player) {
        ExplosionService explosionService(board, player, player);

        if (!explosionService.checkAvailability()) {
            Logger::log(Level::WARNING, "No valid spaces for explosion");
            return;
        }

        explosionService.setting();

        auto effectCoords = explosionService.getEffectCoords();

        if (!effectCoords.empty()) {
            explosionService.renderExplosion();
            explosionService.apply();

            Logger::log(Level::INFO, "Controller Explosion power card was used succesfully");
        }
        else {
            Logger::log(Level::WARNING, "No valid explosion effects to apply");
        }
    }

    ////------------------------------------------ Destruction -------------------------------------------
    Destruction::Destruction() {
        m_ability = PowerCardType::Destruction;
    }

    void Destruction::apply(Board& board, Player& player) {
        for (const auto& [coord, stack] : board) {
            auto top_card = board.getTopCard(coord);
            const CombatCard& card = top_card->get();
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
        m_hasUserSelected = false;
    }

    void Flame::setSelectedMove(const Coord& coord, CombatCardType cardType) {
        m_selectedCoord = coord;
        m_selectedCardType = cardType;
        m_hasUserSelected = true;
    }

    const std::vector<std::pair<Coord, CombatCardType>>&Flame::getAvailableChoices() const {
        return m_availableChoices;
    }

    void Flame::apply(Board & board, Player & player) {
        for (const auto& [coord, stack] : board) {
            auto top_card = board.getTopCard(coord);
            CombatCard& card = top_card->get();
            if (card.getColor() != player.getColor() && card.isIllusion()) {
                card.flip();
                Logger::log(Level::INFO, "The opponent's Illusion has been revealed");
            }

            if (!m_hasUserSelected) {
                Logger::log(Level::WARNING, "No selection made yet");
                return;
            }

            auto selected_card = player.getCard(m_selectedCardType);

            if (board.isValidPlaceCard(m_selectedCoord, selected_card)) {
                board.appendMove(m_selectedCoord, std::move(selected_card));
                Logger::log(Level::INFO, "Flame power was used. Card placed at ({}, {})",
                    m_selectedCoord.first, m_selectedCoord.second);
                m_hasUserSelected = false;
                break;
            }
            else {
                Logger::log(Level::WARNING, "Invalid move!");
            }
        }
    }

    ////------------------------------------------ Fire -------------------------------------------




    Fire::Fire(Player& red_player, Player& blue_player)
        : m_red_player{ red_player },
        m_blue_player{ blue_player } {

        m_ability = PowerCardType::Fire;
    }

    void Fire::apply(Board& board, Player& player) {
        std::vector<Coord>duplicateCoord = getDuplicateCards(board, player);
        if (duplicateCoord.size() > 1) {
            for (const auto& coord : duplicateCoord) {
                auto card = board.popTopCardAt(coord);
                color::ColorType cardColor = card.getColor();

                if (cardColor == m_red_player.getColor()) {
                    m_red_player.addCard(std::move(card));
                }
                else {
                    m_blue_player.addCard(std::move(card));
                }
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
            if (stack.empty()) {
                continue;
            }
            if (!stack.back().isIllusion()) {
                choices.emplace_back(x, y);
            }
        }
        return choices;
    }

    ////------------------------------------------ Ash -------------------------------------------
    Ash::Ash() {
        m_ability = PowerCardType::Ash;
        m_hasUserSelected =false;
    }

    void Ash::setSelectedMove(const Coord& coord, CombatCardType cardType) {
        m_selectedCoord = coord;
        m_selectedCardType = cardType;
        m_hasUserSelected = true;
    }

    void Ash::apply(Board& board, Player& player) {
        if (!player.hasUsedCards()) {
            Logger::log(Level::WARNING, "You don't have any removed card");
            return;
        }

        if (!m_hasUserSelected) {
            Logger::log(Level::WARNING, "No selection made yet");
            return;
        }

        auto card = player.getUsedCard(m_selectedCardType);
        if (board.isValidPlaceCard(m_selectedCoord, card)) {
            board.appendMove(m_selectedCoord, std::move(card));
            Logger::log(Level::INFO, "Ash power card was used");
            m_hasUserSelected = false;
        }
    }


    ////------------------------------------------ Spark -------------------------------------------
    Spark::Spark() {
        m_ability = PowerCardType::Spark;
        bool m_hasUserSelected = false;
    }

    void Spark::setSelectedMove(const Coord& from, const Coord& to, CombatCardType cardType) {
        m_coordFrom = from;
        m_coordTo = to;
        m_selectedCardType = cardType;
        m_hasUserSelected = true;
    }

    const std::vector<std::pair<Coord, CombatCardType>>& Spark::getAvailableChoices() const {
        return m_availableChoices;
    }

    std::vector<std::pair<Coord, CombatCardType>> Spark::coverCards(const Board& board, const Player& player) {
        m_availableChoices.clear();
        for (const auto& [coord, stack] : board) {
            if (stack.empty()) {
                continue;
            }
            for (size_t i = 0; i < stack.size() - 1; ++i) {
                const CombatCard& card = stack[i];
                bool player_card = card.getColor() == player.getColor();

                if (player_card) {
                    CombatCardType card_type = card.getType();
                    m_availableChoices.emplace_back(coord, card_type);
                }
            }
        }
        return m_availableChoices;
    }

    void Spark::apply(Board& board, Player& player) {
        auto choices = coverCards(board, player);

        if (choices.empty()) {
            Logger::log(Level::WARNING, "No covered cards");
            return;
        }

        if (!m_hasUserSelected) {
            Logger::log(Level::WARNING, "No selection made yet");
            return;
        }

        bool valid_card = false;
        bool valid_coord = false;
        for (const auto& choice : choices) {
            if (choice.first == m_coordFrom) {
                valid_coord = true;
            }
            if (choice.second == m_selectedCardType) {
                valid_card = true;
            }
        }

        if (valid_card && valid_coord) {
            CombatCard card(m_selectedCardType, player.getColor());
            board.removeCardFromStackAt(m_coordFrom, card);
            board.appendMove(m_coordTo, std::move(card));
            m_hasUserSelected = false;
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

    ////------------------------------------------ Squall -------------------------------------------
    Squall::Squall(Player& red_player, Player& blue_player)
        :m_red_player{ red_player }, m_blue_player{blue_player} {
        m_ability = PowerCardType::Squall;
        m_hasUserSelected = false;
    }

    void Squall::setSelectedMove(const Coord& coord) {
        m_selectedCoord = coord;
        m_hasUserSelected = true;
    }



    void Squall::apply(Board& board, Player& player) {
        for (const auto& [coord, stack] : board) {
            auto visible_cards = board.popTopCardAt(coord);

            if (!m_hasUserSelected) {
                Logger::log(Level::WARNING, "No selection made yet");
                return;
            }

            bool valid_move = false;
            if (coord == m_selectedCoord) {
                board.removeTopCardAt(m_selectedCoord);

                color::ColorType cardColor = visible_cards.getColor();

                if (cardColor == m_red_player.getColor()) {
                    m_red_player.addCard(std::move(visible_cards));
                }
                else {
                    m_blue_player.addCard(std::move(visible_cards));

                    Logger::log(Level::INFO, "Squall ability: Returned a visible card to the opponent's hand.");
                    valid_move = true;
                    m_hasUserSelected = false;
                    break;
                }
            }

            if (!valid_move) {
                Logger::log(Level::WARNING, "No visible card at these coordinates");
            }
        }
    }

    ////------------------------------------------ Gale -------------------------------------------
    Gale::Gale(Player& red_player, Player& blue_player)
        :m_red_player{ red_player }, m_blue_player{ blue_player } {
        m_ability = PowerCardType::Gale;
    }

    void Gale::apply(Board& board, Player& player) {

        for (const auto& [coord, stack] : board) {
            while (stack.size() > 1) {
                for (size_t i = 0; i < stack.size() - 1; ++i) {
                    const CombatCard& card = stack[i];
                    CombatCard popCard = board.popCardFromStackAt(coord, card);
                    color::ColorType cardColor = popCard.getColor();

                    if (cardColor == m_red_player.getColor()) {
                        m_red_player.addCard(std::move(popCard));
                    }
                    else {
                        m_blue_player.addCard(std::move(popCard));
                    }
                }
                Logger::log(Level::INFO, "Moved cards from board to player hand.");
            }
        }
    }



    ////------------------------------------------ Hurricane -------------------------------------------
    Hurricane::Hurricane() {
        m_ability = PowerCardType::Hurricane;
        bool m_hasUserSelected = false;
    }

    void Hurricane::setSelectedMove(Orientation orientation, uint16_t index, MoveDirection direction) {
        m_selectedOrientation = orientation;
        m_selectedIndex = index;
        m_selectedDirection = direction;
        m_hasUserSelected = true;
    }

    const std::unordered_map<Orientation, std::vector<uint16_t>>& Hurricane::getOptions(Board& board) {
        m_options.clear();

        if (board.fixedWidth()) {
            auto rows = board.getFixedRows();
            for (const auto row : rows) {
                m_options[Orientation::Row].push_back(row);
            }
        }
        if (board.fixedHeight()) {
            auto columns = board.getFixedColumns();
            for (const auto column : columns) {
                m_options[Orientation::Column].push_back(column);
            }
        }
        return m_options;
    }

    void Hurricane::apply(Board& board, Player& player) {
        getOptions(board);

        if (!m_hasUserSelected) {
            Logger::log(Level::WARNING, "No selection made yet");
            return;
        }

        auto it = m_options.find(m_selectedOrientation);
        if (it == m_options.end() ||
            std::find(it->second.begin(), it->second.end(), m_selectedIndex) == it->second.end()) {
            Logger::log(Level::WARNING, "Wrong choice");
            return;
        }

        auto dirIt = validDirections.find(m_selectedOrientation);
        if (std::find(dirIt->second.begin(), dirIt->second.end(), m_selectedDirection) == dirIt->second.end()) {
            Logger::log(Level::WARNING, "Wrong choice");
            return;
        }

        if (m_selectedOrientation == Orientation::Row) {
            if (m_selectedDirection == MoveDirection::Right) {
                Coord coord = board.getRightMostOnRow(m_selectedIndex);
                board.removeStack(coord);
                auto vect_coords = board.getCoordsOnRow(m_selectedIndex);
                board.shiftRowToRight(vect_coords);
            }
            else {
                Coord coord = board.getLeftMostOnRow(m_selectedIndex);
                board.removeStack(coord);
                auto vect_coords = board.getCoordsOnRow(m_selectedIndex);
                board.shiftRowToLeft(vect_coords);
            }
        }
        else {
            if (m_selectedDirection == MoveDirection::Up) {
                Coord coord = board.getTopMostOnColumn(m_selectedIndex);
                board.removeStack(coord);
                auto vect_coord = board.getCoordsOnColumn(m_selectedIndex);
                board.shiftColumnUp(vect_coord);
            }
            else {
                Coord coord = board.getBottomMostOnColumn(m_selectedIndex);
                board.removeStack(coord);
                auto vect_coord = board.getCoordsOnColumn(m_selectedIndex);
                board.shiftColumnDown(vect_coord);
            }
        }
        m_hasUserSelected = false;
    }



    ////------------------------------------------ Gust -------------------------------------------
    Gust::Gust() {
        m_ability = PowerCardType::Gust;
        m_hasUserSelected = false;
    }

       void Gust::setSelectedMove(const Coord& fromCoord, const Coord& toCoord) {
        m_selectedFromCoord = fromCoord;
        m_selectedToCoord = toCoord;
        m_hasUserSelected = true;
    }

    const std::vector<Coord>& Gust::getAvailableCards() const {
        return m_availableCards;
    }

    const std::vector<Coord>& Gust::getValidMoves(const Coord& selectedCard, Board& board) {
        m_validMoves.clear();
        
        auto optCard = board.getTopCard(selectedCard);
        if (!optCard.has_value()) {
            return m_validMoves;
        }

        const CombatCard& card = optCard->get();
        std::vector<Coord> neighbors{
            {selectedCard.first - 4, selectedCard.second},
            {selectedCard.first + 4, selectedCard.second},
            {selectedCard.first, selectedCard.second - 3},
            {selectedCard.first, selectedCard.second + 3}
        };

        for (const Coord& neighbor : neighbors) {
            auto optNeighborCard = board.getTopCard(neighbor);
            if (optNeighborCard.has_value()) {
                const CombatCard& neighborCard = optNeighborCard->get();
                if (!neighborCard.isIllusion() && 
                    (neighborCard.getType() != CombatCardType::ETER) &&
                    neighborCard.getType() < card.getType() &&
                    (neighborCard.getType() != CombatCardType::HOLE)) {
                    m_validMoves.push_back(neighbor);
                }
            }
        }
        return m_validMoves;
    }

    std::vector<Coord> Gust::getCardsCoord(const Board& board) {
        m_availableCards.clear();
        for (const auto& [coord, stack] : board) {
            if (!stack.empty() && !stack.back().isIllusion()) {
                m_availableCards.emplace_back(coord);
            }
        }
        return m_availableCards;
    }

    void Gust::apply(Board& board, Player& player) {
        auto cardsCoord = getCardsCoord(board);
        
        if (!m_hasUserSelected) {
            Logger::log(Level::WARNING, "No selection made yet");
            return;
        }

        if (std::find(cardsCoord.begin(), cardsCoord.end(), m_selectedFromCoord) == cardsCoord.end()) {
            Logger::log(Level::WARNING, "Invalid card coordinates");
            return;
        }

        auto optCard = board.getTopCard(m_selectedFromCoord);
        if (!optCard.has_value()) {
            Logger::log(Level::WARNING, "No card found at the specified coordinates");
            return;
        }

        auto validMoves = getValidMoves(m_selectedFromCoord, board);
        if (validMoves.empty()) {
            Logger::log(Level::WARNING, "No valid coordinates to move the card");
            return;
        }

        if (std::find(validMoves.begin(), validMoves.end(), m_selectedToCoord) == validMoves.end()) {
            Logger::log(Level::WARNING, "Invalid coordinates to move the card");
            return;
        }

        CombatCard& card = optCard->get();
        if (board.isValidRemoveStack(m_selectedFromCoord) && 
            board.isValidPlaceCard(m_selectedToCoord, card)) {
            CombatCard movedCard = std::move(card);
            board.removeTopCardAt(m_selectedFromCoord);
            board.appendMove(m_selectedToCoord, std::move(movedCard));
            Logger::log(Level::INFO, "Gust power card was used");
            m_hasUserSelected = false;
        }
    }


    ////------------------------------------------ Mirrage -------------------------------------------
    Mirrage::Mirrage() {
        m_ability = PowerCardType::Mirrage;
        m_hasUserSelected = false;
    }

    void Mirrage::setSelectedMove(const Coord& coord, CombatCardType cardType) {
        m_selectedCoord = coord;
        m_selectedCardType = cardType;
        m_hasUserSelected = true;
    }

    bool Mirrage::getIllusion(Board& board, Player& player) {
        bool foundIllusion = false;
        for (const auto& [coord, stack] : board) {
            auto top_card = board.getTopCard(coord);
            CombatCard& card = top_card->get();
            if (card.isIllusion() && card.getColor() == player.getColor()) {
                card.flip();
                player.addCard(std::move(card));
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

    void Mirrage::apply(Board& board, Player& player) {
        if (!getIllusion(board, player)) {
            return;
        }

        if (!m_hasUserSelected) {
            Logger::log(Level::WARNING, "No selection made yet");
            return;
        }

        auto card = player.getCard(m_selectedCardType);
        card.flip();
        board.appendMove(m_selectedCoord, std::move(card));
        Logger::log(Level::INFO, "Mirrage power card was used");
        m_hasUserSelected = false;
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

        for (const auto& coord : stacksToRemove) {
            board.removeStack(coord);
            Logger::log(Level::INFO, "A stack with 2 or more cards has been removed from the board");
            break;
        }
    }

    ////------------------------------------------ Tide -------------------------------------------

    Tide::Tide() : m_hasUserSelected(false) {
        m_ability = PowerCardType::Tide;
    }

    const std::vector<Coord>& Tide::getAvailableChoices() const {
        return m_availableStacks;
    }

    bool Tide::hasUserSelected() const {
        return m_hasUserSelected;
    }

    void Tide::setSelectedCoords(const Coord& from, const Coord& to) {
        m_selectedFrom = from;
        m_selectedTo = to;
        m_hasUserSelected = true;
    }

    void Tide::apply(Board& board, Player& player) {
        getStacks(board);

        if (m_availableStacks.size() <= 1) {
            Logger::log(Level::WARNING, "Not enough stacks available for swapping");
            return;
        }

        if (!m_hasUserSelected) {
            Logger::log(Level::WARNING, "No selection made yet");
            return;
        }

        if (std::find(m_availableStacks.begin(), m_availableStacks.end(), m_selectedFrom)
            == m_availableStacks.end()) {
            Logger::log(Level::WARNING, "No stack at first selected coordinates");
            return;
        }

        if (std::find(m_availableStacks.begin(), m_availableStacks.end(), m_selectedTo)
            == m_availableStacks.end()) {
            Logger::log(Level::WARNING, "No stack at second selected coordinates");
            return;
        }

        board.swapStacks(m_selectedFrom, m_selectedTo);
        Logger::log(Level::INFO, "Tide power card was used");
        m_hasUserSelected = false;
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
    Mist::Mist() { //joaca inca o iluzie
        m_ability = PowerCardType::Mist;

    }

    void Mist::apply(Board& board, Player& player) {

    }
    bool Mist::hasIllusion(Board& board, IllusionService& illusionService, Player& player) {
        auto playerColor = player.getColor();
        // if (illusionService.hasPlayerIllusion(playerColor)) {
        Logger::log(Level::WARNING, "Player already has an illusion");
        //      return false;
        //  }
       //   else {
        Logger::log(Level::INFO, "You can play your illusion");

        Coord coord;
        std::cin >> coord.first >> coord.second;

        char card_type;
        std::cin >> card_type;
        auto card = player.getCard(charToCombatCard(card_type));

        illusionService.placeIllusion(coord, std::move(card));
        //  }
        return true;
    }


    ////------------------------------------------ Wave -------------------------------------------
    Wave::Wave() :
        m_hasUserSelected(false),
        m_hasSelectedCard(false),
        m_selectedCardType('\0') {
        m_ability = PowerCardType::Wave;
    }

    const std::vector<Coord>& Wave::getAvailableStacks() const {
        return m_availableStacks;
    }

    const std::vector<Coord>& Wave::getAvailableSpaces() const {
        return m_availableSpaces;
    }

    bool Wave::hasUserSelected() const {
        return m_hasUserSelected;
    }

    bool Wave::hasSelectedCard() const {
        return m_hasSelectedCard;
    }

    void Wave::setSelectedMove(const Coord& from, const Coord& to) {
        m_selectedFrom = from;
        m_selectedTo = to;
        m_hasUserSelected = true;
    }

    void Wave::setSelectedCard(char cardType) {
        m_selectedCardType = cardType;
        m_hasSelectedCard = true;
    }

    void Wave::clearSelections() {
        m_hasUserSelected = false;
        m_hasSelectedCard = false;
        m_selectedCardType = '\0';
    }

    void Wave::apply(Board& board, Player& player) {
        m_availableSpaces = std::vector<Coord>( 
            board.availableSpaces().begin(), 
            board.availableSpaces().end() 
        ); 
        if (m_availableSpaces.empty()) {
            Logger::log(Level::WARNING, "No empty spaces to move the stack");
            return;
        }

        std::vector<Coord> coordStack = validStacks(board);
        if (coordStack.empty()) {
            Logger::log(Level::WARNING, "No stacks on the board");
            return;
        }

        for (const auto& coord : coordStack) {
            if (!m_hasUserSelected) {
                Logger::log(Level::WARNING, "No move selection made");
                return;
            }

            if (board.isValidMoveStack(coord, m_selectedTo)) {
                board.moveStack(coord, m_selectedTo);
            }

            if (!m_hasSelectedCard) {
                Logger::log(Level::WARNING, "No card selection made");
                return;
            }

            auto card = player.getCard(charToCombatCard(m_selectedCardType));
            if (board.isValidPlaceCard(coord, card)) {
                board.appendMove(coord, std::move(card));
            }

            clearSelections();
        }

        Logger::log(Level::INFO, "Wave power card was used");
    }

    std::vector<Coord> Wave::validStacks(const Board& board) const {
       m_availableStacks.clear();
        for (const auto& [coord, stack] : board) {
            if (stack.size() > 1) {
                m_availableStacks.emplace_back(coord);
            }
        }
        return m_availableStacks;
    }



    ////------------------------------------------ Whirlpool -------------------------------------------
    Whirlpool::Whirlpool() {
        m_ability = PowerCardType::Whirlpool;
    }

    void Whirlpool::setSelectedPair(const Coord& first, const Coord& second) {
        m_selectedPair = { first, second };
        m_hasUserSelectedPair = true;
    }

    void Whirlpool::setTopCardPosition(const Coord& coord) {
        m_topCardPosition = coord;
        m_hasUserSelectedTopCard = true;
    }

    const std::vector<std::pair<Coord, Coord>>& Whirlpool::getAvailablePairs() const {
        return m_availablePairs;
    }

    std::vector<std::pair<Coord, Coord>> Whirlpool::getPairs(Board& board) {
        m_availablePairs.clear();
        for (const auto& [coord, stack] : board) {
            Coord neighbor1{ coord.first + 4, coord.second };
            Coord neighbor2{ coord.first + 8, coord.second };

            if (!board.hasStack(neighbor1) && board.hasStack(neighbor2)) {
                m_availablePairs.emplace_back(std::make_pair(coord, neighbor2));
            }
        }
        return m_availablePairs;
    }

    void Whirlpool::apply(Board& board, Player& player) {
        getPairs(board);

        if (m_availablePairs.empty()) {
            Logger::log(Level::WARNING, "No valid row to use power card");
            return;
        }
      
        if (!m_hasUserSelectedPair) {
            Logger::log(Level::WARNING, "No pair selection made yet");
            return;
        }

        if (std::find(m_availablePairs.begin(), m_availablePairs.end(), m_selectedPair)
            == m_availablePairs.end()) {
            Logger::log(Level::WARNING, "Invalid coordinate pairs");
            return;
        }

        auto& [first_coord, second_coord] = m_selectedPair;
        auto opt_first_card = board.getTopCard(first_coord);
        auto opt_second_card = board.getTopCard(second_coord);

        if (!opt_first_card || !opt_second_card) {
            Logger::log(Level::WARNING, "No card found at one or both coordinates");
            return;
        }

        CombatCard& first_card = opt_first_card->get();
        CombatCard& second_card = opt_second_card->get();

        if (first_card.getType() < second_card.getType()) {
            board.appendMove({ first_coord.first + 4, first_coord.second }, std::move(first_card));
            board.appendMove({ second_coord.first - 4, second_coord.second }, std::move(second_card));
        }
        else if (first_card.getType() == second_card.getType()) {
            if (!m_hasUserSelectedTopCard) {
                Logger::log(Level::WARNING, "No top card position selected for equal cards");
                return;
            }

            if (m_topCardPosition != first_coord && m_topCardPosition != second_coord) {
                Logger::log(Level::WARNING, "Invalid top card position");
                return;
            }

            if (m_topCardPosition == first_coord) {
                board.appendMove({ second_coord.first - 4, second_coord.second }, std::move(second_card));
                board.appendMove({ first_coord.first + 4, first_coord.second }, std::move(first_card));
            }
            else {
                board.appendMove({ first_coord.first + 4, first_coord.second }, std::move(first_card));
                board.appendMove({ second_coord.first - 4, second_coord.second }, std::move(second_card));
            }
        }
        else {
            board.appendMove({ second_coord.first - 4, second_coord.second }, std::move(second_card));
            board.appendMove({ first_coord.first + 4, first_coord.second }, std::move(first_card));
        }

        board.popTopCardAt(first_coord);
        board.popTopCardAt(second_coord);
        Logger::log(Level::INFO, "Whirlpool power card was played");

        m_hasUserSelectedPair = false;
        m_hasUserSelectedTopCard = false;
    }
    ////------------------------------------------ Blizzard -------------------------------------------
    Blizzard::Blizzard() :
        m_selectedBlockType(BlockType::NONE),
        m_selectedValue(0),
        m_hasUserSelected(false) {
        m_ability = PowerCardType::Blizzard;
    }

    const std::unordered_set<uint16_t>& Blizzard::getAvailableRows() const {
      
        return m_availableRows;
    }

    const std::unordered_set<uint16_t>& Blizzard::getAvailableColumns() const {
       
        return m_availableColumns;
    }

    bool Blizzard::hasUserSelected() const {
       
        return m_hasUserSelected;
    }

    bool Blizzard::hasValidSelection() const {
       
        if (!m_hasUserSelected) return false;

        if (m_selectedBlockType == BlockType::ROW) {
            return m_availableRows.find(m_selectedValue) != m_availableRows.end();
        }
        else if (m_selectedBlockType == BlockType::COLUMN) {
            return m_availableColumns.find(m_selectedValue) != m_availableColumns.end();
        }
        return false;
    }

    void Blizzard::setBlockType(BlockType type) {
        m_selectedBlockType = type;
        m_hasUserSelected = true;
    }

    void Blizzard::setSelectedValue(uint16_t value) {
        m_selectedValue = value;
    }

    void Blizzard::clearSelection() {
        m_hasUserSelected = false;
        m_selectedBlockType = BlockType::NONE;
        m_selectedValue = 0;
    }

    void Blizzard::updateAvailableChoices(const Board& board) {
        m_availableRows.clear();
        m_availableColumns.clear();

        for (const auto& [coord, stack] : board.getCombatCards()) {
            m_availableRows.insert(coord.second);
            m_availableColumns.insert(coord.first);
        }

        for (const auto& coord : board.availableSpaces()) {
            m_availableRows.insert(coord.second);
            m_availableColumns.insert(coord.first);
        }
    }

    void Blizzard::apply(Board& board, Player& player) {
        if (board.availableSpaces().empty()) {
            Logger::log(Level::WARNING, "No available spaces for opponent's next turn");
            return;
        }
       
        updateAvailableChoices(board);
     
        if (!m_hasUserSelected)  {
            Logger::log(Level::WARNING, "No selection made");
            return;
        }
        if (!hasValidSelection()) {
            Logger::log(Level::WARNING, "Invalid selection");
            return;
        }

        if (m_selectedBlockType == BlockType::ROW) {
            board.blockRow(m_selectedValue, player.getColor());
            Logger::log(Level::INFO, "Row {} has been blocked for the opponent's next turn", m_selectedValue);
        }
        else if (m_selectedBlockType == BlockType::COLUMN) {
            board.blockColumn(m_selectedValue, player.getColor());
            Logger::log(Level::INFO, "Column {} has been blocked for the opponent's next turn", m_selectedValue);
        }

        clearSelection();
    }

    ////------------------------------------------ Waterfall -------------------------------------------
   
        Waterfall::Waterfall() :
            m_selectedOrientation(Orientation::Unknown),
            m_selectedIndex(0),
            m_selectedDirection(MoveDirection::Unknown),
            m_hasUserSelected(false) {
            m_ability = PowerCardType::Waterfall;
        }

        const std::unordered_map<Orientation, std::vector<int16_t>>& Waterfall::getAvailableOptions() const {
            return m_availableOptions;
        }

        bool Waterfall::hasUserSelected() const {
            return m_hasUserSelected;
        }

        void Waterfall::setSelection(Orientation orientation, int16_t index, MoveDirection direction) {
            m_selectedOrientation = orientation;
            m_selectedIndex = index;
            m_selectedDirection = direction;
            m_hasUserSelected = true;
        }

        void Waterfall::clearSelection() {
            m_hasUserSelected = false;
            m_selectedOrientation = Orientation::Unknown;
            m_selectedIndex = 0;
            m_selectedDirection = MoveDirection::Unknown;
        }

        std::unordered_map<Orientation, std::vector<int16_t>> Waterfall::getOptions(Board& board) {
            int16_t size = static_cast<int16_t>(board.getSize() - 1);
            std::unordered_map<Orientation, std::vector<int16_t>> choices;
            std::unordered_set<int16_t> processedRows;
            std::unordered_set<int16_t> processedCols;

            for (const auto& [coord, _] : board) {
                if (processedRows.find(coord.second) == processedRows.end()) {
                    auto countRowElements = board.getCoordsOnRow(coord.second);
                    if (countRowElements.size() >= size) {
                        choices[Orientation::Row].emplace_back(coord.second);
                        processedRows.insert(coord.second);
                    }
                }

                if (processedCols.find(coord.first) == processedCols.end()) {
                    auto countColElements = board.getCoordsOnColumn(coord.first);
                    if (countColElements.size() >= size) {
                        choices[Orientation::Column].emplace_back(coord.first);
                        processedCols.insert(coord.first);
                    }
                }
            }

            m_availableOptions = choices;
            return choices;
        }

        bool Waterfall::validateSelection() const {
            if (!m_hasUserSelected) return false;

            auto it = m_availableOptions.find(m_selectedOrientation);
            if (it == m_availableOptions.end()) return false;

            if (std::find(it->second.begin(), it->second.end(), m_selectedIndex) == it->second.end()) {
                return false;
            }

            auto dirIt = validDirections.find(m_selectedOrientation);
            return std::find(dirIt->second.begin(), dirIt->second.end(), m_selectedDirection) != dirIt->second.end();
        }

        void Waterfall::apply(Board& board, Player& player) {
            getOptions(board);

            if (m_availableOptions.empty()) {
                Logger::log(Level::WARNING, "You can't use this power right now");
                return;
            }

            if (!validateSelection()) {
                Logger::log(Level::WARNING, "Invalid selection");
                return;
            }

            if (m_selectedOrientation == Orientation::Row) {
                auto vect = board.getCoordsOnRow(m_selectedIndex);
                if (m_selectedDirection == MoveDirection::Right) {
                    auto destination = board.getRightMostOnRow(m_selectedIndex);
                    board.mergeStacks(vect, destination);
                }
                else {
                    auto destination = board.getLeftMostOnRow(m_selectedIndex);
                    board.mergeStacks(vect, destination);
                }
            }
            else {
                auto vect = board.getCoordsOnColumn(m_selectedIndex);
                if (m_selectedDirection == MoveDirection::Up) {
                    auto destination = board.getTopMostOnColumn(m_selectedIndex);
                    board.mergeStacks(vect, destination);
                }
                else {
                    auto destination = board.getBottomMostOnColumn(m_selectedIndex);
                    board.mergeStacks(vect, destination);
                }
            }

            clearSelection();
        }
    


    ////------------------------------------------ Support -------------------------------------------
        Support::Support() : m_hasUserSelected(false) {
            m_ability = PowerCardType::Support;
        }

        const std::vector<Coord>& Support::getAvailableCards() const {
            return m_availableCards;
        }

        bool Support::hasUserSelected() const {
            return m_hasUserSelected;
        }

        void Support::setSelectedCoord(const Coord& coord) {
            m_selectedCoord = coord;
            m_hasUserSelected = true;
        }

        void Support::clearSelection() {
            m_hasUserSelected = false;
        }

        CombatCardType Support::getNextCardType(CombatCardType currentType) const {
            switch (currentType) {
            case CombatCardType::ONE:
                return CombatCardType::TWO;
            case CombatCardType::TWO:
                return CombatCardType::THREE;
            case CombatCardType::THREE:
                return CombatCardType::FOUR;
            default:
                return currentType;
            }
        }

        std::vector<Coord> Support::CoordCardType(Board& board, const Player& player) const {
            std::vector<Coord> coordCards;
            for (const auto& [coord, stack] : board) {
                if (stack.empty()) continue;
                const CombatCard& card = stack.back();
                if (!card.isIllusion() &&
                    card.getColor() == player.getColor() &&
                    (card.getType() == CombatCardType::TWO ||
                        card.getType() == CombatCardType::THREE ||
                        card.getType() == CombatCardType::FOUR)) {
                    coordCards.emplace_back(coord);
                }
            }
            return coordCards;
        }

        void Support::apply(Board& board, Player& player) {
            m_availableCards = CoordCardType(board, player);

            if (m_availableCards.empty()) {
                Logger::log(Level::WARNING, "No valid cards to apply the power card");
                return;
            }

            if (!m_hasUserSelected) {
                Logger::log(Level::WARNING, "No selection made");
                return;
            }

            if (std::find(m_availableCards.begin(), m_availableCards.end(), m_selectedCoord)
                == m_availableCards.end()) {
                Logger::log(Level::WARNING, "No card at those coordinates");
                return;
            }

            auto top_card = board.getTopCard(m_selectedCoord);
            CombatCard& card = top_card->get();
            CombatCardType currentType = card.getType();
            CombatCardType newType = getNextCardType(currentType);

            if (newType == currentType) {
                Logger::log(Level::WARNING, "Card cannot be incremented beyond FOUR");
                return;
            }

            CombatCard new_card(newType, card.getColor());
            board.popTopCardAt(m_selectedCoord);
            board.appendMove(m_selectedCoord, std::move(new_card));

            clearSelection();
        }

    ////------------------------------------------ Earthquake -------------------------------------------
        Earthquake::Earthquake() {
            m_ability = PowerCardType::Earthquake;
        }

        const std::vector<Coord>& Earthquake::getAvailableTargets() const {
            return m_availableTargets;
        }

        bool Earthquake::hasValidTargets() const {
            return !m_availableTargets.empty();
        }
        void Earthquake::findValidTargets(const Board& board) {
            m_availableTargets.clear();

            for (const auto& [coord, stack] : board) {
                if (!stack.empty()) {
                    const CombatCard& card = stack.back();
                    if (card.getType() == CombatCardType::ONE) {
                        m_availableTargets.push_back(coord);
                    }
                }
            }
        }

        void Earthquake::apply(Board& board, Player& player) {
            findValidTargets(board);

            if (!hasValidTargets()) {
                Logger::log(Level::WARNING, "No visible cards with number 1");
                return;
            }

            const Coord& targetCoord = m_availableTargets[0];
            board.removeTopCardAt(targetCoord);
            board.availableSpaces();

            Logger::log(Level::INFO, "Earthquake power removed a visible card with number 1.");
        }


    ////------------------------------------------ Crumble -------------------------------------------
        Crumble::Crumble() : m_hasUserSelected(false) {
            m_ability = PowerCardType::Crumble;
        }

        const std::vector<Coord>& Crumble::getAvailableCards() const {
            return m_availableCards;
        }

        bool Crumble::hasUserSelected() const {
            return m_hasUserSelected;
        }

        void Crumble::setSelectedCoord(const Coord& coord) {
            m_selectedCoord = coord;
            m_hasUserSelected = true;
        }

        void Crumble::clearSelection() {
            m_hasUserSelected = false;
        }

        CombatCardType Crumble::getPreviousCardType(CombatCardType currentType) const {
            switch (currentType) {
            case CombatCardType::TWO:
                return CombatCardType::ONE;
            case CombatCardType::THREE:
                return CombatCardType::TWO;
            case CombatCardType::FOUR:
                return CombatCardType::THREE;
            default:
                return currentType;
            }
        }

        std::vector<Coord> Crumble::CoordCardType(Board& board, const Player& player) const {
            std::vector<Coord> coordCards;
            for (const auto& [coord, stack] : board) {
                if (stack.empty()) continue;
                const CombatCard& card = stack.back();
                if (!card.isIllusion() &&
                    card.getColor() == player.getColor() &&
                    (card.getType() == CombatCardType::TWO ||
                        card.getType() == CombatCardType::THREE ||
                        card.getType() == CombatCardType::FOUR)) {
                    coordCards.emplace_back(coord);
                }
            }
            return coordCards;
        }

        void Crumble::apply(Board& board, Player& player) {
            m_availableCards = CoordCardType(board, player);

            if (m_availableCards.empty()) {
                Logger::log(Level::WARNING, "No valid cards to apply the power card");
                return;
            }

            if (!m_hasUserSelected) {
                Logger::log(Level::WARNING, "No selection made");
                return;
            }

            if (std::find(m_availableCards.begin(), m_availableCards.end(), m_selectedCoord)
                == m_availableCards.end()) {
                Logger::log(Level::WARNING, "No card at those coordinates");
                return;
            }

            auto top_card = board.getTopCard(m_selectedCoord);
            CombatCard& card = top_card->get();
            CombatCardType currentType = card.getType();
            CombatCardType newType = getPreviousCardType(currentType);

            if (newType == currentType) {
                Logger::log(Level::WARNING, "Card cannot be decremented further");
                return;
            }

            CombatCard new_card(newType, card.getColor());
            board.popTopCardAt(m_selectedCoord);
            board.appendMove(m_selectedCoord, std::move(new_card));

            clearSelection();
        }


    ////------------------------------------------ Border -------------------------------------------
    Border::Border() {
        m_ability = PowerCardType::Border;
    }

    void Border::apply(Board& board, Player& player) {
    }


    ////------------------------------------------ Avalanche -------------------------------------------
    Avalanche::Avalanche() :
        m_selectedDirection(MoveDirection::Unknown),
        m_hasUserSelected(false) {
        m_ability = PowerCardType::Avalanche;
    }

    const std::vector<std::pair<MoveDirection, std::pair<Coord, Coord>>>&
        Avalanche::getAvailableChoices() const {
        return m_availableChoices;
    }

    bool Avalanche::hasUserSelected() const {
        return m_hasUserSelected;
    }

    void Avalanche::setSelectedMove(MoveDirection direction,
        const Coord& firstCoord, const Coord& secondCoord) {
        m_selectedDirection = direction;
        m_selectedPair = std::make_pair(firstCoord, secondCoord);
        m_hasUserSelected = true;
    }

    void Avalanche::clearSelection() {
        m_hasUserSelected = false;
        m_selectedDirection = MoveDirection::Unknown;
    }

    std::vector<std::pair<Orientation, std::pair<Coord, Coord>>>
        Avalanche::getPairs(Board& board) {
  
        std::vector<std::pair<Orientation, std::pair<Coord, Coord>>> pairs;
        const std::vector<std::pair<Coord, Orientation>> directions = {
            {{4, 0}, Orientation::Row},
            {{-4, 0}, Orientation::Row},
            {{0, 3}, Orientation::Column},
            {{0, -3}, Orientation::Column}
        };

        for (const auto& [coord, stack] : board) {
            for (const auto& [delta, orientation] : directions) {
                Coord neighbor = { coord.first + delta.first, coord.second + delta.second };
                if (board.hasStack(neighbor)) {
                    pairs.emplace_back(orientation, std::make_pair(coord, neighbor));
                }
            }
        }
        return pairs;
    }

    std::vector<std::pair<MoveDirection, std::pair<Coord, Coord>>>Avalanche::checkShifting(
        const std::vector<std::pair<Orientation, std::pair<Coord, Coord>>>& pack, Board& board) {

        std::vector<std::pair<MoveDirection, std::pair<Coord, Coord>>> choices;

        const std::unordered_map<Orientation, std::vector<std::pair<Coord, MoveDirection>>> directions = {
            {Orientation::Row,    {{{4, 0}, MoveDirection::Right}, {{-4, 0}, MoveDirection::Left}}},
            {Orientation::Column, {{{0, 3}, MoveDirection::Up},    {{0, -3}, MoveDirection::Down}}}
        };

        for (const auto& [type, pair] : pack) {
            for (const auto& [delta, shift] : directions.at(type)) {
                Coord newCoord = { pair.first.first + delta.first, pair.first.second + delta.second };
                if (!board.hasStack(newCoord)) {
                    choices.emplace_back(shift, std::make_pair(pair.first, pair.second));
                }
            }
        }

        return choices;
    }
    bool Avalanche::validateAndApplyMove(Board& board,
        const MoveDirection& type, const std::pair<Coord, Coord>& selectedPair) {
        const auto& delta = directionDeltas.at(type);
        std::vector<std::pair<Coord, Coord>> stacks;
        Coord newCoord1, newCoord2;

        if (type == MoveDirection::Down || type == MoveDirection::Up) {
            newCoord1 = { selectedPair.second.first, selectedPair.second.second + delta.second };
            newCoord2 = { selectedPair.first.first, selectedPair.first.second + delta.second };
            stacks.push_back({ selectedPair.second, newCoord1 });
            stacks.push_back({ selectedPair.first, newCoord2 });
        }
        else {
            newCoord1 = { selectedPair.first.first + delta.first, selectedPair.first.second };
            newCoord2 = { selectedPair.second.first + delta.first, selectedPair.second.second };
            stacks.push_back({ selectedPair.first, newCoord1 });
            stacks.push_back({ selectedPair.second, newCoord2 });
        }

        if (board.isValidMoveStacks(stacks)) {
            board.moveStacks(stacks);
            Logger::log(Level::INFO, "Move applied");
            return true;
        }
        Logger::log(Level::WARNING, "Invalid choice!");
        return false;
    }

    void Avalanche::apply(Board& board, Player& player) {
        auto pairs = getPairs(board);
        if (pairs.empty()) {
            Logger::log(Level::WARNING, "You can't use this card rn");
            return;
        }

        m_availableChoices = checkShifting(pairs, board);

        if (!m_hasUserSelected) {
            Logger::log(Level::WARNING, "No selection made");
            return;
        }

        auto choice = std::pair{ m_selectedDirection, m_selectedPair };
        auto it = std::find(m_availableChoices.begin(), m_availableChoices.end(), choice);

        if (it != m_availableChoices.end()) {
            const auto& [type, selectedPair] = *it;
            validateAndApplyMove(board, type, selectedPair);
        }
        else {
            Logger::log(Level::WARNING, "Invalid choice!");
        }

        Logger::log(Level::INFO, "Avalanche card used");
        clearSelection();
    }



    ////------------------------------------------ Rock -------------------------------------------
    Rock::Rock() :
        m_hasUserSelected(false),
        m_hasCardSelected(false),
        m_selectedCardType('\0') {
        m_ability = PowerCardType::Rock;
    }

    const std::vector<Coord>& Rock::getAvailableIllusions() const {
        return m_availableIllusions;
    }

    bool Rock::hasUserSelected() const {
        return m_hasUserSelected;
    }

    bool Rock::hasCardSelected() const {
        return m_hasCardSelected;
    }

    void Rock::setSelectedCoord(const Coord& coord) {
        m_selectedCoord = coord;
        m_hasUserSelected = true;
    }

    void Rock::setSelectedCard(char cardType) {
        m_selectedCardType = cardType;
        m_hasCardSelected = true;
    }

    void Rock::clearSelection() {
        m_hasUserSelected = false;
        m_hasCardSelected = false;
        m_selectedCardType = '\0';
    }

    std::vector<Coord> Rock::getIllusionCoords(const Board& board) const {
        std::vector<Coord> illusionCoords;
        for (const auto& [coord, stack] : board) {
            if (!stack.empty() && stack.back().isIllusion()) {
                Logger::log(Level::INFO, "Illusion card at ({}, {})", coord.first, coord.second);
                illusionCoords.emplace_back(coord);
            }
        }
        if (illusionCoords.empty()) {
            Logger::log(Level::WARNING, "No illusions on the board");
        }
        return illusionCoords;
    }

    void Rock::apply(Board& board, Player& player) {
        m_availableIllusions = getIllusionCoords(board);

        if (m_availableIllusions.empty() || !player.hasCards()) {
            return;
        }

        if (!m_hasUserSelected || !m_hasCardSelected) {
            Logger::log(Level::WARNING, "No selection made");
            return;
        }

        if (std::find(m_availableIllusions.begin(), m_availableIllusions.end(), m_selectedCoord)
            == m_availableIllusions.end()) {
            Logger::log(Level::WARNING, "No illusion at these coordinates");
            return;
        }

        auto card = player.getCard(charToCombatCard(m_selectedCardType));
        board.appendMove(m_selectedCoord, std::move(card));

        Logger::log(Level::INFO, "Rock power card was used");

        clearSelection();
    }
}
