#include "PowerCards.h"
using namespace logger;

namespace base {

    ////------------------------------------------ ControllerExplosion -------------------------------------------
    ControllerExplosion::ControllerExplosion() {
        m_ability = PowerCardType::ControllerExplosion;
    }

    void ControllerExplosion::apply(Board& board, Player& player) { /// nu stiu cum functioneaza explozia daca poti sa o faci tu :( 
        ExplosionService explosionService(board, player, player);

        if (!explosionService.checkAvailability()) {
            Logger::log(Level::WARNING, "No valid spaces for explosion");
            return;
        }

        explosionService.setting();

        auto effectCoords = explosionService.getEffectCoords();

        /* if (!effectCoords.empty()) {
             explosionService.renderExplosion();
             explosionService.apply();

             Logger::log(Level::INFO, "Controller Explosion power card was used succesfully");
         }
         else {
             Logger::log(Level::WARNING, "No valid explosion effects to apply");
         }*/
    }

    ////------------------------------------------ Destruction -------------------------------------------
    Destruction::Destruction() {
        m_ability = PowerCardType::Destruction;
    }

    bool Destruction::canUseAbility(const Board& board, const Player& player) const {
        if (!board.hasLastPlayedCard()) {
            return false;
        }
        if (board.getLastPlayerColor() == player.getColor()) {
            return false;
        }
        return true;
    }

    std::optional<Coord> Destruction::getTargetPosition(const Board& board) const {
        if (!board.hasLastPlayedCard()) {
            return std::nullopt;
        }
        return board.getLastCardPosition();
    }

    std::string Destruction::getErrorMessage(const Board& board, const Player& player) const {
        if (!board.hasLastPlayedCard()) {
            return "No last played card to destroy";
        }
        if (board.getLastPlayerColor() == player.getColor()) {
            return "Last card was played by you, not your opponent";
        }
        return "";
    }

    void Destruction::apply(Board& board, Player& player) {
        if (!canUseAbility(board, player)) {
            Logger::log(Level::WARNING, getErrorMessage(board, player));
            return;
        }

        Coord last_position = board.getLastCardPosition();
        board.removeTopCardAt(last_position);

        Logger::log(Level::INFO, "Destruction power destroyed the last played card at ({}, {})",
            last_position.first, last_position.second);
    }

    ////------------------------------------------ Flame -------------------------------------------
    Flame::Flame() {                               ///////////////// asta e cu iluzie, nu merge
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

            Logger::log(Level::INFO, "It's your turn, place a card");
            Coord new_coord;
            std::cin >> new_coord.first >> new_coord.second;
            char card_type;
            std::cin >> card_type;


            auto selected_card = player.getCard(charToCombatCard(card_type));

            if (board.isValidPlaceCard(new_coord, selected_card)) {
                board.appendMove(new_coord, std::move(selected_card));
                Logger::log(Level::INFO, "Flame power was used. Card placed at ({}, {})",
                    new_coord.first, new_coord.second);
                break;
            }
            else {
                Logger::log(Level::WARNING, "Invalid move!");
            }
        }
    }

    ////------------------------------------------ Fire -------------------------------------------



    Fire::Fire() {
        m_ability = PowerCardType::Fire;
    }

    std::vector<std::pair<Coord, CombatCardType>> Fire::getVisibleCards(const Board& board) const {
        return std::ranges::views::filter(board, [](const auto& pair) {
            const auto& stack = pair.second;
            return !stack.empty() && !stack.back().isIllusion();
            }) | std::ranges::views::transform([](const auto& pair) {
                return std::pair{ pair.first, pair.second.back().getType() };
                }) | std::ranges::to<std::vector>();
    }

    bool Fire::canUseAbility(const Board& board) const {
        auto visible_cards = getVisibleCards(board);
        if (visible_cards.empty()) {
            return false;
        }

        auto valid_types = getValidChoices(board);
        return !valid_types.empty();
    }

    std::vector<CombatCardType> Fire::getValidChoices(const Board& board) const {
        auto visible_cards = getVisibleCards(board);
        std::vector<CombatCardType> result;

        for (auto type : std::array{ CombatCardType::ONE, CombatCardType::TWO,
                                    CombatCardType::THREE, CombatCardType::FOUR }) {
            auto count = std::ranges::count_if(visible_cards, [type](const auto& pair) {
                return pair.second == type;
                });

            if (count >= 2) {
                result.push_back(type);
            }
        }

        return result;
    }

    bool Fire::isValidChoice(CombatCardType chosen_type,
        const std::vector<CombatCardType>& valid_choices) const {
        return std::ranges::contains(valid_choices, chosen_type);
    }

    std::string Fire::getErrorMessage(const Board& board) const {
        auto visible_cards = getVisibleCards(board);
        if (visible_cards.empty()) {
            return "No visible cards on the board";
        }

        auto valid_types = getValidChoices(board);
        if (valid_types.empty()) {
            return "No card type appears at least twice on the board";
        }

        return "";
    }

    void Fire::applyEffect(Board& board, CombatCardType chosen_type) {
        auto visible_cards = getVisibleCards(board);

        for (const auto& [coord, type] : visible_cards) {
            if (type == chosen_type) {
                board.removeTopCardAt(coord);
                board.returnUsedCardToHand(type);
            }
        }

        Logger::log(Level::INFO, "Fire power returned all visible {} cards to their owners",
            combatCardToChar(chosen_type));
    }

    void Fire::setChosenCard(CombatCardType card_type) {
        m_chosen_card = card_type;
        m_has_choice = true;
    }

    void Fire::apply(Board& board, Player& player) {
        if (!canUseAbility(board)) {
            Logger::log(Level::WARNING, getErrorMessage(board));
            return;
        }

        auto valid_choices = getValidChoices(board);

        if (!m_has_choice) {
            Logger::log(Level::WARNING, "No card type was chosen");
            return;
        }

        if (!isValidChoice(m_chosen_card, valid_choices)) {
            Logger::log(Level::WARNING, "Invalid choice");
            return;
        }

        applyEffect(board, m_chosen_card);
        m_has_choice = false;
    }
    ////------------------------------------------ Ash -------------------------------------------
    Ash::Ash() {
        m_ability = PowerCardType::Ash;
    }

    bool Ash::canUseAbility(const Player& player) const {
        return player.hasUsedCards();
    }

    std::vector<Ash::UsedCardInfo> Ash::getUsedCardsInfo(const Player& player) const {
        if (!canUseAbility(player)) {
            return {};
        }

        std::vector<UsedCardInfo> used_cards;
        const auto& cards = player.getUsedCards();

        for (const auto& card : cards) {
            used_cards.push_back({
                card.second.getType(),
                card.second.getColor()
                });
        }

        return used_cards;
    }


    std::string Ash::getErrorMessage(const Player& player) const {
        if (!player.hasUsedCards()) {
            return "You don't have any removed card";
        }
        return "";
    }

    void Ash::setSelection(const Coord& coordinates, CombatCardType card_type) {
        m_selected_coord = coordinates;
        m_selected_card = card_type;
        m_has_selection = true;
    }

    void Ash::apply(Board& board, Player& player) {
        if (!canUseAbility(player)) {
            Logger::log(Level::WARNING, getErrorMessage(player));
            return;
        }

        if (!m_has_selection) {
            Logger::log(Level::WARNING, "No selection was made");
            return;
        }

        auto card = player.getUsedCard(m_selected_card);
        board.appendMove(m_selected_coord, std::move(card));

        Logger::log(Level::INFO, "Ash power card was used");

        m_has_selection = false;
    }



    ////------------------------------------------ Spark -------------------------------------------
    Spark::Spark() {
        m_ability = PowerCardType::Spark;
    }

    void Spark::setAvailableChoices(const std::vector<std::pair<Coord, CombatCardType>>& choices) {
        availableChoices = choices;
    }

    std::optional<Coord> Spark::getSelectedFromCoord() const {
        return selectedFromCoord;
    }

    void Spark::setSelectedFromCoord(Coord coord) {
        selectedFromCoord = coord;
    }

    void Spark::setSelectedCardType(CombatCardType type) {
        selectedCardType = type;
    }

    std::optional<CombatCardType> Spark::getSelectedCardType() const {
        return selectedCardType;
    }

    void Spark::setMoveDestination(Coord coord) {
        moveDestination = coord;
    }

    std::optional<Coord> Spark::getMoveDestination() const {
        return moveDestination;
    }

    void Spark::apply(Board& board, Player& player) {
        auto choices = coverCards(board, player);
        setAvailableChoices(choices);

        if (choices.empty()) {
            Logger::log(Level::WARNING, "You have no covered cards on the board!");
            return;
        }

        if (!getSelectedFromCoord()) {
            Logger::log(Level::WARNING, "No card selected!");
            return;
        }

        auto it = std::find_if(choices.begin(), choices.end(),
            [this](const auto& pair) { return pair.first == *getSelectedFromCoord(); });

        if (it == choices.end()) {
            Logger::log(Level::WARNING, "You don't have any covered card at these coordinates!");
            return;
        }

        if (!getSelectedCardType()) {
            Logger::log(Level::WARNING, "No card type selected!");
            return;
        }

        if (it->second != *getSelectedCardType()) {
            Logger::log(Level::WARNING, "This card doesn't exist at the specified position!");
            return;
        }

        if (!getMoveDestination()) {
            Logger::log(Level::WARNING, "No move destination selected!");
            return;
        }

        CombatCard card(*getSelectedCardType(), player.getColor());

        if (!board.isValidPlaceCard(*getMoveDestination(), card)) {
            Logger::log(Level::WARNING, "You can't move the card to this position!");
            return;
        }

        board.removeCardFromStackAt(*getSelectedFromCoord(), card);
        board.appendMove(*getMoveDestination(), std::move(card));

        Logger::log(Level::INFO, "Card successfully moved!");
    }

    std::vector<std::pair<Coord, CombatCardType>> Spark::coverCards(const Board& board, const Player& player) {
        std::vector<std::pair<Coord, CombatCardType>> choices;
        for (const auto& [coord, stack] : board) {
            if (stack.size() < 2) continue;
            for (size_t i = 0; i < stack.size() - 1; ++i) {
                const CombatCard& card = stack[i];
                if (card.getColor() == player.getColor()) {
                    choices.emplace_back(coord, card.getType());
                }
            }
        }
        return choices;
    }

    ////------------------------------------------ Squall -------------------------------------------
    Squall::Squall() {
        m_ability = PowerCardType::Squall;
    }
    void Squall::setVisibleCards(const std::vector<std::pair<Coord, CombatCardType>>& cards) {
        visibleCards = cards;
    }

    std::optional<Coord> Squall::getSelectedCardCoord() const {
        return selectedCardCoord;
    }

    void Squall::setSelectedCardCoord(Coord coord) {
        selectedCardCoord = coord;
    }

    std::vector<std::pair<Coord, CombatCardType>> Squall::getVisibleCards(const Board& board, const Player& player) {
        std::vector<std::pair<Coord, CombatCardType>> visible_cards;
        for (const auto& [coord, stack] : board) {
            if (!stack.empty()) {
                const CombatCard& top_card = stack.back();
                if (!top_card.isIllusion() && top_card.getColor() != player.getColor()) {
                    visible_cards.emplace_back(coord, top_card.getType());
                }
            }
        }
        return visible_cards;
    }

    void Squall::apply(Board& board, Player& player) {
        auto visible_cards = getVisibleCards(board, player);
        setVisibleCards(visible_cards);

        if (visible_cards.empty()) {
            Logger::log(Level::WARNING, "No visible opponent cards on the board!");
            return;
        }

        if (!getSelectedCardCoord()) {
            Logger::log(Level::WARNING, "No card selected!");
            return;
        }

        auto it = std::find_if(visible_cards.begin(), visible_cards.end(),
            [this](const auto& pair) { return pair.first == *getSelectedCardCoord(); });

        if (it == visible_cards.end()) {
            Logger::log(Level::WARNING, "Invalid coordinates! No opponent card there.");
            return;
        }

        player.addCard(board.popTopCardAt(*getSelectedCardCoord()));
        Logger::log(Level::INFO, "Card successfully returned to opponent's hand!");
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
            {selectedCard.first - 2, selectedCard.second},
            {selectedCard.first + 2, selectedCard.second},
            {selectedCard.first, selectedCard.second - 1},
            {selectedCard.first, selectedCard.second + 1}
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
            Coord neighbor1{ coord.first + 2, coord.second };
            Coord neighbor2{ coord.first + (2*2), coord.second };

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
            {{2, 0}, Orientation::Row},
            {{-2, 0}, Orientation::Row},
            {{0, 1}, Orientation::Column},
            {{0, -1}, Orientation::Column}
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
            {Orientation::Row,    {{{2, 0}, MoveDirection::Right}, {{-2, 0}, MoveDirection::Left}}},
            {Orientation::Column, {{{0, 1}, MoveDirection::Up},    {{0, -1}, MoveDirection::Down}}}
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
