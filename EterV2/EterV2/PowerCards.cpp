#include "PowerCards.h"
#include<set>
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
    Gale::Gale() {
        m_ability = PowerCardType::Gale;
    }

    void Gale::apply(Board& board, Player& player) {
        std::vector<std::pair<Coord, CombatCardType>> cards_to_remove;


        for (const auto& [coord, stack] : board) {
            if (stack.size() <= 1) {
                continue;
            }

            for (size_t i = 0; i < stack.size() - 1; i++) {
                cards_to_remove.emplace_back(coord, stack[i].getType());
            }
        }

        for (const auto& [coord, card_type] : cards_to_remove) {
            board.removeCardFromStackAt(coord, CombatCard(card_type, player.getColor()));
            board.returnUsedCardToHand(card_type);
        }

        Logger::log(Level::INFO, "Gale power card effect completed");
    }

    ////------------------------------------------ Hurricane -------------------------------------------
    Hurricane::Hurricane() {
        m_ability = PowerCardType::Hurricane;
    }

    void Hurricane::setOptions(const std::unordered_map<Orientation, std::vector<uint16_t>>& opts) {
        options = opts;
    }

    std::unordered_map<Orientation, std::vector<uint16_t>> Hurricane::getOptions() const {
        return options;
    }

    void Hurricane::setUserSelection(Orientation orient, uint16_t index) {
        userSelection = std::make_tuple(orient, index);
    }

    std::optional<std::tuple<Orientation, uint16_t>> Hurricane::getUserSelection() const {
        return userSelection;
    }

    void Hurricane::setUserDirection(MoveDirection dir) {
        userDirection = dir;
    }

    std::optional<MoveDirection> Hurricane::getUserDirection() const {
        return userDirection;
    }

    void Hurricane::apply(Board& board, Player& player) {
        auto options = getOptions();
        if (options.empty()) {
            Logger::log(Level::WARNING, "You can't use this mage right now");
            return;
        }

        if (!getUserSelection() || !getUserDirection()) {
            Logger::log(Level::WARNING, "No valid selection or direction set!");
            return;
        }

        auto [orient, index] = *getUserSelection();
        auto shift_dir = *getUserDirection();

        std::vector<std::pair<Coord, Coord>> to_shift;

        if (orient == Orientation::Row) {
            if (shift_dir == MoveDirection::Right) {
                Coord coord = board.getRightMostOnRow(index);
                auto stack_to_return = board.popStack(coord);
                for (auto& card : stack_to_return) {
                    board.returnCardToHand(std::move(card), card.getColor());
                }
                auto vect_coords = board.getCoordsOnRow(index);
                board.shiftRowToRight(vect_coords);
            }
            else {
                Coord coord = board.getLeftMostOnRow(index);
                auto stack_to_return = board.popStack(coord);
                for (auto& card : stack_to_return) {
                    board.returnCardToHand(std::move(card), card.getColor());
                }
                auto vect_coords = board.getCoordsOnRow(index);
                board.shiftRowToLeft(vect_coords);
            }
        }
        else {
            if (shift_dir == MoveDirection::Up) {
                Coord coord = board.getTopMostOnColumn(index);
                auto stack_to_return = board.popStack(coord);
                for (auto& card : stack_to_return) {
                    board.returnCardToHand(std::move(card), card.getColor());
                }
                auto vect_coord = board.getCoordsOnColumn(index);
                board.shiftColumnUp(vect_coord);
            }
            else {
                Coord coord = board.getBottomMostOnColumn(index);
                auto stack_to_return = board.popStack(coord);
                for (auto& card : stack_to_return) {
                    board.returnCardToHand(std::move(card), card.getColor());
                }
                auto vect_coord = board.getCoordsOnColumn(index);
                board.shiftColumnDown(vect_coord);
            }
        }
    }


    ////------------------------------------------ Gust -------------------------------------------
    Gust::Gust() {
        m_ability = PowerCardType::Gust;
    }
    void Gust::setValidSourceCards(const std::vector<Coord>& sources) {
        validSourceCards = sources;
    }

    std::vector<Coord> Gust::getValidSourceCards() const {
        return validSourceCards;
    }

    void Gust::setSelectedSource(Coord coord) {
        selectedSource = coord;
    }

    std::optional<Coord> Gust::getSelectedSource() const {
        return selectedSource;
    }

    void Gust::setValidDestinations(const std::vector<Coord>& destinations) {
        validDestinations = destinations;
    }

    std::vector<Coord> Gust::getValidDestinations() const {
        return validDestinations;
    }

    void Gust::setSelectedDestination(Coord coord) {
        selectedDestination = coord;
    }

    std::optional<Coord> Gust::getSelectedDestination() const {
        return selectedDestination;
    }

    void Gust::apply(Board& board, Player& player) {
        std::vector<Coord> sources;
        for (const auto& [coord, stack] : board) {
            if (!stack.empty() && !stack.back().isIllusion()) {
                sources.push_back(coord);
            }
        }
        setValidSourceCards(sources);

        if (sources.empty()) {
            Logger::log(Level::WARNING, "No valid cards to move");
            return;
        }

        if (!getSelectedSource()) {
            Logger::log(Level::WARNING, "No source card selected!");
            return;
        }

        Coord sourceCoord = *getSelectedSource();
        if (std::find(sources.begin(), sources.end(), sourceCoord) == sources.end()) {
            Logger::log(Level::WARNING, "Invalid source coordinates");
            return;
        }

        std::vector<Coord> destinations;
        const std::vector<std::pair<int16_t, int16_t>> adjacentOffsets = {
            {2, 0}, {-2, 0}, {0, 1}, {0, -1}
        };

        CombatCardType sourceValue = board[sourceCoord].back().getType();
        for (const auto& [dx, dy] : adjacentOffsets) {
            Coord adjCoord = { sourceCoord.first + dx, sourceCoord.second + dy };
            if (board.hasStack(adjCoord)) {
                auto& adjStack = board[adjCoord];
                if (!adjStack.empty() && !adjStack.back().isIllusion()) {
                    if (adjStack.back().getType() < sourceValue) {
                        destinations.push_back(adjCoord);
                    }
                }
            }
        }
        setValidDestinations(destinations);

        if (destinations.empty()) {
            Logger::log(Level::WARNING, "No valid destinations available");
            return;
        }

        if (!getSelectedDestination()) {
            Logger::log(Level::WARNING, "No destination selected!");
            return;
        }

        Coord destCoord = *getSelectedDestination();
        if (std::find(destinations.begin(), destinations.end(), destCoord) == destinations.end()) {
            Logger::log(Level::WARNING, "Invalid destination coordinates");
            return;
        }

        CombatCard card = std::move(board.popTopCardAt(sourceCoord));
        board.appendMove(destCoord, std::move(card));

        Logger::log(Level::INFO, "Gust power: Successfully moved card from ({}, {}) to ({}, {})",
            sourceCoord.first, sourceCoord.second, destCoord.first, destCoord.second);
    }
    ////------------------------------------------ Mirrage -------------------------------------------
    Mirrage::Mirrage() {                                 /////////////// iluzie, nu merge
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
            //  if (board.isValidMove(coord, card)) {
            card.flip();
            board.appendMove(coord, std::move(card));
            Logger::log(Level::INFO, "Mirrage power card was used");
            // }
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

    void Storm::setAvailableStacks(const std::vector<Coord>& stacks) {
        availableStacks = stacks;
    }

    std::vector<Coord> Storm::getAvailableStacks() const {
        return availableStacks;
    }

    void Storm::setSelectedStack(size_t index) {
        selectedStack = index;
    }

    std::optional<size_t> Storm::getSelectedStack() const {
        return selectedStack;
    }

    void Storm::apply(Board& board, Player& player) {
        std::vector<Coord> stacksToRemove;

        for (const auto& [coord, stack] : board) {
            if (stack.size() > 1) {
                stacksToRemove.push_back(coord);
            }
        }
        setAvailableStacks(stacksToRemove);

        if (stacksToRemove.empty()) {
            Logger::log(Level::WARNING, "No stacks with 2 or more cards found on the board");
            return;
        }

        if (!getSelectedStack() || *getSelectedStack() == 0 || *getSelectedStack() > stacksToRemove.size()) {
            Logger::log(Level::WARNING, "Invalid stack selection");
            return;
        }

        const Coord& selectedCoord = stacksToRemove[*getSelectedStack() - 1];

        try {
            if (board.isValidRemoveStack(selectedCoord)) {
                board.removeStack(selectedCoord);
            }
        }
        catch (const std::runtime_error& e) {
            Logger::log(Level::ERROR, "Failed to remove stack: {}", e.what());
        }
    }

    ////------------------------------------------ Tide -------------------------------------------
    Tide::Tide() {
        m_ability = PowerCardType::Tide;
    }

    void Tide::setAvailableStacks(const std::vector<Coord>& stacks) {
        availableStacks = stacks;
    }

    std::vector<Coord> Tide::getAvailableStacks() const {
        return availableStacks;
    }

    void Tide::setSelectedStacks(const std::pair<Coord, Coord>& stacks) {
        selectedStacks = stacks;
    }

    std::optional<std::pair<Coord, Coord>> Tide::getSelectedStacks() const {
        return selectedStacks;
    }

    std::vector<Coord> Tide::getStacks(const Board& board) {
        std::vector<Coord> stacks_coord;
        for (const auto& [coord, stack] : board) {
            if (!stack.empty()) {
                stacks_coord.emplace_back(coord);
            }
        }
        return stacks_coord;
    }

    void Tide::apply(Board& board, Player& player) {
        std::vector<Coord> stacks = getStacks(board);
        setAvailableStacks(stacks);

        if (stacks.size() < 2) {
            Logger::log(Level::WARNING, "There are not enough stacks to swap.");
            return;
        }

        if (!getSelectedStacks()) {
            Logger::log(Level::WARNING, "No stacks selected for swapping.");
            return;
        }

        auto [coord_from, coord_to] = *getSelectedStacks();

        if (std::find(stacks.begin(), stacks.end(), coord_from) == stacks.end() ||
            std::find(stacks.begin(), stacks.end(), coord_to) == stacks.end()) {
            Logger::log(Level::WARNING, "Invalid stack selection.");
            return;
        }

        board.swapStacks(coord_from, coord_to);
        Logger::log(Level::INFO, "Tide power card was used to swap stacks at ({}, {}) and ({}, {}).",
            coord_from.first, coord_from.second, coord_to.first, coord_to.second);
    }


    ////------------------------------------------ Mist -------------------------------------------
    Mist::Mist() { /////////////////////////////////////////////////////////joaca inca o iluzie, nu merge
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
    Wave::Wave() {
        m_ability = PowerCardType::Wave;
    }

    void Wave::setAvailableStacks(const std::vector<Coord>& stacks) {
        availableStacks = stacks;
    }

    std::vector<Coord> Wave::getAvailableStacks() const {
        return availableStacks;
    }

    void Wave::setSelectedStack(const Coord& stack) {
        selectedStack = stack;
    }

    std::optional<Coord> Wave::getSelectedStack() const {
        return selectedStack;
    }

    void Wave::setDestination(const Coord& destination) {
        this->destination = destination;
    }

    std::optional<Coord> Wave::getDestination() const {
        return destination;
    }

    void Wave::setSelectedCard(char card) {
        selectedCard = card;
    }

    std::optional<char> Wave::getSelectedCard() const {
        return selectedCard;
    }

    std::vector<Coord> Wave::validStacks(const Board& board) const {
        std::vector<Coord> coordStack;
        for (const auto& [coord, stack] : board) {
            if (stack.size() > 1) {
                coordStack.emplace_back(coord);
            }
        }
        return coordStack;
    }

    void Wave::apply(Board& board, Player& player) {
        if (board.availableSpaces().empty()) {
            Logger::log(Level::WARNING, "No empty spaces to move the stack.");
            return;
        }

        std::vector<Coord> stacks = validStacks(board);
        setAvailableStacks(stacks);

        if (stacks.empty()) {
            Logger::log(Level::WARNING, "No stacks on the board.");
            return;
        }

        if (!getSelectedStack()) {
            Logger::log(Level::WARNING, "No stack selected.");
            return;
        }

        Coord selected = *getSelectedStack();

        if (std::find(stacks.begin(), stacks.end(), selected) == stacks.end()) {
            Logger::log(Level::WARNING, "Invalid stack selection.");
            return;
        }

        if (!getDestination()) {
            Logger::log(Level::WARNING, "No destination selected.");
            return;
        }

        Coord dest = *getDestination();

        if (!board.availableSpaces().contains(dest)) {
            Logger::log(Level::WARNING, "Invalid destination coordinates.");
            return;
        }

        if (board.isValidMoveStack(selected, dest)) {
            board.moveStack(selected, dest);

            if (!getSelectedCard()) {
                Logger::log(Level::WARNING, "No card selected for placement.");
                return;
            }

            try {
                auto card = player.getCard(charToCombatCard(*getSelectedCard()));
                board.appendMove(selected, std::move(card));
                Logger::log(Level::INFO, "Wave power card was used successfully.");
            }
            catch (const std::runtime_error& e) {
                Logger::log(Level::ERROR, "Error placing card: {}", e.what());
            }
        }
        else {
            Logger::log(Level::WARNING, "Invalid stack movement.");
        }
    }


    ////------------------------------------------ Whirlpool -------------------------------------------
    Whirlpool::Whirlpool() {
        m_ability = PowerCardType::Whirlpool;
    }

    void Whirlpool::apply(Board& board, Player& player) {
        validPairsOnRow = getPairsOnRow(board);
        validPairsOnColumn = getPairsOnColumn(board);

        if (validPairsOnRow.empty() && validPairsOnColumn.empty()) {
            Logger::log(Level::WARNING, "No valid row or column to use power card");
            return;
        }

        setValidPairsOnRow(validPairsOnRow);
        setValidPairsOnColumn(validPairsOnColumn);

        char choice = getUserChoice();
        if (choice == 'r') {

            Coord firstCoord = getFirstCoord();
            Coord secondCoord = getSecondCoord();


            std::pair<Coord, Coord> inputCoords{ firstCoord, secondCoord };
            if (std::find(validPairsOnRow.begin(), validPairsOnRow.end(), inputCoords) == validPairsOnRow.end()) {
                Logger::log(Level::WARNING, "Invalid coordinate pairs");
                return;
            }

            auto optFirstCard = board.getTopCard(firstCoord);
            auto optSecondCard = board.getTopCard(secondCoord);

            if (!optFirstCard.has_value() || !optSecondCard.has_value()) {
                Logger::log(Level::WARNING, "No card found at one or both coordinates");
                return;
            }

            CombatCard& firstCard = optFirstCard->get();
            CombatCard& secondCard = optSecondCard->get();


            if (firstCard.getType() < secondCard.getType()) {
                board.appendMove({ firstCoord.first + 2, firstCoord.second }, std::move(firstCard));
                board.appendMove({ secondCoord.first - 2, secondCoord.second }, std::move(secondCard));
            }
            else if (firstCard.getType() == secondCard.getType()) {

                Coord equalCardCoord = getEqualCardCoord();
                if (equalCardCoord != firstCoord && equalCardCoord != secondCoord) {
                    Logger::log(Level::WARNING, "Invalid coordinates for placing card");
                    return;
                }

                if (equalCardCoord == firstCoord) {
                    board.appendSpecialCard({ secondCoord.first - 2, secondCoord.second }, std::move(secondCard));
                    board.appendSpecialCard({ firstCoord.first + 2, firstCoord.second }, std::move(firstCard));
                }
                else {
                    board.appendSpecialCard({ firstCoord.first + 2, firstCoord.second }, std::move(firstCard));
                    board.appendSpecialCard({ secondCoord.first - 2, secondCoord.second }, std::move(secondCard));
                }
            }
            else {
                board.appendMove({ secondCoord.first - 2, secondCoord.second }, std::move(secondCard));
                board.appendMove({ firstCoord.first + 2, firstCoord.second }, std::move(firstCard));
            }

            board.popTopCardAt(firstCoord);
            board.popTopCardAt(secondCoord);
            Logger::log(Level::INFO, "Whirlpool power card was played");
        }

        if (choice == 'c') {
            Coord firstCoord = getFirstCoord();
            Coord secondCoord = getSecondCoord();


            std::pair<Coord, Coord> inputCoords{ firstCoord, secondCoord };
            if (std::find(validPairsOnRow.begin(), validPairsOnRow.end(), inputCoords) == validPairsOnRow.end()) {
                Logger::log(Level::WARNING, "Invalid coordinate pairs");
                return;
            }

            auto optFirstCard = board.getTopCard(firstCoord);
            auto optSecondCard = board.getTopCard(secondCoord);

            if (!optFirstCard.has_value() || !optSecondCard.has_value()) {
                Logger::log(Level::WARNING, "No card found at one or both coordinates");
                return;
            }

            CombatCard& firstCard = optFirstCard->get();
            CombatCard& secondCard = optSecondCard->get();

            if (firstCard.getType() < secondCard.getType()) {
                board.appendMove({ firstCoord.first, firstCoord.second + 1 }, std::move(firstCard));
                board.appendMove({ secondCoord.first, secondCoord.second - 1 }, std::move(secondCard));

            }
            else if (firstCard.getType() == secondCard.getType()) {

                Coord equalCardCoord = getEqualCardCoord();
                if (equalCardCoord != firstCoord && equalCardCoord != secondCoord) {
                    Logger::log(Level::WARNING, "Invalid coordinates for placing card");
                    return;
                }

                if (equalCardCoord == firstCoord) {
                    board.appendSpecialCard({ secondCoord.first , secondCoord.second - 1 }, std::move(secondCard));
                    board.appendSpecialCard({ firstCoord.first , firstCoord.second + 1 }, std::move(firstCard));

                }
                else {
                    board.appendSpecialCard({ firstCoord.first , firstCoord.second + 1 }, std::move(firstCard));
                    board.appendSpecialCard({ secondCoord.first , secondCoord.second - 1 }, std::move(secondCard));
                }
            }
            else {
                board.appendMove({ secondCoord.first, secondCoord.second - 1 }, std::move(secondCard));
                board.appendMove({ firstCoord.first, firstCoord.second + 1 }, std::move(firstCard));
            }

            board.popTopCardAt(firstCoord);
            board.popTopCardAt(secondCoord);
            Logger::log(Level::INFO, "Whirlpool power card was played");
        }


    }

    void Whirlpool::setUserChoice(char choice) {
        userChoice = choice;
    }

    char Whirlpool::getUserChoice() const {
        return userChoice;
    }

    void Whirlpool::setFirstCoord(const Coord& coord) {
        firstCoord = coord;
    }

    Coord Whirlpool::getFirstCoord() const {
        return firstCoord;
    }

    void Whirlpool::setSecondCoord(const Coord& coord) {
        secondCoord = coord;
    }

    Coord Whirlpool::getSecondCoord() const {
        return secondCoord;
    }

    void Whirlpool::setEqualCardCoord(const Coord& coord) {
        equalCardCoord = coord;
    }

    Coord Whirlpool::getEqualCardCoord() const {
        return equalCardCoord;
    }

    void Whirlpool::setValidPairsOnRow(const std::vector<std::pair<Coord, Coord>>& pairs) {
        validPairsOnRow = pairs;
    }

    std::vector<std::pair<Coord, Coord>> Whirlpool::getValidPairsOnRow() const {
        return validPairsOnRow;
    }

    void Whirlpool::setValidPairsOnColumn(const std::vector<std::pair<Coord, Coord>>& pairs) {
        validPairsOnColumn = pairs;
    }

    std::vector<std::pair<Coord, Coord>> Whirlpool::getValidPairsOnColumn() const {
        return validPairsOnColumn;
    }


    std::vector<std::pair<Coord, Coord>> Whirlpool::getPairsOnRow(Board& board) {
        std::vector<std::pair<Coord, Coord>> valid_pairs_on_row;
        for (const auto& [coord, stack] : board) {

            Coord neighbor1{ coord.first + 2, coord.second };
            Coord neighbor2{ coord.first + 4, coord.second };
            bool valid_option = (stack.size() == 1 && !board.hasStack(neighbor1) && board.hasStack(neighbor2)
                && board[neighbor2].size() == 1) ? true : false;
            if (valid_option) {
                valid_pairs_on_row.emplace_back(std::make_pair(coord, neighbor2));
            }
        }
        return valid_pairs_on_row;
    }
    std::vector<std::pair<Coord, Coord>> Whirlpool::getPairsOnColumn(Board& board) {
        std::vector<std::pair<Coord, Coord>> valid_pairs_on_column;
        for (const auto& [coord, stack] : board) {

            Coord neighbor1{ coord.first, coord.second + 1 };
            Coord neighbor2{ coord.first, coord.second + 2 };

            bool valid_option = (stack.size() == 1 && !board.hasStack(neighbor1) && board.hasStack(neighbor2)
                && board[neighbor2].size() == 1) ? true : false;

            if (valid_option) {
                valid_pairs_on_column.emplace_back(std::make_pair(coord, neighbor2));
            }
        }
        return valid_pairs_on_column;
    }


    ////------------------------------------------ Blizzard -------------------------------------------
    Blizzard::Blizzard() {
        m_ability = PowerCardType::Blizzard;
    }

    void Blizzard::setBlockChoice(char choice) {
        blockChoice = choice;
    }

    std::optional<char> Blizzard::getBlockChoice() const {
        return blockChoice;
    }

    void Blizzard::setChosenRow(uint16_t row) {
        chosenRow = row;
    }

    std::optional<uint16_t> Blizzard::getChosenRow() const {
        return chosenRow;
    }

    void Blizzard::setChosenColumn(uint16_t column) {
        chosenColumn = column;
    }

    std::optional<uint16_t> Blizzard::getChosenColumn() const {
        return chosenColumn;
    }

    void Blizzard::apply(Board& board, Player& player) {
        if (board.availableSpaces().empty()) {
            Logger::log(Level::WARNING, "No available spaces for opponent's next turn");
            return;
        }

        if (!getBlockChoice()) {
            Logger::log(Level::WARNING, "No block choice set (Row or Column)");
            return;
        }

        char choice = toupper(*getBlockChoice());

        if (choice == 'R') {
            std::unordered_set<uint16_t> rows;
            for (const auto& [coord, stack] : board.getCombatCards()) {
                rows.insert(coord.second);
            }
            for (const auto& y : board.availableSpaces()) {
                rows.insert(y.second);
            }

            if (!getChosenRow()) {
                Logger::log(Level::WARNING, "No row selected");
                return;
            }

            uint16_t chosen_row = *getChosenRow();

            if (rows.find(chosen_row) == rows.end()) {
                Logger::log(Level::WARNING, "Invalid row selection");
                return;
            }

            board.blockRow(chosen_row, player.getColor());
            Logger::log(Level::INFO, "Row {} has been blocked for the opponent's next turn", chosen_row);
        }
        else if (choice == 'C') {
            std::unordered_set<uint16_t> columns;
            for (const auto& [coord, stack] : board.getCombatCards()) {
                columns.insert(coord.first);
            }
            for (const auto& x : board.availableSpaces()) {
                columns.insert(x.first);
            }

            if (!getChosenColumn()) {
                Logger::log(Level::WARNING, "No column selected");
                return;
            }

            uint16_t chosen_column = *getChosenColumn();

            if (columns.find(chosen_column) == columns.end()) {
                Logger::log(Level::WARNING, "Invalid column selection");
                return;
            }

            board.blockColumn(chosen_column, player.getColor());
            Logger::log(Level::INFO, "Column {} has been blocked for the opponent's next turn", chosen_column);
        }
        else {
            Logger::log(Level::WARNING, "Invalid choice. Please enter R for Row or C for Column");
        }
    }
    ////------------------------------------------ Waterfall -------------------------------------------
    Waterfall::Waterfall() {
        m_ability = PowerCardType::Waterfall;
    }

    void Waterfall::setOrientation(Orientation orientation) {
        selectedOrientation = orientation;
    }

    std::optional<Orientation> Waterfall::getOrientation() const {
        return selectedOrientation;
    }

    void Waterfall::setIndex(int16_t index) {
        selectedIndex = index;
    }

    std::optional<int16_t> Waterfall::getIndex() const {
        return selectedIndex;
    }

    void Waterfall::setMoveDirection(MoveDirection direction) {
        selectedMoveDirection = direction;
    }

    std::optional<MoveDirection> Waterfall::getMoveDirection() const {
        return selectedMoveDirection;
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
        return choices;
    }

    std::tuple<Orientation, int16_t, MoveDirection> Waterfall::input(Board& board) {
        auto options = getOptions(board);

        if (options.empty()) {
            Logger::log(Level::WARNING, "You can't use this mage right now");
            return { Orientation::Unknown, 0, MoveDirection::Unknown };
        }

        if (!getOrientation() || !getIndex() || !getMoveDirection()) {
            Logger::log(Level::WARNING, "Not all choices have been set");
            return { Orientation::Unknown, 0, MoveDirection::Unknown };
        }

        Orientation orient = *getOrientation();
        int16_t index = *getIndex();
        MoveDirection move_dir = *getMoveDirection();

        auto it = options.find(orient);
        if (it == options.end() || std::find(it->second.begin(), it->second.end(), index) == it->second.end()) {
            Logger::log(Level::WARNING, "Wrong choice");
            return { Orientation::Unknown, 0, MoveDirection::Unknown };
        }

        auto dirIt = validDirections.find(orient);
        if (std::find(dirIt->second.begin(), dirIt->second.end(), move_dir) == dirIt->second.end()) {
            Logger::log(Level::WARNING, "Wrong choice");
            return { Orientation::Unknown, 0, MoveDirection::Unknown };
        }

        Logger::log(Level::INFO, "Valid choice");
        return { orient, index, move_dir };
    }

    void Waterfall::apply(Board& board, Player& player) {
        auto choice = input(board);
        auto orient = std::get<0>(choice);
        auto index = std::get<1>(choice);
        auto dir = std::get<2>(choice);

        if (orient == Orientation::Row) {
            auto vect = board.getCoordsOnRow(index);
            if (dir == MoveDirection::Right) {
                auto destination = board.getRightMostOnRow(index);
                board.mergeStacks(vect, destination);
            }
            else {
                auto destination = board.getLeftMostOnRow(index);
                board.mergeStacks(vect, destination);
            }
        }
        else {
            auto vect = board.getCoordsOnColumn(index);
            if (dir == MoveDirection::Up) {
                auto destination = board.getTopMostOnColumn(index);
                board.mergeStacks(vect, destination);
            }
            else {
                auto destination = board.getBottomMostOnColumn(index);
                board.mergeStacks(vect, destination);
            }
        }
    }


    ////------------------------------------------ Support -------------------------------------------
    Support::Support() {
        m_ability = PowerCardType::Support;
    }

    void Support::setSelectedCoord(const Coord& coord) {
        selectedCoord = coord;
    }

    std::optional<Coord> Support::getSelectedCoord() const {
        return selectedCoord;
    }

    void Support::apply(Board& board, Player& player) {
        std::vector<Coord> validCards = CoordCardType(board, player);
        if (validCards.empty()) {
            Logger::log(Level::WARNING, "No valid cards to apply the power card");
            return;
        }

        if (!getSelectedCoord()) {
            Logger::log(Level::WARNING, "No card selected");
            return;
        }

        Coord new_coord = *getSelectedCoord();

        if (std::find(validCards.begin(), validCards.end(), new_coord) == validCards.end()) {
            Logger::log(Level::WARNING, "No valid card at those coordinates");
            return;
        }

        board.incrementCardValue(new_coord);
        Logger::log(Level::INFO, "Support power used!");
    }

    std::vector<Coord> Support::CoordCardType(Board& board, const Player& player) const {
        std::vector<Coord> coordCards;
        for (const auto& [coord, stack] : board) {
            if (stack.empty()) continue;
            const CombatCard& card = stack.back();
            if (card.getColor() == player.getColor() &&
                (card.getType() == CombatCardType::TWO ||
                    card.getType() == CombatCardType::THREE ||
                    card.getType() == CombatCardType::ONE)) {
                coordCards.emplace_back(coord);
            }
        }
        return coordCards;
    }


    ////------------------------------------------ Earthquake -------------------------------------------
    Earthquake::Earthquake() {
        m_ability = PowerCardType::Earthquake;
    }

    void Earthquake::setCardsToRemove(const std::vector<Coord>& cards) {
        cardsToRemove = cards;
    }

    std::vector<Coord> Earthquake::getCardsToRemove() const {
        return cardsToRemove;
    }

    void Earthquake::apply(Board& board, Player& player) {
        std::vector<Coord> foundCards;

        for (const auto& [coord, stack] : board) {
            if (stack.empty()) continue;

            const CombatCard& topCard = stack.back();
            if (topCard.getType() == CombatCardType::ONE) {
                foundCards.push_back(coord);
            }
        }

        if (foundCards.empty()) {
            Logger::log(Level::WARNING, "No visible cards with value ONE found on the board");
            return;
        }

        setCardsToRemove(foundCards);

        for (const auto& coord : getCardsToRemove()) {
            try {
                board.removeTopCardAt(coord);
                Logger::log(Level::INFO, "Removed card with value ONE at coordinates ({}, {})",
                    coord.first, coord.second);
            }
            catch (const std::runtime_error& err) {
                Logger::log(Level::WARNING,
                    "Cannot remove card at ({}, {}) because it would break adjacency rule",
                    coord.first, coord.second);
                continue;
            }
        }
    }


    ////------------------------------------------ Crumble -------------------------------------------
    Crumble::Crumble() {
        m_ability = PowerCardType::Crumble;
    }
    void Crumble::setValidCards(const std::vector<Coord>& cards) {
        validCards = cards;
    }

    std::vector<Coord> Crumble::getValidCards() const {
        return validCards;
    }

    void Crumble::setSelectedCard(const Coord& coord) {
        selectedCard = coord;
    }

    std::optional<Coord> Crumble::getSelectedCard() const {
        return selectedCard;
    }
    std::vector<Coord> Crumble::findValidCards(const Board& board, const Player& player) const {
        std::vector<Coord> foundCards;
        for (const auto& [coord, stack] : board) {
            if (stack.empty()) continue;

            const CombatCard& card = stack.back();
            if (!board.isDecrementedCard(coord) &&
                card.getColor() != player.getColor() &&
                (card.getType() == CombatCardType::TWO ||
                    card.getType() == CombatCardType::THREE ||
                    card.getType() == CombatCardType::FOUR)) {
                foundCards.emplace_back(coord);
            }
        }
        return foundCards;
    }
    void Crumble::apply(Board& board, Player& player) {
        std::vector<Coord> foundCards = findValidCards(board, player);

        if (foundCards.empty()) {
            Logger::log(Level::WARNING, "No valid cards to apply the power card");
            return;
        }
        setValidCards(foundCards);

        if (!getSelectedCard()) {
            Logger::log(Level::WARNING, "No card selected to decrease");
            return;
        }

        Coord selected = *getSelectedCard();

        if (std::find(validCards.begin(), validCards.end(), selected) == validCards.end()) {
            Logger::log(Level::WARNING, "No valid card at those coordinates");
            return;
        }

        board.decrementCard(selected);
    }


    ////------------------------------------------ Border -------------------------------------------


    Border::Border() {
        m_ability = PowerCardType::Border;
    }

    void Border::setValidPositions(const std::set<int>& rows, const std::set<int>& cols) {
        validRows = rows;
        validCols = cols;
    }

    std::pair<std::set<int>, std::set<int>> Border::getValidPositions() const {
        return { validRows, validCols };
    }

    void Border::setSelectedCoord(const Coord& coord) {
        selectedCoord = coord;
    }

    std::optional<Coord> Border::getSelectedCoord() const {
        return selectedCoord;
    }

    bool Border::applyNeutralCard(Player& player, Board& board) {
        auto rect_size = board.getBoundingRectSize();
        std::set<int> unique_cols;
        std::set<int> unique_rows;

        if (rect_size.first == 3) {
            auto unfixed_col = board.getUnfixedColumns();
            auto available_spaces = board.availableSpaces();
            for (auto y : available_spaces) {
                if (unfixed_col.find(y.first) == unfixed_col.end()) {
                    unique_cols.insert(y.first);
                }
            }
        }
        else if (rect_size.second == 3) {
            auto unfixed_rows = board.getUnfixedRows();
            auto available_spaces = board.availableSpaces();
            for (auto x : available_spaces) {
                if (unfixed_rows.find(x.second) == unfixed_rows.end()) {
                    unique_rows.insert(x.second);
                }
            }
        }
        else {
            Logger::log(Level::WARNING, "You cannot use this power right now");
            return false;
        }

        setValidPositions(unique_rows, unique_cols);

        if (!getSelectedCoord()) {
            Logger::log(Level::WARNING, "No coordinates selected for placement");
            return false;
        }

        Coord coords = *getSelectedCoord();
        bool isValidPosition = validRows.contains(coords.second) || validCols.contains(coords.first);

        if (isValidPosition) {
            CombatCard card(CombatCardType::BORDER, player.getColor());
            board.appendMove(coords, std::move(card));
            return true;
        }
        else {
            Logger::log(Level::WARNING, "Invalid coordinate choice");
            return false;
        }
    }

    void Border::apply(Board& board, Player& player) {
        if (!applyNeutralCard(player, board)) {
            return;
        }

        if (!getSelectedCoord()) {
            Logger::log(Level::WARNING, "No second card selected");
            return;
        }

        Coord coord_input = *getSelectedCoord();

        if (!selectedCardType) {
            Logger::log(Level::WARNING, "No card type selected");
            return;
        }

        CombatCard card(*selectedCardType, player.getColor());
        board.appendMove(coord_input, std::move(card));
        Logger::log(Level::INFO, "Border power used");
    }

    void Border::setSelectedCardType(CombatCardType type) {
        selectedCardType = type;
    }

    std::optional<CombatCardType> Border::getSelectedCardType() const {
        return selectedCardType;
    }


    ////------------------------------------------ Avalanche -------------------------------------------

    Avalanche::Avalanche() {
        m_ability = PowerCardType::Avalanche;
    }

    void Avalanche::setSelectedMove(const std::pair<MoveDirection, std::pair<Coord, Coord>>& move) {
        selectedMove = move;
    }

    std::optional<std::pair<MoveDirection, std::pair<Coord, Coord>>> Avalanche::getSelectedMove() const {
        return selectedMove;
    }

    std::vector<std::pair<Orientation, std::pair<Coord, Coord>>> Avalanche::getPairs(Board& board) {
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

    void Avalanche::apply(Board& board, Player& player) {
        auto pairs = getPairs(board);
        if (pairs.empty()) {
            Logger::log(Level::WARNING, "You can't use this card right now");
            return;
        }

        auto choices = checkShifting(pairs, board);
        if (choices.empty()) {
            Logger::log(Level::WARNING, "No valid moves available");
            return;
        }

        if (!getSelectedMove()) {
            Logger::log(Level::WARNING, "No move selected");
            return;
        }

        auto choice = *getSelectedMove();
        auto it = std::find(choices.begin(), choices.end(), choice);

        if (it != choices.end()) {
            const auto& [type, selectedPair] = *it;
            const std::unordered_map<MoveDirection, Coord> directionDeltas = {
                {MoveDirection::Down, {0, 1}},
                {MoveDirection::Up, {0, -1}},
                {MoveDirection::Right, {2, 0}},
                {MoveDirection::Left, {-2, 0}}
            };

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
            }
            else {
                Logger::log(Level::WARNING, "Invalid choice!");
            }
        }
        else {
            Logger::log(Level::WARNING, "Invalid choice!");
        }

        Logger::log(Level::INFO, "Avalanche card used");
    }



    ////------------------------------------------ Rock -------------------------------------------
    Rock::Rock() {  ///////////////////////////////// foloseste iluzie, nu merge 
        m_ability = PowerCardType::Rock;
    }

    void Rock::apply(Board& board, Player& player) {
        std::vector<Coord>illusionCoord = getIllusionCoords(board);
        if (!illusionCoord.empty() && player.hasCards()) {

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
