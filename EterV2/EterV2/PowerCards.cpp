#include "PowerCards.h"
//#include<set>
using namespace logger;
//
namespace base {
    
        ////------------------------------------------ ControllerExplosion -------------------------------------------
        ControllerExplosion::ControllerExplosion(Board& m_board, Player& red, Player& blue, ExplosionService& explosion_service)
            : PowerCard(m_board, red, blue),
            m_explosion_service{explosion_service} {
            m_ability = PowerCardType::ControllerExplosion;
        }
        
        bool ControllerExplosion::apply() {
            if (!m_explosion_service.used()) {
                return false;
            }

            return true;
        }

        ExplosionService& ControllerExplosion::getExplosionService() {
            return m_explosion_service;
        }

        bool ControllerExplosion::isAvailable() const {
            return m_explosion_service.used();
        }
    
        ////------------------------------------------ Destruction -------------------------------------------
    Destruction::Destruction(Board& m_board, Player& red, Player& blue)
        : PowerCard(m_board, red, blue) {

        m_ability = PowerCardType::Destruction;
    }

    bool Destruction::canUseAbility(color::ColorType color) const {
        if (!m_board.hasLastPlayedCard()) {
            return false;
        }
        if (m_board.getLastPlayerColor() == color) {
            return false;
        }
        return true;
    }

    uint16_t Destruction::getTargetCardID() const {

        return m_board.getTopCard(m_board.getLastCardPosition())->get().getID();
    }

    std::string Destruction::getErrorMessage(color::ColorType colorPlayer) const {
        if (!m_board.hasLastPlayedCard()) {
            return "No last played card to destroy";
        }
        if (m_board.getLastPlayerColor() == colorPlayer) {
            return "Last card was played by you, not your opponent";
        }
        return "";
    }

    void Destruction::setColor(color::ColorType colorPlayer) {
        m_color = colorPlayer;
    }

    bool Destruction::apply() {
        if (!canUseAbility(m_color)) {
            Logger::log(Level::WARNING, getErrorMessage(m_color));
            return false;
        }

        Coord last_position = m_board.getLastCardPosition();
        m_board.removeTopCardAt(last_position);

        Logger::log(Level::INFO, "Destruction power destroyed the last played card at ({}, {})",
            last_position.first, last_position.second);

        return true;
    }
    
        ///------------------------------------------ Flame -------------------------------------------
        Flame::Flame(Board& m_board, Player& red, Player& blue, IllusionService& illusion_service)
            :PowerCard(m_board, red, blue),
            m_illusion_service{illusion_service} {  //iluzie                        ///////////////// asta e cu iluzie, nu merge
            m_ability = PowerCardType::Flame;
        }

        void Flame::setColor(color::ColorType colorPlayer) {
            m_color = colorPlayer;
        }
    
        bool Flame::apply() {
            Player& enemy_player{ (m_color == color::ColorType::RED) ? m_player_blue : m_player_red };

            if (m_illusion_service.hasIllusion(enemy_player)) {
                return false;
            }
            


            return false;
        }
    
        ////------------------------------------------ Fire -------------------------------------------
    //


    Fire::Fire(Board& m_board, Player& red, Player& blue) : PowerCard(m_board, red, blue) {

        m_ability = PowerCardType::Fire;
    }

    std::vector<std::pair<Coord, CombatCardType>> Fire::getVisibleCards() const {
        return std::ranges::views::filter(m_board, [](const auto& pair) {
            const auto& stack = pair.second;
            return !stack.empty() && !stack.back().isIllusion();
            }) | std::ranges::views::transform([](const auto& pair) {
                return std::pair{ pair.first, pair.second.back().getType() };
                }) | std::ranges::to<std::vector>();
    }

    bool Fire::canUseAbility() const {
        auto visible_cards = getVisibleCards();
        if (visible_cards.empty()) {
            return false;
        }

        auto valid_types = getValidChoices();
        return !valid_types.empty();
    }

    std::vector<CombatCardType> Fire::getValidChoices() const {
        auto visible_cards = getVisibleCards();
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

    std::string Fire::getErrorMessage() const {
        auto visible_cards = getVisibleCards();
        if (visible_cards.empty()) {
            return "No visible cards on the m_board";
        }

        auto valid_types = getValidChoices();
        if (valid_types.empty()) {
            return "No card type appears at least twice on the m_board";
        }

        return "";
    }

    void Fire::applyEffect(CombatCardType chosen_type) {
        auto visible_cards = getVisibleCards();

        for (const auto& [coord, type] : visible_cards) {
            if (type == chosen_type) {
                m_board.removeTopCardAt(coord);
                m_board.returnUsedCardToHand(type);
            }
        }

        Logger::log(Level::INFO, "Fire power returned all visible {} cards to their owners",
            combatCardToChar(chosen_type));
    }

    void Fire::setChosenCardByID(uint16_t card_id) {
        if (auto card_type = getCardTypeByID(card_id)) {
            m_chosen_card = *card_type;
            m_has_choice = true;
        }
    }

    void Fire::setChosenCard(CombatCardType card_type) {
        m_chosen_card = card_type;
        m_has_choice = true;
    }

    std::unordered_set<uint16_t> Fire::getVisibleCardIDs() const {
        std::unordered_set<uint16_t> card_ids;
        auto visible_cards = getVisibleCards();

        for (const auto& [coord, _] : visible_cards) {
            const auto& stack = m_board[coord];
            card_ids.insert(stack.back().getID());
        }
        return card_ids;
    }

    std::unordered_set<uint16_t> Fire::getCardIDsOfType(CombatCardType type) const {
        std::unordered_set<uint16_t> card_ids;
        auto visible_cards = getVisibleCards();

        for (const auto& [coord, card_type] : visible_cards) {
            if (card_type == type) {
                const auto& stack = m_board[coord];
                card_ids.insert(stack.back().getID());
            }
        }
        return card_ids;
    }
    std::optional<CombatCardType> Fire::getCardTypeByID(uint16_t card_id) const {

        auto visible_cards = getVisibleCards();

        for (const auto& [coord, _] : visible_cards) {
            const auto& stack = m_board[coord];
            if (stack.back().getID() == card_id) {
                return stack.back().getType();
            }
        }
        return std::nullopt;
    }

    bool Fire::apply() {
        if (!canUseAbility()) {
            Logger::log(Level::WARNING, getErrorMessage());
            return false;
        }

        auto valid_choices = getValidChoices();

        if (!m_has_choice) {
            Logger::log(Level::WARNING, "No card type was chosen");
            return false;
        }

        if (!isValidChoice(m_chosen_card, valid_choices)) {
            Logger::log(Level::WARNING, "Invalid choice");
            return false;
        }

        applyEffect(m_chosen_card);
        m_has_choice = false;

        return false;
    }
    ////------------------------------------------ Ash -------------------------------------------
    Ash::Ash(Board& m_board, Player& red, Player& blue) : PowerCard(m_board, red, blue) {
        m_ability = PowerCardType::Ash;
    }

    bool Ash::canUseAbility(color::ColorType colorPlayer) const {
        if (m_player_blue.getColor() == colorPlayer) {
            return m_player_blue.hasUsedCards();
        }
        return m_player_red.hasUsedCards();
    }

    std::unordered_set<uint16_t>Ash::getUsedCardIDs(color::ColorType colorPlayer) const {
        if (!canUseAbility(colorPlayer)) {
            return {};
        }

        std::unordered_set<uint16_t> card_ids;
        const auto& player = (m_player_blue.getColor() == colorPlayer) ? m_player_blue : m_player_red;
        const auto& cards = player.getUsedCards();

        for (const auto& card : cards) {
            card_ids.insert(card.second.getID());
        }
        return card_ids;
    }

    std::optional<std::pair<CombatCardType, color::ColorType>>Ash::getCardInfoByID(uint16_t card_id) const {
        const auto& player = (m_color == color::ColorType::BLUE) ? m_player_blue : m_player_red;
        const auto& used_cards = player.getUsedCards();

        for (const auto& [type, card] : used_cards) {
            if (card.getID() == card_id) {
                return std::make_pair(card.getType(), card.getColor());
            }
        }
        return std::nullopt;
    }

    std::vector<Ash::UsedCardInfo> Ash::getUsedCardsInfo(color::ColorType colorPlayer) const {
        if (!canUseAbility(colorPlayer)) {
            return {};
        }

        std::vector<UsedCardInfo> used_cards;
        if (m_player_blue.getColor() == colorPlayer) {
            const auto& cards = m_player_blue.getUsedCards();
            for (const auto& card : cards) {
                used_cards.push_back({
                    card.second.getType(),
                    card.second.getColor()
                    });
            }
        }
        else {
            const auto& cards = m_player_red.getUsedCards();
            for (const auto& card : cards) {
                used_cards.push_back({
                    card.second.getType(),
                    card.second.getColor()
                    });
            }
        }
        return used_cards;
    }


    std::string Ash::getErrorMessage(color::ColorType color) const {
        if (m_player_blue.getColor() == color) {
            if (!m_player_blue.hasUsedCards()) {
                return "You don't have any removed card";
            }
        }
        else {
            if (!m_player_red.hasUsedCards()) {
                return "You don't have any removed card";
            }
        }
        return "";
    }

    void Ash::setSelection(const Coord& coordinates, CombatCardType card_type) {
        m_selected_coord = coordinates;
        m_selected_card = card_type;
        m_has_selection = true;
    }

    bool Ash::apply() {
        if (!canUseAbility(m_color)) {
            Logger::log(Level::WARNING, getErrorMessage(m_color));
            return false;
        }

        if (!m_has_selection) {
            Logger::log(Level::WARNING, "No selection was made");
            return false;
        }
        if (m_color == color::ColorType::BLUE) {
            auto card = m_player_blue.getUsedCard(m_selected_card);
            m_board.appendMove(m_selected_coord, std::move(card));

            Logger::log(Level::INFO, "Ash power card was used");
        }
        if (m_color == color::ColorType::RED) {
            auto card = m_player_red.getUsedCard(m_selected_card);
            m_board.appendMove(m_selected_coord, std::move(card));

            Logger::log(Level::INFO, "Ash power card was used");
        }

        m_has_selection = false;
        return false;
    }

    void Ash::setColor(color::ColorType colorPlayer) {
        m_color = colorPlayer;
    }

    void Ash::setSelectionByID(const Coord& coordinates, uint16_t card_id) {
        const auto& player = (m_color == color::ColorType::BLUE) ? m_player_blue : m_player_red;
        const auto& used_cards = player.getUsedCards();

        for (const auto& [type, card] : used_cards) {
            if (card.getID() == card_id) {
                m_selected_coord = coordinates;
                m_selected_card = card.getType();
                m_has_selection = true;
                return;
            }
        }
    }


    ////------------------------------------------ Spark -------------------------------------------
    Spark::Spark(Board& m_board, Player& red, Player& blue) :PowerCard(m_board, red, blue) {
        m_ability = PowerCardType::Spark;
    }

    void Spark::setAvailableChoices(const std::vector<std::pair<Coord, CombatCardType>>& choices) {
        availableChoices = choices;
    }

    void Spark::setSelectedFromCoord(Coord coord) {
        selectedFromCoord = coord;
    }

    void Spark::setSelectedCardType(CombatCardType type) {
        selectedCardType = type;
    }
    void Spark::setMoveDestination(Coord coord) {
        moveDestination = coord;
    }

    void Spark::setColor(color::ColorType colorPlayer) {
        m_color = colorPlayer;
    }

    void Spark::setSelectionByID(uint16_t card_id) {
        if (auto card_info = getCardInfoByID(card_id)) {
            selectedFromCoord = card_info->coord;
            selectedCardType = card_info->type;
        }
    }

    std::optional<CombatCardType> Spark::getSelectedCardType() const {
        return selectedCardType;
    }

    std::optional<Coord> Spark::getSelectedFromCoord() const {
        return selectedFromCoord;
    }

    std::optional<Coord> Spark::getMoveDestination() const {
        return moveDestination;
    }

    std::unordered_set<uint16_t> Spark::getCoveredCardIDs() {
        std::unordered_set<uint16_t> card_ids;
        for (const auto& [coord, stack] : m_board) {
            if (stack.size() < 2) continue;
            for (size_t i = 0; i < stack.size() - 1; ++i) {
                const CombatCard& card = stack[i];
                if ((m_color == color::ColorType::BLUE && card.getColor() == m_player_blue.getColor()) ||
                    (m_color == color::ColorType::RED && card.getColor() == m_player_red.getColor())) {
                    card_ids.insert(card.getID());
                }
            }
        }
        return card_ids;
    }

    std::optional<Spark::CardInfo>Spark::getCardInfoByID(uint16_t card_id) const {
        for (const auto& [coord, stack] : m_board) {
            if (stack.size() < 2) continue;
            for (size_t i = 0; i < stack.size() - 1; ++i) {
                const CombatCard& card = stack[i];
                if (card.getID() == card_id) {
                    return CardInfo{ coord, card.getType(), card.getID() };
                }
            }
        }
        return std::nullopt;
    }

    bool Spark::apply() {
        auto choices = coverCards();
        setAvailableChoices(choices);

        if (choices.empty()) {
            Logger::log(Level::WARNING, "You have no covered cards on the m_board!");
            return false;
        }

        if (!getSelectedFromCoord()) {
            Logger::log(Level::WARNING, "No card selected!");
            return false;
        }

        auto it = std::find_if(choices.begin(), choices.end(),
            [this](const auto& pair) { return pair.first == *getSelectedFromCoord(); });

        if (it == choices.end()) {
            Logger::log(Level::WARNING, "You don't have any covered card at these coordinates!");
            return false;
        }

        if (!getSelectedCardType()) {
            Logger::log(Level::WARNING, "No card type selected!");
            return false;
        }

        if (it->second != *getSelectedCardType()) {
            Logger::log(Level::WARNING, "This card doesn't exist at the specified position!");
            return false;
        }

        if (!getMoveDestination()) {
            Logger::log(Level::WARNING, "No move destination selected!");
            return false;
        }

        CombatCard card(*getSelectedCardType(), m_player_red.getColor());

        if (!m_board.isValidPlaceCard(*getMoveDestination(), card)) {
            Logger::log(Level::WARNING, "You can't move the card to this position!");
            return false;
        }

        m_board.removeCardFromStackAt(*getSelectedFromCoord(), card);
        m_board.appendMove(*getMoveDestination(), std::move(card));

        Logger::log(Level::INFO, "Card successfully moved!");
        return false;
    }

    std::vector<std::pair<Coord, CombatCardType>> Spark::coverCards() {
        std::vector<std::pair<Coord, CombatCardType>> choices;
        for (const auto& [coord, stack] : m_board) {
            if (stack.size() < 2) continue;
            for (size_t i = 0; i < stack.size() - 1; ++i) {
                const CombatCard& card = stack[i];
                if (m_color == color::ColorType::BLUE) {
                    if (card.getColor() == m_player_blue.getColor()) {
                        choices.emplace_back(coord, card.getType());
                    }
                }
                else {
                    if (card.getColor() == m_player_red.getColor()) {
                        choices.emplace_back(coord, card.getType());
                    }
                }

            }
        }
        return choices;
    }

    ////------------------------------------------ Squall -------------------------------------------
    Squall::Squall(Board& m_board, Player& red, Player& blue) :PowerCard(m_board, red, blue) {
        m_ability = PowerCardType::Squall;
    }
    void Squall::setSelectedCardID(uint16_t card_id) {
        m_selected_card_id = card_id;

    }

    void Squall::setVisibleCards(const std::vector<std::pair<Coord, CombatCardType>>& cards) {
        visibleCards = cards;
    }

    void Squall::setSelectedCardCoord(Coord coord) {
        selectedCardCoord = coord;
    }

    void Squall::setColor(color::ColorType colorPlayer) {
        m_color = colorPlayer;
    }

    std::optional<uint16_t> Squall::getSelectedCardID() const {
        return m_selected_card_id;
    }

    std::optional<Coord> Squall::getSelectedCardCoord() const {
        return selectedCardCoord;
    }

    std::vector<std::pair<Coord, CombatCardType>> Squall::getVisibleCards() {
        std::vector<std::pair<Coord, CombatCardType>> visible_cards;
        for (const auto& [coord, stack] : m_board) {
            if (!stack.empty()) {
                const CombatCard& top_card = stack.back();
                if (!top_card.isIllusion() && top_card.getColor() != m_color) {
                    visible_cards.emplace_back(coord, top_card.getType());
                }
            }
        }
        return visible_cards;
    }

    std::optional<CombatCardType> Squall::getCardTypeByID(uint16_t card_id) const {
        for (const auto& [coord, stack] : m_board) {
            if (!stack.empty() && stack.back().getID() == card_id) {
                return stack.back().getType();
            }
        }
        return std::nullopt;
    }

    std::unordered_set<uint16_t> Squall::getVisibleCardsIDs() const {
        std::unordered_set<uint16_t> visible_cards_ids;
        for (const auto& [coord, stack] : m_board) {
            if (!stack.empty()) {
                const CombatCard& top_card = stack.back();
                if (!top_card.isIllusion() && top_card.getColor() != m_color) {
                    visible_cards_ids.insert(top_card.getID());
                }
            }
        }
        return visible_cards_ids;
    }

    bool Squall::apply() {
        auto visible_cards = getVisibleCards();
        setVisibleCards(visible_cards);

        if (visible_cards.empty()) {
            Logger::log(Level::WARNING, "No visible opponent cards on the m_board!");
            return false;
        }

        if (!getSelectedCardCoord()) {
            Logger::log(Level::WARNING, "No card selected!");
            return false;
        }

        auto it = std::find_if(visible_cards.begin(), visible_cards.end(),
            [this](const auto& pair) { return pair.first == *getSelectedCardCoord(); });

        if (it == visible_cards.end()) {
            Logger::log(Level::WARNING, "Invalid coordinates! No opponent card there.");
            return false;
        }
        if (m_color == color::ColorType::RED) {
            m_player_red.addCard(m_board.popTopCardAt(*getSelectedCardCoord()));
        }
        else {
            m_player_blue.addCard(m_board.popTopCardAt(*getSelectedCardCoord()));
        }

        Logger::log(Level::INFO, "Card successfully returned to opponent's hand!");
        return false;
    }

    //    ////------------------------------------------ Gale -------------------------------------------
    Gale::Gale(Board& m_board, Player& red, Player& blue) :PowerCard(m_board, red, blue) {
        m_ability = PowerCardType::Gale;
    }

    bool Gale::apply() {
        std::vector<std::pair<Coord, CombatCardType>> cards_to_remove;


        for (const auto& [coord, stack] : m_board) {
            if (stack.size() <= 1) {
                continue;
            }

            for (size_t i = 0; i < stack.size() - 1; i++) {
                cards_to_remove.emplace_back(coord, stack[i].getType());
            }
        }

        for (const auto& [coord, card_type] : cards_to_remove) {
            m_board.removeCardFromStackAt(coord, CombatCard(card_type, m_color));
            m_board.returnUsedCardToHand(card_type);
        }

        Logger::log(Level::INFO, "Gale power card effect completed");
        return false;
    }

    void Gale::setColor(color::ColorType colorPlayer) {
        m_color = colorPlayer;
    }

    //    ////------------------------------------------ Hurricane -------------------------------------------
    Hurricane::Hurricane(Board& m_board, Player& red, Player& blue) :PowerCard(m_board, red, blue) {
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

    bool Hurricane::apply() {
        auto options = getOptions();
        if (options.empty()) {
            Logger::log(Level::WARNING, "You can't use this mage right now");
            return false;
        }

        if (!getUserSelection() || !getUserDirection()) {
            Logger::log(Level::WARNING, "No valid selection or direction set!");
            return false;
        }

        auto [orient, index] = *getUserSelection();
        auto shift_dir = *getUserDirection();

        std::vector<std::pair<Coord, Coord>> to_shift;

        if (orient == Orientation::Row) {
            if (shift_dir == MoveDirection::Right) {
                Coord coord = m_board.getRightMostOnRow(index);
                auto stack_to_return = m_board.popStack(coord);
                for (auto& card : stack_to_return) {
                    m_board.returnCardToHand(std::move(card), card.getColor());
                }
                auto vect_coords = m_board.getCoordsOnRow(index);
                m_board.shiftRowToRight(vect_coords);
            }
            else {
                Coord coord = m_board.getLeftMostOnRow(index);
                auto stack_to_return = m_board.popStack(coord);
                for (auto& card : stack_to_return) {
                    m_board.returnCardToHand(std::move(card), card.getColor());
                }
                auto vect_coords = m_board.getCoordsOnRow(index);
                m_board.shiftRowToLeft(vect_coords);
            }
        }
        else {
            if (shift_dir == MoveDirection::Up) {
                Coord coord = m_board.getTopMostOnColumn(index);
                auto stack_to_return = m_board.popStack(coord);
                for (auto& card : stack_to_return) {
                    m_board.returnCardToHand(std::move(card), card.getColor());
                }
                auto vect_coord = m_board.getCoordsOnColumn(index);
                m_board.shiftColumnUp(vect_coord);
            }
            else {
                Coord coord = m_board.getBottomMostOnColumn(index);
                auto stack_to_return = m_board.popStack(coord);
                for (auto& card : stack_to_return) {
                    m_board.returnCardToHand(std::move(card), card.getColor());
                }
                auto vect_coord = m_board.getCoordsOnColumn(index);
                m_board.shiftColumnDown(vect_coord);
            }
        }
        return false;
    }
    //
    //
    //    ////------------------------------------------ Gust -------------------------------------------
    Gust::Gust(Board& m_board, Player& red, Player& blue) :PowerCard(m_board, red, blue) {
        m_ability = PowerCardType::Gust;
    }
    void Gust::setValidSourceCards(const std::vector<Coord>& sources) {
        validSourceCards = sources;
    }

    std::vector<Coord> Gust::getValidSourceCards() const {
        return validSourceCards;
    }

    std::unordered_set<uint16_t> Gust::getValidSourceCardIDs() const {
        std::unordered_set<uint16_t> valid_sources;
        for (const auto& [coord, stack] : m_board) {
            if (!stack.empty() && !stack.back().isIllusion()) {
                valid_sources.insert(stack.back().getID());
            }
        }
        return valid_sources;
    }

    std::unordered_set<uint16_t> Gust::getValidDestinationIDs() const {
        if (!m_selected_source_id) {
            return {};
        }

        std::unordered_set<uint16_t> valid_destinations;

        Coord sourceCoord;
        CombatCardType sourceValue;
        bool found = false;

        for (const auto& [coord, stack] : m_board) {
            if (!stack.empty() && stack.back().getID() == *m_selected_source_id) {
                sourceCoord = coord;
                sourceValue = stack.back().getType();
                found = true;
                break;
            }
        }

        if (!found) return {};
        const std::vector<std::pair<int16_t, int16_t>> adjacentOffsets = {
            {2, 0}, {-2, 0}, {0, 1}, {0, -1}
        };

        for (const auto& [dx, dy] : adjacentOffsets) {
            Coord adjCoord = { sourceCoord.first + dx, sourceCoord.second + dy };
            if (m_board.hasStack(adjCoord)) {
                auto& adjStack = m_board[adjCoord];
                if (!adjStack.empty() && !adjStack.back().isIllusion()) {
                    if (adjStack.back().getType() < sourceValue) {
                        valid_destinations.insert(adjStack.back().getID());
                    }
                }
            }
        }

        return valid_destinations;

    }

    std::optional<uint16_t> Gust::getSelectedSourceID() const {
        return m_selected_source_id;
    }

    std::optional<uint16_t> Gust::getSelectedDestinationID() const {
        return m_selected_destination_id;
    }

    void Gust::setSelectedSource(Coord coord) {
        selectedSource = coord;
    }

    void Gust::setSelectedDestinationID(uint16_t card_id) {
        m_selected_destination_id = card_id;
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

    void Gust::setSelectedSourceID(uint16_t card_id) {
        m_selected_source_id = card_id;
    }

    void Gust::setSelectedDestination(Coord coord) {
        selectedDestination = coord;
    }

    std::optional<Coord> Gust::getSelectedDestination() const {
        return selectedDestination;
    }

    bool Gust::apply() {
        auto valid_sources = getValidSourceCardIDs();
        if (valid_sources.empty()) {
            Logger::log(Level::WARNING, "No valid cards to move");
            return false;
        }

        if (!m_selected_source_id) {
            Logger::log(Level::WARNING, "No source card selected!");
            return false;
        }

        if (valid_sources.find(*m_selected_source_id) == valid_sources.end()) {
            Logger::log(Level::WARNING, "Invalid source card");
            return false;
        }

        auto valid_destinations = getValidDestinationIDs();
        if (valid_destinations.empty()) {
            Logger::log(Level::WARNING, "No valid destinations available");
            return false;
        }

        if (!m_selected_destination_id) {
            Logger::log(Level::WARNING, "No destination selected!");
            return false;
        }

        if (valid_destinations.find(*m_selected_destination_id) == valid_destinations.end()) {
            Logger::log(Level::WARNING, "Invalid destination card");
            return false;
        }

        Coord sourceCoord, destCoord;
        for (const auto& [coord, stack] : m_board) {
            if (!stack.empty()) {
                if (stack.back().getID() == *m_selected_source_id) {
                    sourceCoord = coord;
                }
                if (stack.back().getID() == *m_selected_destination_id) {
                    destCoord = coord;
                }
            }
        }

        CombatCard card = std::move(m_board.popTopCardAt(sourceCoord));
        m_board.appendMove(destCoord, std::move(card));
        Logger::log(Level::INFO, "Gust power: Successfully moved card!");
        return false;
    }

        ////------------------------------------------ Mirrage -------------------------------------------
        Mirrage::Mirrage(Board& m_board, Player& red, Player& blue):PowerCard(m_board, red, blue) {                                 /////////////// iluzie, nu merge
            m_ability = PowerCardType::Mirrage;
        }
    
        bool Mirrage::apply() {
            if (getIllusion(m_board, m_player_red)) {
                std::cout << "Choose the coordinates for the new illusion and the new illusion" << std::endl;
                Coord coord;
                std::cin >> coord.first >> coord.second;
    
                char card_type;
                std::cin >> card_type;
                auto card = m_player_red.getCard(charToCombatCard(card_type));
                //  if (m_board.isValidMove(coord, card)) {
                card.flip();
                m_board.appendMove(coord, std::move(card));
                Logger::log(Level::INFO, "Mirrage power card was used");
                // }
            }

            return false;
        }
    
        bool Mirrage::getIllusion(Board& m_board, Player& m_player_red) {
            bool foundIllusion = false;
            for (const auto& [coord, stack] : m_board) {
                auto top_card = m_board.getTopCard(coord);
                CombatCard& card = top_card->get();
                if (card.isIllusion() && card.getColor() == m_player_red.getColor()) {
                    card.flip();
                    m_player_red.addCard(std::move(card));
                    CombatCard& card_value = card;
                    m_board.removeTopCardAt(coord);
                    m_board.availableSpaces();
                    Logger::log(Level::INFO, "You successfully received the illusion back");
                    foundIllusion = true;
                    break;
                }
            }
            if (!foundIllusion) {
                Logger::log(Level::INFO, "You don't have any illusion on the m_board");
            }
            return foundIllusion;
        }
    
    
        ////------------------------------------------ Storm -------------------------------------------
    Storm::Storm(Board& m_board, Player& red, Player& blue) :PowerCard(m_board, red, blue) {
        m_ability = PowerCardType::Storm;
    }

    void Storm::setAvailableStacks(const std::vector<Coord>& stacks) {
        availableStacks = stacks;
    }

    void Storm::setSelectedStackID(uint16_t card_id) {
        m_selected_stack_id = card_id;
    }

    void Storm::setSelectedStack(size_t index) {
        selectedStack = index;
    }

    std::optional<size_t> Storm::getSelectedStack() const {
        return selectedStack;
    }

    std::optional<uint16_t> Storm::getSelectedStackID() const {
        return m_selected_stack_id;
    }

    std::vector<Coord> Storm::getAvailableStacks() const {
        return availableStacks;
    }

    std::unordered_set<uint16_t> Storm::getAvailableStackIDs() const {
        std::unordered_set<uint16_t> stacks_ids;
        for (const auto& [coord, stack] : m_board) {
            if (stack.size() > 1) {
                stacks_ids.insert(stack.back().getID());
            }
        }
        return stacks_ids;
    }

    std::optional<size_t> Storm::getStackSize(uint16_t card_id) const {
        for (const auto& [coord, stack] : m_board) {
            if (!stack.empty() && stack.back().getID() == card_id) {
                return stack.size();
            }
        }
        return std::nullopt;
    }

    bool Storm::apply() {
        auto stacks_ids = getAvailableStackIDs();
        if (stacks_ids.empty()) {
            Logger::log(Level::WARNING, "No stacks with 2 or more cards found on the board");
            return false;
        }

        if (!m_selected_stack_id) {
            Logger::log(Level::WARNING, "No stack selected");
            return false;
        }

        if (stacks_ids.find(*m_selected_stack_id) == stacks_ids.end()) {
            Logger::log(Level::WARNING, "Invalid stack selection");
            return false;
        }

        for (const auto& [coord, stack] : m_board) {
            if (!stack.empty() && stack.back().getID() == *m_selected_stack_id) {
                try {
                    if (m_board.isValidRemoveStack(coord)) {
                        m_board.removeStack(coord);
                        Logger::log(Level::INFO, "Successfully removed stack");
                    }
                }
                catch (const std::runtime_error& e) {
                    Logger::log(Level::ERROR, "Failed to remove stack: {}", e.what());
                }
                return false;
            }
        }
        return false;
    }
    //
    //    ////------------------------------------------ Tide -------------------------------------------
    Tide::Tide(Board& m_board, Player& red, Player& blue) :PowerCard(m_board, red, blue) {
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

    void Tide::setSelectedStackIDs(uint16_t first_id, uint16_t second_id) {
        m_selected_stack_ids = std::make_pair(first_id, second_id);
    }

    std::optional<std::pair<Coord, Coord>> Tide::getSelectedStacks() const {
        return selectedStacks;
    }

    std::unordered_set<uint16_t> Tide::getAvailableStackIDs() const {
        std::unordered_set<uint16_t> stack_ids;
        for (const auto& [coord, stack] : m_board) {
            if (!stack.empty()) {
                stack_ids.insert(stack.back().getID());
            }
        }
        return stack_ids;
    }

    std::optional<std::pair<uint16_t, uint16_t>> Tide::getSelectedStackIDs() {
        return m_selected_stack_ids;
    }

    std::vector<Coord> Tide::getStacks() {
        std::vector<Coord> stacks_coord;
        for (const auto& [coord, stack] : m_board) {
            if (!stack.empty()) {
                stacks_coord.emplace_back(coord);
            }
        }
        return stacks_coord;
    }

    bool Tide::apply() {
        auto available_stacks = getAvailableStackIDs();
        if (available_stacks.size() < 2) {
            Logger::log(Level::WARNING, "There are not enough stacks to swap.");
            return false;
        }

        if (!m_selected_stack_ids) {
            Logger::log(Level::WARNING, "No stacks selected for swapping.");
            return false;
        }

        auto [first_id, second_id] = *m_selected_stack_ids;

        if (available_stacks.find(first_id) == available_stacks.end() ||
            available_stacks.find(second_id) == available_stacks.end()) {
            Logger::log(Level::WARNING, "Invalid stack selection.");
            return false;
        }

        Coord coord_from, coord_to;
        bool found_first = false, found_second = false;

        for (const auto& [coord, stack] : m_board) {
            if (!stack.empty()) {
                if (stack.back().getID() == first_id) {
                    coord_from = coord;
                    found_first = true;
                }
                else if (stack.back().getID() == second_id) {
                    coord_to = coord;
                    found_second = true;
                }
            }
            if (found_first && found_second) break;
        }

        if (!found_first || !found_second) {
            Logger::log(Level::WARNING, "Could not find selected stacks.");
            return false;
        }

        m_board.swapStacks(coord_from, coord_to);
        Logger::log(Level::INFO, "Tide power card was used to swap stacks successfully.");
        return false;
    }


    
        ////------------------------------------------ Mist -------------------------------------------
        Mist::Mist(Board& m_board, Player& red, Player& blue):PowerCard(m_board, red, blue) { /////////////////////////////////////////////////////////joaca inca o iluzie, nu merge
            m_ability = PowerCardType::Mist;
    
        }
    
        bool Mist::apply() {
            return false;
        }
        bool Mist::hasIllusion(Board& m_board, IllusionService& illusionService, Player& player) {
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
    
            //illusionService.placeIllusion(coord, std::move(card));
            //  }
            return true;
        }
    
    
        ////------------------------------------------ Wave -------------------------------------------
    Wave::Wave(Board& m_board, Player& red, Player& blue) :PowerCard(m_board, red, blue) {
        m_ability = PowerCardType::Wave;
    }

    void Wave::setAvailableStacks(const std::vector<Coord>& stacks) {
        m_available_stacks = stacks;
    }


    void Wave::setSelectedStack(const Coord& stack) {
        m_selected_stack = stack;
    }

    std::optional<Coord> Wave::getSelectedStack() const {
        return m_selected_stack;
    }
    void Wave::setSelectedStackID(uint16_t stack_id) {
        m_selected_stack_id = stack_id;
    }


    std::unordered_set<uint16_t> Wave::getAvailableStackIDs() const {
        std::unordered_set<uint16_t> stack_ids;
        for (const auto& [coord, stack] : m_board) {
            if (stack.size() > 1) {
                stack_ids.insert(stack.back().getID());
            }
        }
        return stack_ids;
    }
    std::vector<Coord> Wave::getAvailableStacks() const {
        return m_available_stacks;
    }

    std::optional<uint16_t> Wave::getSelectedStackID() const {
        return m_selected_stack_id;
    }

    void Wave::setColor(color::ColorType colorPlayer) {
        m_color = colorPlayer;
    }

    void Wave::setDestination(const Coord& destination) {
        this->m_destination = destination;
    }

    std::optional<Coord> Wave::getDestination() const {
        return m_destination;
    }

    void Wave::setSelectedCard(char card) {
        m_selected_card = card;
    }

    std::optional<char> Wave::getSelectedCard() const {
        return m_selected_card;
    }

    std::vector<Coord> Wave::validStacks() const {
        std::vector<Coord> coordStack;
        for (const auto& [coord, stack] : m_board) {
            if (stack.size() > 1) {
                coordStack.emplace_back(coord);
            }
        }
        return coordStack;
    }

    bool Wave::apply() {
        if (m_board.availableSpaces().empty()) {
            Logger::log(Level::WARNING, "No empty spaces to move the stack.");
            return false;
        }

        std::vector<Coord> stacks = validStacks();
        setAvailableStacks(stacks);

        if (stacks.empty()) {
            Logger::log(Level::WARNING, "No stacks on the m_board.");
            return false;
        }

        if (!getSelectedStack()) {
            Logger::log(Level::WARNING, "No stack selected.");
            return false;
        }

        Coord selected = *getSelectedStack();

        if (std::find(stacks.begin(), stacks.end(), selected) == stacks.end()) {
            Logger::log(Level::WARNING, "Invalid stack selection.");
            return false;
        }

        if (!getDestination()) {
            Logger::log(Level::WARNING, "No destination selected.");
            return false;
        }

        Coord dest = *getDestination();

        if (!m_board.availableSpaces().contains(dest)) {
            Logger::log(Level::WARNING, "Invalid destination coordinates.");
            return false;
        }

        if (m_board.isValidMoveStack(selected, dest)) {
            m_board.moveStack(selected, dest);

            if (!getSelectedCard()) {
                Logger::log(Level::WARNING, "No card selected for placement.");
                return false;
            }

            try {
                if (m_color == color::ColorType::RED) {
                    auto card = m_player_red.getCard(charToCombatCard(*getSelectedCard()));
                    m_board.appendMove(selected, std::move(card));
                }
                else {
                    auto card = m_player_blue.getCard(charToCombatCard(*getSelectedCard()));
                    m_board.appendMove(selected, std::move(card));
                }
                Logger::log(Level::INFO, "Wave power card was used successfully.");
            }
            catch (const std::runtime_error& e) {
                Logger::log(Level::ERROR, "Error placing card: {}", e.what());
            }
            return false;
        }
        else {
            return false;
            Logger::log(Level::WARNING, "Invalid stack movement.");
        }
    }
    //
    //
    //    ////------------------------------------------ Whirlpool -------------------------------------------
    Whirlpool::Whirlpool(Board& m_board, Player& red, Player& blue) :PowerCard(m_board, red, blue) {
        m_ability = PowerCardType::Whirlpool;
    }

    bool Whirlpool::apply() {
        validPairsOnRow = getPairsOnRow();
        validPairsOnColumn = getPairsOnColumn();

        if (validPairsOnRow.empty() && validPairsOnColumn.empty()) {
            Logger::log(Level::WARNING, "No valid row or column to use power card");
            return false;
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
                return false;
            }

            auto optFirstCard = m_board.getTopCard(firstCoord);
            auto optSecondCard = m_board.getTopCard(secondCoord);

            if (!optFirstCard.has_value() || !optSecondCard.has_value()) {
                Logger::log(Level::WARNING, "No card found at one or both coordinates");
                return false;
            }

            CombatCard& firstCard = optFirstCard->get();
            CombatCard& secondCard = optSecondCard->get();


            if (firstCard.getType() < secondCard.getType()) {
                m_board.appendMove({ firstCoord.first + 2, firstCoord.second }, std::move(firstCard));
                m_board.appendMove({ secondCoord.first - 2, secondCoord.second }, std::move(secondCard));
            }
            else if (firstCard.getType() == secondCard.getType()) {

                Coord equalCardCoord = getEqualCardCoord();
                if (equalCardCoord != firstCoord && equalCardCoord != secondCoord) {
                    Logger::log(Level::WARNING, "Invalid coordinates for placing card");
                    return false;
                }

                if (equalCardCoord == firstCoord) {
                    m_board.appendSpecialCard({ secondCoord.first - 2, secondCoord.second }, std::move(secondCard));
                    m_board.appendSpecialCard({ firstCoord.first + 2, firstCoord.second }, std::move(firstCard));
                }
                else {
                    m_board.appendSpecialCard({ firstCoord.first + 2, firstCoord.second }, std::move(firstCard));
                    m_board.appendSpecialCard({ secondCoord.first - 2, secondCoord.second }, std::move(secondCard));
                }
            }
            else {
                m_board.appendMove({ secondCoord.first - 2, secondCoord.second }, std::move(secondCard));
                m_board.appendMove({ firstCoord.first + 2, firstCoord.second }, std::move(firstCard));
            }

            m_board.popTopCardAt(firstCoord);
            m_board.popTopCardAt(secondCoord);
            Logger::log(Level::INFO, "Whirlpool power card was played");
        }

        if (choice == 'c') {
            Coord firstCoord = getFirstCoord();
            Coord secondCoord = getSecondCoord();


            std::pair<Coord, Coord> inputCoords{ firstCoord, secondCoord };
            if (std::find(validPairsOnRow.begin(), validPairsOnRow.end(), inputCoords) == validPairsOnRow.end()) {
                Logger::log(Level::WARNING, "Invalid coordinate pairs");
                return false;
            }

            auto optFirstCard = m_board.getTopCard(firstCoord);
            auto optSecondCard = m_board.getTopCard(secondCoord);

            if (!optFirstCard.has_value() || !optSecondCard.has_value()) {
                Logger::log(Level::WARNING, "No card found at one or both coordinates");
                return false;
            }

            CombatCard& firstCard = optFirstCard->get();
            CombatCard& secondCard = optSecondCard->get();

            if (firstCard.getType() < secondCard.getType()) {
                m_board.appendMove({ firstCoord.first, firstCoord.second + 1 }, std::move(firstCard));
                m_board.appendMove({ secondCoord.first, secondCoord.second - 1 }, std::move(secondCard));

            }
            else if (firstCard.getType() == secondCard.getType()) {

                Coord equalCardCoord = getEqualCardCoord();
                if (equalCardCoord != firstCoord && equalCardCoord != secondCoord) {
                    Logger::log(Level::WARNING, "Invalid coordinates for placing card");
                    return false;
                }

                if (equalCardCoord == firstCoord) {
                    m_board.appendSpecialCard({ secondCoord.first , secondCoord.second - 1 }, std::move(secondCard));
                    m_board.appendSpecialCard({ firstCoord.first , firstCoord.second + 1 }, std::move(firstCard));

                }
                else {
                    m_board.appendSpecialCard({ firstCoord.first , firstCoord.second + 1 }, std::move(firstCard));
                    m_board.appendSpecialCard({ secondCoord.first , secondCoord.second - 1 }, std::move(secondCard));
                }
            }
            else {
                m_board.appendMove({ secondCoord.first, secondCoord.second - 1 }, std::move(secondCard));
                m_board.appendMove({ firstCoord.first, firstCoord.second + 1 }, std::move(firstCard));
            }

            m_board.popTopCardAt(firstCoord);
            m_board.popTopCardAt(secondCoord);
            Logger::log(Level::INFO, "Whirlpool power card was played");
            return false;
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


    std::vector<std::pair<Coord, Coord>> Whirlpool::getPairsOnRow() {
        std::vector<std::pair<Coord, Coord>> valid_pairs_on_row;
        for (const auto& [coord, stack] : m_board) {

            Coord neighbor1{ coord.first + 2, coord.second };
            Coord neighbor2{ coord.first + 4, coord.second };
            bool valid_option = (stack.size() == 1 && !m_board.hasStack(neighbor1) && m_board.hasStack(neighbor2)
                && m_board[neighbor2].size() == 1) ? true : false;
            if (valid_option) {
                valid_pairs_on_row.emplace_back(std::make_pair(coord, neighbor2));
            }
        }
        return valid_pairs_on_row;
    }
    std::vector<std::pair<Coord, Coord>> Whirlpool::getPairsOnColumn() {
        std::vector<std::pair<Coord, Coord>> valid_pairs_on_column;
        for (const auto& [coord, stack] : m_board) {

            Coord neighbor1{ coord.first, coord.second + 1 };
            Coord neighbor2{ coord.first, coord.second + 2 };

            bool valid_option = (stack.size() == 1 && !m_board.hasStack(neighbor1) && m_board.hasStack(neighbor2)
                && m_board[neighbor2].size() == 1) ? true : false;

            if (valid_option) {
                valid_pairs_on_column.emplace_back(std::make_pair(coord, neighbor2));
            }
        }
        return valid_pairs_on_column;
    }
    //
    //
    //    ////------------------------------------------ Blizzard -------------------------------------------
    Blizzard::Blizzard(Board& m_board, Player& red, Player& blue) :PowerCard(m_board, red, blue) {
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

    bool Blizzard::apply() {
        if (m_board.availableSpaces().empty()) {
            Logger::log(Level::WARNING, "No available spaces for opponent's next turn");
            return false;
        }

        if (!getBlockChoice()) {
            Logger::log(Level::WARNING, "No block choice set (Row or Column)");
            return false;
        }

        char choice = toupper(*getBlockChoice());

        if (choice == 'R') {
            std::unordered_set<uint16_t> rows;
            for (const auto& [coord, stack] : m_board.getCombatCards()) {
                rows.insert(coord.second);
            }
            for (const auto& y : m_board.availableSpaces()) {
                rows.insert(y.second);
            }

            if (!getChosenRow()) {
                Logger::log(Level::WARNING, "No row selected");
                return false;
            }

            uint16_t chosen_row = *getChosenRow();

            if (rows.find(chosen_row) == rows.end()) {
                Logger::log(Level::WARNING, "Invalid row selection");
                return false;
            }

            m_board.blockRow(chosen_row, m_color);
            Logger::log(Level::INFO, "Row {} has been blocked for the opponent's next turn", chosen_row);
        }
        else if (choice == 'C') {
            std::unordered_set<uint16_t> columns;
            for (const auto& [coord, stack] : m_board.getCombatCards()) {
                columns.insert(coord.first);
            }
            for (const auto& x : m_board.availableSpaces()) {
                columns.insert(x.first);
            }

            if (!getChosenColumn()) {
                Logger::log(Level::WARNING, "No column selected");
                return false;
            }

            uint16_t chosen_column = *getChosenColumn();

            if (columns.find(chosen_column) == columns.end()) {
                Logger::log(Level::WARNING, "Invalid column selection");
                return false;
            }

            m_board.blockColumn(chosen_column, m_color);
            Logger::log(Level::INFO, "Column {} has been blocked for the opponent's next turn", chosen_column);
            return false;
        }
        else {
            Logger::log(Level::WARNING, "Invalid choice. Please enter R for Row or C for Column");
            return false;
        }
    }
    void Blizzard::setColor(color::ColorType colorPlayer) {
        m_color = colorPlayer;

    }
    //    ////------------------------------------------ Waterfall -------------------------------------------
    Waterfall::Waterfall(Board& m_board, Player& red, Player& blue) :PowerCard(m_board, red, blue) {
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

    std::unordered_map<Orientation, std::vector<int16_t>> Waterfall::getOptions() {
        int16_t size = static_cast<int16_t>(m_board.getSize() - 1);
        std::unordered_map<Orientation, std::vector<int16_t>> choices;
        std::unordered_set<int16_t> processedRows;
        std::unordered_set<int16_t> processedCols;

        for (const auto& [coord, _] : m_board) {
            if (processedRows.find(coord.second) == processedRows.end()) {
                auto countRowElements = m_board.getCoordsOnRow(coord.second);
                if (countRowElements.size() >= size) {
                    choices[Orientation::Row].emplace_back(coord.second);
                    processedRows.insert(coord.second);
                }
            }

            if (processedCols.find(coord.first) == processedCols.end()) {
                auto countColElements = m_board.getCoordsOnColumn(coord.first);
                if (countColElements.size() >= size) {
                    choices[Orientation::Column].emplace_back(coord.first);
                    processedCols.insert(coord.first);
                }
            }
        }
        return choices;
    }

    std::tuple<Orientation, int16_t, MoveDirection> Waterfall::input() {
        auto options = getOptions();

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

    bool Waterfall::apply() {
        auto choice = input();
        auto orient = std::get<0>(choice);
        auto index = std::get<1>(choice);
        auto dir = std::get<2>(choice);

        if (orient == Orientation::Row) {
            auto vect = m_board.getCoordsOnRow(index);
            if (dir == MoveDirection::Right) {
                auto destination = m_board.getRightMostOnRow(index);
                m_board.mergeStacks(vect, destination);
            }
            else {
                auto destination = m_board.getLeftMostOnRow(index);
                m_board.mergeStacks(vect, destination);
            }
        }
        else {
            auto vect = m_board.getCoordsOnColumn(index);
            if (dir == MoveDirection::Up) {
                auto destination = m_board.getTopMostOnColumn(index);
                m_board.mergeStacks(vect, destination);
            }
            else {
                auto destination = m_board.getBottomMostOnColumn(index);
                m_board.mergeStacks(vect, destination);
            }
        }

        return false;
    }

    //
    //    ////------------------------------------------ Support -------------------------------------------
    Support::Support(Board& m_board, Player& red, Player& blue) :PowerCard(m_board, red, blue) {
        m_ability = PowerCardType::Support;
    }

    void Support::setSelectedCoord(const Coord& coord) {
        selectedCoord = coord;
    }

    std::optional<Coord> Support::getSelectedCoord() const {
        return selectedCoord;
    }

    bool Support::apply() {
        auto valid_cards = getValidCardIDs();
        if (valid_cards.empty()) {
            Logger::log(Level::WARNING, "No valid cards to apply the power card");
            return false;
        }

        if (!m_selected_card_id) {
            Logger::log(Level::WARNING, "No card selected");
            return false;
        }

        if (valid_cards.find(*m_selected_card_id) == valid_cards.end()) {
            Logger::log(Level::WARNING, "No valid card with that ID");
            return false;
        }

        for (const auto& [coord, stack] : m_board) {
            if (!stack.empty() && stack.back().getID() == *m_selected_card_id) {
                m_board.incrementCardValue(coord);
                Logger::log(Level::INFO, "Support power used!");
                return false;
            }
        }
        return false;
    }

    std::vector<Coord> Support::CoordCardType() const {
        std::vector<Coord> coordCards;
        for (const auto& [coord, stack] : m_board) {
            if (stack.empty()) continue;
            const CombatCard& card = stack.back();
            if (card.getColor() == m_color &&
                (card.getType() == CombatCardType::TWO ||
                    card.getType() == CombatCardType::THREE ||
                    card.getType() == CombatCardType::ONE)) {
                coordCards.emplace_back(coord);
            }
        }
        return coordCards;
    }

    void Support::setColor(color::ColorType colorPlayer) {
        m_color = colorPlayer;
    }

    std::unordered_set<uint16_t> Support::getValidCardIDs() const {
        std::unordered_set<uint16_t> valid_cards;
        for (const auto& [coord, stack] : m_board) {
            if (stack.empty()) continue;
            const CombatCard& card = stack.back();
            if (card.getColor() == m_color &&
                (card.getType() == CombatCardType::TWO ||
                    card.getType() == CombatCardType::THREE ||
                    card.getType() == CombatCardType::ONE)) {
                valid_cards.insert(card.getID());
            }
        }
        return valid_cards;
    }

    std::optional<uint16_t> Support::getSelectedCardID() const {
        return m_selected_card_id;
    }

    void Support::setSelectedCardID(uint16_t card_id) {
        m_selected_card_id = card_id;
    }


    //    ////------------------------------------------ Earthquake -------------------------------------------
    Earthquake::Earthquake(Board& m_board, Player& red, Player& blue) :PowerCard(m_board, red, blue) {
        m_ability = PowerCardType::Earthquake;
    }

    std::unordered_set<uint16_t> Earthquake::getCardsToRemoveIDs() const {
        std::unordered_set<uint16_t> cards_ids;
        for (const auto& [coord, stack] : m_board) {
            if (stack.empty()) continue;
            const CombatCard& topCard = stack.back();
            if (topCard.getType() == CombatCardType::ONE) {
                cards_ids.insert(topCard.getID());
            }
        }
        return cards_ids;
    }

    void Earthquake::setCardsToRemove(const std::vector<Coord>& cards) {
        cardsToRemove = cards;
    }

    std::vector<Coord> Earthquake::getCardsToRemove() const {
        return cardsToRemove;
    }

    bool Earthquake::apply() {
        auto cards_to_remove = getCardsToRemoveIDs();
        if (cards_to_remove.empty()) {
            Logger::log(Level::WARNING, "No visible cards with value ONE found on the board");
            return false;
        }

        m_cards_to_remove = cards_to_remove;

        for (const auto& card_id : m_cards_to_remove) {
          
            for (const auto& [coord, stack] : m_board) {
                if (!stack.empty() && stack.back().getID() == card_id) {
                    try {
                        m_board.removeTopCardAt(coord);
                        Logger::log(Level::INFO, "Removed card with value ONE");
                    }
                    catch (const std::runtime_error& err) {
                        Logger::log(Level::WARNING,
                            "Cannot remove card because it would break adjacency rule");
                        continue;
                    }
                    break;
                }
            }
        }
        return false;
    }

    //
    //    ////------------------------------------------ Crumble -------------------------------------------
    Crumble::Crumble(Board& m_board, Player& red, Player& blue) :PowerCard(m_board, red, blue) {
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
    std::unordered_set<uint16_t> Crumble::getValidCardIDs() const {
        std::unordered_set<uint16_t> valid_ids;
        for (const auto& [coord, stack] : m_board) {
            if (stack.empty()) continue;
            const CombatCard& card = stack.back();
            if (!m_board.isDecrementedCard(coord) &&
                card.getColor() != m_color &&
                (card.getType() == CombatCardType::TWO ||
                    card.getType() == CombatCardType::THREE ||
                    card.getType() == CombatCardType::FOUR)) {
                valid_ids.insert(card.getID());
            }
        }
        return valid_ids;
    }
    std::optional<uint16_t> Crumble::getSelectedCardID() const {
        return m_selected_card_id;
    }
    void Crumble::setColor(color::ColorType colorPlayer) {
        m_color = colorPlayer;
    }
    void Crumble::setSelectedCardID(uint16_t card_id) {
        m_selected_card_id = card_id;
    }
    std::vector<Coord> Crumble::findValidCards() const {
        std::vector<Coord> foundCards;
        for (const auto& [coord, stack] : m_board) {
            if (stack.empty()) continue;

            const CombatCard& card = stack.back();
            if (!m_board.isDecrementedCard(coord) &&
                card.getColor() != m_color &&
                (card.getType() == CombatCardType::TWO ||
                    card.getType() == CombatCardType::THREE ||
                    card.getType() == CombatCardType::FOUR)) {
                foundCards.emplace_back(coord);
            }
        }
        return foundCards;
    }
    bool Crumble::apply() {
        auto valid_cards = getValidCardIDs();
        if (valid_cards.empty()) {
            Logger::log(Level::WARNING, "No valid cards to apply the power card");
            return false;
        }

        if (!m_selected_card_id) {
            Logger::log(Level::WARNING, "No card selected to decrease");
            return false;
        }

        if (valid_cards.find(*m_selected_card_id) == valid_cards.end()) {
            Logger::log(Level::WARNING, "No valid card with that ID");
            return false;
        }

        for (const auto& [coord, stack] : m_board) {
            if (!stack.empty() && stack.back().getID() == *m_selected_card_id) {
                m_board.decrementCard(coord);
                Logger::log(Level::INFO, "Successfully decremented card");
                return false;
            }
        }
        return false;
    }

    //
    //    ////------------------------------------------ Border -------------------------------------------
    //
    //
    Border::Border(Board& m_board, Player& red, Player& blue) :PowerCard(m_board, red, blue) {
        m_ability = PowerCardType::Border;
    }

    void Border::setValidPositions(const std::set<int>& rows, const std::set<int>& cols) {
        validRows = rows;
        validCols = cols;
    }

    std::pair<std::set<int>, std::set<int>> Border::getValidPositions() const {
        return { validRows, validCols };
    }

    std::optional<uint16_t> Border::getSecondCardID() const {
        return second_card_id;
    }

    void Border::setSelectedCoord(const Coord& coord) {
        selectedCoord = coord;
    }

    std::optional<Coord> Border::getSelectedCoord() const {
        return selectedCoord;
    }

    void Border::setSecondCardIDAndType(uint16_t card_id, CombatCardType type) {
        selectedCardType = type;
        second_card_id = card_id;
    }

    bool Border::applyNeutralCard() {
        auto rect_size = m_board.getBoundingRectSize();
        std::set<int> unique_cols;
        std::set<int> unique_rows;

        if (rect_size.first == 3) {
            auto unfixed_col = m_board.getUnfixedColumns();
            auto available_spaces = m_board.availableSpaces();
            for (auto y : available_spaces) {
                if (unfixed_col.find(y.first) == unfixed_col.end()) {
                    unique_cols.insert(y.first);
                }
            }
        }
        else if (rect_size.second == 3) {
            auto unfixed_rows = m_board.getUnfixedRows();
            auto available_spaces = m_board.availableSpaces();
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
            if (m_color == color::ColorType::BLUE) {
                CombatCard card(CombatCardType::BORDER, color::ColorType::BLUE);
                m_board.appendMove(coords, std::move(card));
                return true;
            }
            else if (m_color == color::ColorType::RED) {
                CombatCard card(CombatCardType::BORDER, color::ColorType::RED);
                m_board.appendMove(coords, std::move(card));
                return true;
            }
        }
        else {
            Logger::log(Level::WARNING, "Invalid coordinate choice");
            return false;
        }
    }

    bool Border::apply() {
        if (!applyNeutralCard()) {
            return false;
        }

        if (!getSelectedCoord()) {
            Logger::log(Level::WARNING, "No second card selected");
            return false;
        }

        Coord coord_input = *getSelectedCoord();

        if (!selectedCardType) {
            Logger::log(Level::WARNING, "No card type selected");
            return false;
        }

        CombatCard card(*selectedCardType, m_color);
        m_board.appendMove(coord_input, std::move(card));
        Logger::log(Level::INFO, "Border power used");
        return false;
    }

    void Border::setSelectedCardType(CombatCardType type) {
        selectedCardType = type;
    }

    std::optional<CombatCardType> Border::getSelectedCardType() const {
        return selectedCardType;
    }

    void Border::setColor(color::ColorType colorPlayer) {
        m_color = colorPlayer;
    }

    //
    //    ////------------------------------------------ Avalanche -------------------------------------------
    //
    Avalanche::Avalanche(Board& m_board, Player& red, Player& blue) :PowerCard(m_board, red, blue) {
        m_ability = PowerCardType::Avalanche;
    }

    void Avalanche::setSelectedMove(const std::pair<MoveDirection, std::pair<Coord, Coord>>& move) {
        selectedMove = move;
    }
   
    void Avalanche::setSelectedMoveByIDs(uint16_t first_id, uint16_t second_id, MoveDirection direction) {

        Coord first_coord, second_coord;
        for (const auto& [coord, stack] : m_board) {
            if (!stack.empty()) {
                if (stack.back().getID() == first_id) {
                    first_coord = coord;
                }
                if (stack.back().getID() == second_id) {
                    second_coord = coord;
                }
            }
        }
        selectedMove = std::make_pair(direction, std::make_pair(first_coord, second_coord));
    }
    std::optional<std::pair<MoveDirection, std::pair<Coord, Coord>>> Avalanche::getSelectedMove() const {
        return selectedMove;
    }

    std::vector<std::tuple<uint16_t, uint16_t, MoveDirection>> Avalanche::getAvailableMovesWithIDs() {
        std::vector<std::tuple<uint16_t, uint16_t, MoveDirection>> moves_with_ids;
        auto pairs = getPairs();
        auto moves = checkShifting(pairs);

        for (const auto& [direction, coords] : moves) {
            const auto& [first_coord, second_coord] = coords;
            if (m_board.hasStack(first_coord) && m_board.hasStack(second_coord)) {
                moves_with_ids.push_back(std::make_tuple(
                    m_board[first_coord].back().getID(),
                    m_board[second_coord].back().getID(),
                    direction
                ));
            }
        }
        return moves_with_ids;
    }
   

    std::vector<std::pair<Orientation, std::pair<Coord, Coord>>> Avalanche::getPairs() {

        std::vector<std::pair<Orientation, std::pair<Coord, Coord>>> pairs;
        const std::vector<std::pair<Coord, Orientation>> directions = {
            {{2, 0}, Orientation::Row},
            {{-2, 0}, Orientation::Row},
            {{0, 1}, Orientation::Column},
            {{0, -1}, Orientation::Column}
        };

        for (const auto& [coord, stack] : m_board) {
            for (const auto& [delta, orientation] : directions) {
                Coord neighbor = { coord.first + delta.first, coord.second + delta.second };
                if (m_board.hasStack(neighbor)) {
                    pairs.emplace_back(orientation, std::make_pair(coord, neighbor));
                }
            }
        }
        return pairs;
    }

    std::vector<std::pair<MoveDirection, std::pair<Coord, Coord>>>Avalanche::checkShifting(

        const std::vector<std::pair<Orientation, std::pair<Coord, Coord>>>& pack) {

        std::vector<std::pair<MoveDirection, std::pair<Coord, Coord>>> choices;

        const std::unordered_map<Orientation, std::vector<std::pair<Coord, MoveDirection>>> directions = {
            {Orientation::Row,    {{{2, 0}, MoveDirection::Right}, {{-2, 0}, MoveDirection::Left}}},
            {Orientation::Column, {{{0, 1}, MoveDirection::Up},    {{0, -1}, MoveDirection::Down}}}
        };

        for (const auto& [type, pair] : pack) {
            for (const auto& [delta, shift] : directions.at(type)) {
                Coord newCoord = { pair.first.first + delta.first, pair.first.second + delta.second };
                if (!m_board.hasStack(newCoord)) {
                    choices.emplace_back(shift, std::make_pair(pair.first, pair.second));
                }
            }
        }

        return choices;
    }

    bool Avalanche::apply() {

        auto available_moves = getAvailableMovesWithIDs();
        if (available_moves.empty()) {
            Logger::log(Level::WARNING, "No valid moves available");
            return false;
        }

        if (!selectedMove) {
            Logger::log(Level::WARNING, "No move selected");
            return false;
        }
        Coord first_coord, second_coord;
        const auto& [direction, coords] = *selectedMove;
        first_coord = coords.first;
        second_coord = coords.second;

        const std::unordered_map<MoveDirection, Coord> directionDeltas = {
            {MoveDirection::Down, {0, 1}},
            {MoveDirection::Up, {0, -1}},
            {MoveDirection::Right, {2, 0}},
            {MoveDirection::Left, {-2, 0}}
        };

        const auto& delta = directionDeltas.at(direction);
        std::vector<std::pair<Coord, Coord>> stacks;

        if (direction == MoveDirection::Down || direction == MoveDirection::Up) {
            Coord newCoord1 = { second_coord.first, second_coord.second + delta.second };
            Coord newCoord2 = { first_coord.first, first_coord.second + delta.second };
            stacks.push_back({ second_coord, newCoord1 });
            stacks.push_back({ first_coord, newCoord2 });
        }
        else {
            Coord newCoord1 = { first_coord.first + delta.first, first_coord.second };
            Coord newCoord2 = { second_coord.first + delta.first, second_coord.second };
            stacks.push_back({ first_coord, newCoord1 });
            stacks.push_back({ second_coord, newCoord2 });
        }

        if (m_board.isValidMoveStacks(stacks)) {
            m_board.moveStacks(stacks);
            Logger::log(Level::INFO, "Move applied");
        }
        else {
            Logger::log(Level::WARNING, "Invalid move!");
        }
        return false;
        Logger::log(Level::INFO, "Avalanche card used");
    }

    
        ////------------------------------------------ Rock -------------------------------------------
        Rock::Rock(Board& m_board, Player& red, Player& blue):PowerCard(m_board, red, blue) {  ///////////////////////////////// foloseste iluzie, nu merge 
            m_ability = PowerCardType::Rock;
        }
    
        bool Rock::apply() {
            std::vector<Coord>illusionCoord = getIllusionCoords(m_board);
            if (!illusionCoord.empty() && m_player_red.hasCards()) {
    
                std::cout << "Illusion coordinates: " << std::endl;
    
                Coord coord;
                std::cin >> coord.first >> coord.second;
    
                if (std::find(illusionCoord.begin(), illusionCoord.end(), coord) == illusionCoord.end()) {
                    Logger::log(Level::WARNING, "No illusion at these coordinates");
                }
    
                std::cout << "Choose a card to cover the illusion" << std::endl;
                char card_type;
                std::cin >> card_type;
                auto card = m_player_red.getCard(charToCombatCard(card_type));
    
                m_board.appendMove(coord, std::move(card));
            }
            Logger::log(Level::INFO, "Rock power card was used");
            return false;
        }
    
        std::vector<Coord>Rock::getIllusionCoords(const Board& m_board) const {
            std::vector<Coord>illusionCoords;
            for (const auto& [coord, stack] : m_board) {
                if (!stack.empty() && stack.back().isIllusion()) {
                    Logger::log(Level::INFO, "Illusion card at ({}, {})", coord.first, coord.second);
                    illusionCoords.emplace_back(coord);
                }
                else {
                    Logger::log(Level::WARNING, "No illusions on the m_board");
                }
            }
            return illusionCoords;
        }
    
    }