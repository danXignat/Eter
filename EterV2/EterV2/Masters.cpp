#include"Masters.h"

#include <format>

using namespace logger;

namespace base {

    /*    Config& config = Config::getInstance();
        uint16_t x_step = config.getCardSpacingX();
        uint16_t y_step = config.getCardSpacingY();*/

        ///----------------------------------------------------MasterOfFire----------------------------------------
        ///---------Burn------------

    MasterOfFireFront::MasterOfFireFront(Board& board, Player& player_red, Player& player_blue, color::ColorType color)
        : MageCard(board, player_red, player_blue, color) {
        m_type = MageType::Fire;
        m_ability = MageTypeAbility::Burn;
    }

    void MasterOfFireFront::setSelectedCoord(const Coord& coord) {
        m_selectedCoord.emplace(coord);
    }

    const std::unordered_set<uint16_t>& MasterOfFireFront::getAvailableChoices() const {
        return m_availableChoices;
    }

    std::unordered_set<uint16_t> MasterOfFireFront::getChoices() {
        m_availableChoices.clear();
        int minimumSize = 2;

        for (const auto& [coord, stack] : m_board) {
            auto& [x, y] = coord;
            if (stack.size() < minimumSize) {
                continue;
            }

            bool is_player_card = stack[stack.size() - 2].getColor() == m_color;
            bool is_covered = stack.back().getColor() != m_color;

            if (is_player_card && is_covered) {
                m_availableChoices.insert(stack.back().getID());
            }
        }

        return m_availableChoices;
    }

    bool MasterOfFireFront::apply() {
        if (m_availableChoices.empty()) {
            Logger::log(Level::INFO, "You can't use this mage right now");
            return false;
        }

        if (!m_selectedCoord) {
            Logger::log(Level::INFO, "No selection made yet");
            return false;
        }

        /*if (std::find(m_availableChoices.begin(), m_availableChoices.end(), m_selectedCoord)
           == m_availableChoices.end()) {
            Logger::log(Level::INFO, "Impossible choice");
            return false;
        }*/

        m_board.removeTopCardAt(m_selectedCoord.value());
        Logger::log(Level::INFO, "Mage fire ability remove top card used");

        m_selectedCoord.reset();
        return true;
    }


    ///--------Burn Row and Col-----------

    MasterOfFireBack::MasterOfFireBack(Board& board, Player& player_red, Player& player_blue, color::ColorType color)
        : MageCard(board, player_red, player_blue, color),
        line_choice{ LineChoice::None } {
        m_type = MageType::Fire;
        m_ability = MageTypeAbility::BurnRowOrColumn;
    }


    void MasterOfFireBack::_setRowChoices() {
        Config& config{ Config::getInstance() };
        uint16_t x_step{ config.getCardSpacingX() };
        uint16_t y_step{ config.getCardSpacingY() };

        auto [min_corner, max_corner] {m_board.getBoudingRect()};
        std::unordered_set<uint16_t> buffer1, buffer2;
        bool has_own_color1 = false, has_own_color2 = false;

        uint16_t top = 0, bottom = 0;
        for (uint16_t x = min_corner.first; x <= max_corner.first; x += x_step) {
            Coord coord1{ x, min_corner.second }, coord2{ x, max_corner.second };

            if (m_board.hasStack(coord1)) {
                buffer1.insert(m_board[coord1].back().getID());
                has_own_color1 = has_own_color1 || (m_board[coord1].back().getColor() == m_color);
                top++;
            }

            if (m_board.hasStack(coord2)) {
                buffer2.insert(m_board[coord2].back().getID());
                has_own_color2 = has_own_color2 || (m_board[coord2].back().getColor() == m_color);
                bottom++;
            }
        }

        if (top >= 3 && has_own_color1) {
            m_availableRows[min_corner.second] = buffer1;
        }
        if (bottom >= 3 && has_own_color2) {
            m_availableRows[max_corner.second] = buffer2;
        }
    }
    void MasterOfFireBack::_setColChoices() {
        Config& config{ Config::getInstance() };
        uint16_t x_step{ config.getCardSpacingX() };
        uint16_t y_step{ config.getCardSpacingY() };

        auto [min_corner, max_corner] {m_board.getBoudingRect()};
        std::unordered_set<uint16_t> buffer1, buffer2;
        bool has_own_color1 = false, has_own_color2 = false;

        uint16_t left = 0, right = 0;
        for (uint16_t y = min_corner.second; y <= max_corner.second; y += y_step) {
            Coord coord1{ min_corner.first, y }, coord2{ max_corner.first, y };

            if (m_board.hasStack(coord1)) {
                buffer1.insert(m_board[coord1].back().getID());
                has_own_color1 = has_own_color1 || (m_board[coord1].back().getColor() == m_color && !has_own_color1);
                left++;
            }

            if (m_board.hasStack(coord2)) {
                buffer2.insert(m_board[coord2].back().getID());
                has_own_color2 = has_own_color2 || (m_board[coord2].back().getColor() == m_color);
                right++;
            }
        }

        if (left >= 3 && has_own_color1) {
            m_availableColumns[min_corner.first] = buffer1;
        }
        if (right >= 3 && has_own_color2) {
            m_availableColumns[max_corner.first] = buffer2;
        }
    }


    std::unordered_set<uint16_t> MasterOfFireBack::getChoices() {
        _setColChoices();
        _setRowChoices();

        std::unordered_set<uint16_t> choices;

        for (auto& [pos, ids] : m_availableRows) {
            choices.insert(ids.begin(), ids.end());
        }
        for (auto& [pos, ids] : m_availableColumns) {
            choices.insert(ids.begin(), ids.end());
        }

        return choices;
    }

    std::unordered_set<uint16_t> MasterOfFireBack::getRemoveChoices(const Coord& coord) {
        if (m_availableRows.contains(coord.second)) {
            return m_availableRows[coord.second];
        }

        if (m_availableColumns.contains(coord.first)) {
            return m_availableColumns[coord.first];
        }
    }

    void MasterOfFireBack::setSelectedChoice(const Coord& coord) {
        if (m_availableRows.contains(coord.second)) {
            m_selectedChoice = coord.second;

            line_choice = LineChoice::Row;
            m_hasUserSelected = true;
            return;
        }

        if (m_availableColumns.contains(coord.first)) {
            m_selectedChoice = coord.first;

            line_choice = LineChoice::Column;
            m_hasUserSelected = true;
        }
    }

    bool MasterOfFireBack::apply() {

        if (m_availableRows.empty() && m_availableColumns.empty())
        {
            Logger::log(Level::INFO, "You can't use this mage right now");
            return false;
        }

        if (!m_hasUserSelected) {
            Logger::log(Level::INFO, "No selection made yet");
            return false;
        }

        if (line_choice == LineChoice::Row) {
            m_board.removeRow(m_selectedChoice);
        }
        else if (line_choice == LineChoice::Column) {
            m_board.removeColumn(m_selectedChoice);
        }
        else {
            Logger::log(Level::INFO, "invalid input");
            return false;
        }

        Logger::log(Level::INFO, "Mage fire ability remove top card used");
        return true;
    }

    ///------------------------------------------ MasterOfEarth-------------------------------------------
    ///-------------Bury-------------
    MasterOfEarthFront::MasterOfEarthFront(Board& board, Player& player_red, Player& player_blue, color::ColorType color)
        : MageCard(board, player_red, player_blue, color) {
        m_type = MageType::Earth;
        m_ability = MageTypeAbility::Bury;
        m_hasUserSelected = false;
        m_hasCardTypeSelected = false;
    }

    bool MasterOfEarthFront::validPosition(const Coord& coord, CombatCardType other_type) const {
        using enum CombatCardType;

        const auto& top_card{ m_board[coord].back() };
        std::unordered_set available_types{ ONE, TWO, THREE, FOUR };

        bool correct_type{ available_types.contains(top_card.getType()) };
        bool correct_val{ top_card.getType() > other_type };
        bool correct_color{ top_card.getColor() != m_color };

        return correct_color && correct_type && correct_val;
    }

    std::unordered_set<uint16_t> MasterOfEarthFront::getChoices(CombatCardType type) {
        m_availableChoices.clear();

        for (const auto& [coord, stack] : m_board) {
            if (validPosition(coord, type)) {
                m_availableChoices.insert(stack.back().getID());
            }
        }

        return m_availableChoices;
    }

    void MasterOfEarthFront::setSelectedCoord(const Coord& coord) {
        m_selectedCoord = coord;
        m_hasUserSelected = true;
    }

    void MasterOfEarthFront::setSelectedCard(uint16_t id) {
        m_selectedId = id;
        m_hasCardTypeSelected = true;
    }

    bool MasterOfEarthFront::apply() {
        if (m_board.hasStack(m_selectedCoord) == false) {
            return false;
        }

        if (m_availableChoices.empty()) {
            Logger::log(Level::INFO, "No valid position for using this Mage!");
            return false;
        }

        if (!m_hasCardTypeSelected) {
            Logger::log(Level::WARNING, "No card type selected!");
            return false;
        }

        if (m_availableChoices.contains(m_board[m_selectedCoord].back().getID()) == false) {
            return false;
        }

        if (m_color == color::ColorType::RED) {
            CombatCard card = m_player_red.getCardByID(m_selectedId);

            m_board.appendSpecialCard(m_selectedCoord, std::move(card));
        }
        else {
            CombatCard card = m_player_blue.getCardByID(m_selectedId);

            m_board.appendSpecialCard(m_selectedCoord, std::move(card));
        }

        Logger::log(Level::INFO, "Mage Earth Bury ability card used");
        m_hasUserSelected = false;
        return true;
    }
    ///---------Hole----------

    MasterOfEarthBack::MasterOfEarthBack(Board& board, Player& player_red, Player& player_blue, color::ColorType color)
        : MageCard(board, player_red, player_blue, color) {
        m_type = MageType::Earth;
        m_ability = MageTypeAbility::Hole;
        m_hasUserSelected = false;

    }
    void MasterOfEarthBack::setSelectedCoord(const Coord& coord) {
        m_selectedCoord = coord;
        m_hasUserSelected = true;
    }

    const std::vector<Coord>& MasterOfEarthBack::getAvailableChoices() const {
        m_availableChoices.assign(m_board.availableSpaces().begin(), m_board.availableSpaces().end());
        return m_availableChoices;
    }

    bool MasterOfEarthBack::apply() {
        getAvailableChoices();

        if (m_availableChoices.empty()) {
            Logger::log(Level::INFO, "No available choices for this mage rn");
            return false;
        }

        if (!m_hasUserSelected) {
            Logger::log(Level::WARNING, "No selection made yet!");
            return false;
        }

        if (std::find(m_availableChoices.begin(), m_availableChoices.end(), m_selectedCoord) == m_availableChoices.end()) {
            Logger::log(Level::INFO, "You entered an invalid option for this mage");
            return false;
        }

        CombatCard card(CombatCardType::HOLE, color::ColorType::DEFAULT);
        m_board.appendMove(m_selectedCoord, std::move(card));
        Logger::log(Level::INFO, "Mage Earth Hole ability card used");
        m_hasUserSelected = false;
        return true;
    }


    /// ---------------------------------------MasterOfAir----------------------------------------
    /// ---------BlowAway--------
    MasterOfAirFront::MasterOfAirFront(Board& board, Player& player_red, Player& player_blue, color::ColorType color)
        : MageCard(board, player_red, player_blue, color),
        m_hasUserSelectedFrom(false), m_hasUserSelectedTo(false) {
        m_type = MageType::Air;
        m_ability = MageTypeAbility::BlowAway;
    }


    bool MasterOfAirFront::apply() {
        if (m_availableChoicesFrom.empty() || m_availableChoicesTo.empty()) {
            Logger::log(Level::WARNING, "You can't use this mage right now!");
            return false;
        }

        if (!m_hasUserSelectedFrom || !m_hasUserSelectedTo) {
            Logger::log(Level::WARNING, "Move coordinates have not been fully selected!");
            return false;
        }

        if (!m_availableChoicesFrom.contains(m_board[m_moveFrom].back().getID()) ||
            !m_availableChoicesTo.contains(m_moveTo)) {
            Logger::log(Level::WARNING, "Invalid source coordinate!");
            return false;
        }

        if (m_board.isValidMoveStack(m_moveFrom, m_moveTo)) {
            m_board.moveStack(m_moveFrom, m_moveTo);
            Logger::log(Level::INFO, "Mage Air BlowAway ability card used");
            m_hasUserSelectedFrom = false;
            m_hasUserSelectedTo = false;
            return true;
        }
        else {
            Logger::log(Level::INFO, "Invalid destination coordinate!");
            return false;
        }
    }

    void MasterOfAirFront::setMoveFrom(const Coord& coord) {
        m_moveFrom = coord;
        m_hasUserSelectedFrom = true;
        m_stackBufer.clear();
        m_stackBufer = getIDSAt(coord);
    }

    void MasterOfAirFront::setMoveTo(const Coord& coord) {
        m_moveTo = coord;
        m_hasUserSelectedTo = true;
    }


    const std::unordered_set<uint16_t>& MasterOfAirFront::getAvailableChoicesFrom() {
        m_availableChoicesFrom.clear();
        for (const auto& [coord, stack] : m_board) {
            if (stack.back().getColor() == m_color) {
                m_availableChoicesFrom.insert(stack.back().getID());
            }
        }
        return m_availableChoicesFrom;
    }

    const std::unordered_set<Coord, utils::CoordFunctor>& MasterOfAirFront::getAvailableChoicesTo() {
        if (m_hasUserSelectedFrom == false) {
            throw std::runtime_error("you  didn t select  from");
        }
        m_availableChoicesTo.clear();

        for (const Coord& space : m_board.availableSpaces()) {
            if (m_board.isValidMoveStack(m_moveFrom, space)) {
                m_availableChoicesTo.insert(space);
            }
        }

        return m_availableChoicesTo;
    }

    std::unordered_set<uint16_t> MasterOfAirFront::getIDSAt(const Coord& coord) {
        std::unordered_set<uint16_t> res;

        if (m_board.hasStack(coord)) {
            for (const auto& card : m_board[coord]) {
                res.insert(card.getID());
            }
        }

        return res;
    }
    /// ---------BlowEter---------

    MasterOfAirBack::MasterOfAirBack(Board& board, Player& player_red, Player& player_blue, color::ColorType color)
        : MageCard(board, player_red, player_blue, color) {
        m_type = MageType::Air;
        m_ability = MageTypeAbility::BlowEter;
        m_hasUserSelected = false;
    }
    void MasterOfAirBack::setSelectedCoord(const Coord& coord) {
        m_selectedCoord = coord;
        m_hasUserSelected = true;
    }
    const std::vector<Coord>& MasterOfAirBack::getAvailableChoices(const Board& board) const {
        m_availableChoices.assign(board.availableSpaces().begin(), board.availableSpaces().end());
        return m_availableChoices;
    }
    bool MasterOfAirBack::apply() {
        getAvailableChoices(m_board);
        if (m_availableChoices.empty()) {
            Logger::log(Level::INFO, "No available options for this mage rn");
            return false;
        }

        if (m_availableChoices.empty()) {
            Logger::log(Level::INFO, "No available options for this mage rn");
            return false;
        }

        if (!m_hasUserSelected) {
            Logger::log(Level::WARNING, "No selection made yet!");
            return false;
        }

        if (std::find(m_availableChoices.begin(), m_availableChoices.end(), m_selectedCoord) == m_availableChoices.end()) {
            Logger::log(Level::INFO, "Invalid option");
            return false;
        }

        if (!eter) {
            return false;
        }

        m_board.appendMove(m_selectedCoord, std::move(eter.value()));
        eter.reset();
        Logger::log(Level::INFO, "Mage Air BlowEter ability card used");

        m_hasUserSelected = false; // reset for next use
        return true;
    }

    uint16_t MasterOfAirBack::createEter() {
        eter.emplace(CombatCardType::ETER, m_color);

        if (m_color == color::ColorType::RED) {
            m_player_red.addCard(std::move(eter.value()));
        }
        else {
            m_player_blue.addCard(std::move(eter.value()));
        }

        return eter->getID();
    }


    //----------------------------------------- MasterOfWater---------------------------------------

    //---------Boat------------
    MasterOfWaterFront::MasterOfWaterFront(Board& board, Player& player_red, Player& player_blue, color::ColorType color)
        : MageCard(board, player_red, player_blue, color) {
        m_type = MageType::Water;
        m_ability = MageTypeAbility::Boat;

    }

    bool MasterOfWaterFront::apply() {
        if (m_availableChoicesFrom.empty() || m_availableChoicesTo.empty()) {
            Logger::log(Level::WARNING, "You can't use this mage right now!");
            return false;
        }

        if (!m_hasUserSelectedFrom || !m_hasUserSelectedTo) {
            Logger::log(Level::WARNING, "Move coordinates have not been fully selected!");
            return false;
        }

        if (!m_availableChoicesFrom.contains(m_board[m_moveFrom].back().getID()) ||
            !m_availableChoicesTo.contains(m_moveTo)) {
            Logger::log(Level::WARNING, "Invalid source coordinate!");
            return false;
        }

        if (m_board.isValidMoveStack(m_moveFrom, m_moveTo)) {
            m_board.moveStack(m_moveFrom, m_moveTo);
            Logger::log(Level::INFO, "Mage Air BlowAway ability card used");
            m_hasUserSelectedFrom = false;
            m_hasUserSelectedTo = false;
            return true;
        }
        else {
            Logger::log(Level::INFO, "Invalid destination coordinate!");
            return false;
        }
    }

    void MasterOfWaterFront::setMoveFrom(const Coord& coord) {
        m_moveFrom = coord;
        m_hasUserSelectedFrom = true;
        m_stackBufer.clear();
        m_stackBufer = getIDSAt(coord);
    }

    void MasterOfWaterFront::setMoveTo(const Coord& coord) {
        m_moveTo = coord;
        m_hasUserSelectedTo = true;
    }


    const std::unordered_set<uint16_t>& MasterOfWaterFront::getAvailableChoicesFrom() {
        m_availableChoicesFrom.clear();
        for (const auto& [coord, stack] : m_board) {
            if (stack.back().getColor() != m_color) {
                m_availableChoicesFrom.insert(stack.back().getID());
            }
        }
        return m_availableChoicesFrom;
    }

    const std::unordered_set<Coord, utils::CoordFunctor>& MasterOfWaterFront::getAvailableChoicesTo() {
        if (m_hasUserSelectedFrom == false) {
            throw std::runtime_error("you  didn t select  from");
        }
        m_availableChoicesTo.clear();

        for (const Coord& space : m_board.availableSpaces()) {
            if (m_board.isValidMoveStack(m_moveFrom, space)) {
                m_availableChoicesTo.insert(space);
            }
        }

        return m_availableChoicesTo;
    }

    std::unordered_set<uint16_t> MasterOfWaterFront::getIDSAt(const Coord& coord) {
        std::unordered_set<uint16_t> res;

        if (m_board.hasStack(coord)) {
            for (const auto& card : m_board[coord]) {
                res.insert(card.getID());
            }
        }

        return res;
    }

    /// -----------BoatRowOrColumn--------------

    MasterOfWaterBack::MasterOfWaterBack(Board& board, Player& player_red, Player& player_blue, color::ColorType color)
        : MageCard(board, player_red, player_blue, color) {
        m_type = MageType::Water;
        m_ability = MageTypeAbility::BoatRowOrColumn;
        line_choice = LineChoice::None;
    }


    std::optional<std::unordered_map<BorderType, std::vector<Coord>>> MasterOfWaterBack::getBorders(Board& board, Player& player) const {
        Config& config = Config::getInstance();
        uint16_t x_step = config.getCardSpacingX();
        uint16_t y_step = config.getCardSpacingY();

        std::unordered_map<BorderType, std::vector<Coord>> borders;

        auto [corner1, corner2] = board.getBoudingRect();
        auto [width, height] = board.getBoundingRectSize();

        std::vector<Coord> top_border, bottom_border, left_border, right_border;


        for (uint16_t x = corner1.first; x <= corner2.first; x += x_step) {
            if (board.hasStack({ x, corner1.second })) {
                top_border.push_back({ x, corner1.second });
            }
            if (height > 1 && board.hasStack({ x, corner2.second })) {
                bottom_border.push_back({ x, corner2.second });
            }
        }
        for (uint16_t y = corner1.second; y <= corner2.second; y += y_step) {
            if (board.hasStack({ corner1.first, y })) {
                left_border.push_back({ corner1.first, y });
            }
            if (width > 1 && board.hasStack({ corner2.first, y })) {
                right_border.push_back({ corner2.first, y });
            }
        }
        auto addBorder = [&borders](std::vector<Coord>& border, BorderType type, uint16_t size) {
            if (border.size() >= size) {
                borders[type] = std::move(border);
            }
            };
        uint16_t size = board.getSize() - 1;
        addBorder(top_border, BorderType::Top, size);
        addBorder(bottom_border, BorderType::Bottom, size);
        addBorder(left_border, BorderType::Left, size);
        addBorder(right_border, BorderType::Right, size);

        return borders.empty() ? std::nullopt : std::make_optional(borders);
    }

    bool MasterOfWaterBack::apply() {
        if (getAvailableSpaces().contains(m_end) == false) {
            return false;
        }

        if (!m_setStart || !m_setEnd) {
            return false;
        }

        if (moving_cards_buffer.empty()) {
            return false;
        }

        if (line_choice == LineChoice::None) {
            return false;
        }

        if (line_choice == LineChoice::Row) {
            int16_t end_rank{ m_end.second };

            m_board.moveRow(moving_line, end_rank);
        }
        else {
            int16_t end_rank{ m_end.first };

            m_board.moveColumn(moving_line, end_rank);
        }

        return true;
    }

    /*static std::string_view borderToString(BorderType border) {

   switch (border) {
   case BorderType::Top:
       return "Top";

   case BorderType::Bottom:
       return "Bottom";

   case BorderType::Left:
       return "Left";

   case BorderType::Right:
       return "Right";

   default:
       return "Unknown";
   }

   }*/


    void MasterOfWaterBack::_setRowChoices() {
        Config& config{ Config::getInstance() };
        uint16_t x_step{ config.getCardSpacingX() };
        uint16_t y_step{ config.getCardSpacingY() };

        auto [min_corner, max_corner] {m_board.getBoudingRect()};
        std::unordered_set<uint16_t> buffer1, buffer2;

        uint16_t top = 0, bottom = 0;
        for (uint16_t x = min_corner.first; x <= max_corner.first; x += x_step) {
            Coord coord1{ x, min_corner.second }, coord2{ x, max_corner.second };

            if (m_board.hasStack(coord1)) {
                buffer1.insert(m_board[coord1].back().getID());
                top++;
            }

            if (m_board.hasStack(coord2)) {
                buffer2.insert(m_board[coord2].back().getID());
                bottom++;
            }
        }

        if (top >= 3 && buffer1.size()) {
            m_availableRows[min_corner.second] = buffer1;
        }
        if (bottom >= 3 && buffer2.size()) {
            m_availableRows[max_corner.second] = buffer2;
        }
    }
    void MasterOfWaterBack::_setColChoices() {
        Config& config{ Config::getInstance() };
        uint16_t x_step{ config.getCardSpacingX() };
        uint16_t y_step{ config.getCardSpacingY() };

        auto [min_corner, max_corner] {m_board.getBoudingRect()};
        std::unordered_set<uint16_t> buffer1, buffer2;

        uint16_t left = 0, right = 0;
        for (uint16_t y = min_corner.second; y <= max_corner.second; y += y_step) {
            Coord coord1{ min_corner.first, y }, coord2{ max_corner.first, y };

            if (m_board.hasStack(coord1)) {
                buffer1.insert(m_board[coord1].back().getID());
                left++;
            }

            if (m_board.hasStack(coord2)) {
                buffer2.insert(m_board[coord2].back().getID());
                right++;
            }
        }

        if (left >= 3 && buffer1.size()) {
            m_availableColumns[min_corner.first] = buffer1;
        }
        if (right >= 3 && buffer2.size()) {
            m_availableColumns[max_corner.first] = buffer2;
        }
    }


    std::unordered_set<uint16_t> MasterOfWaterBack::getChoices() {
        _setColChoices();
        _setRowChoices();

        std::unordered_set<uint16_t> choices;

        for (auto& [pos, ids] : m_availableRows) {
            choices.insert(ids.begin(), ids.end());
        }
        for (auto& [pos, ids] : m_availableColumns) {
            choices.insert(ids.begin(), ids.end());
        }

        return choices;
    }

    void MasterOfWaterBack::setStart(const Coord& coord) {
        if (m_availableRows.contains(coord.second)) {
            moving_line = coord.second;
            line_choice = LineChoice::Row;
            m_setStart = true;

            for (const auto& [pos, stack] : m_board) {
                if (m_availableRows[coord.second].contains(stack.back().getID())) {
                    moving_cards_buffer[pos] = std::vector<uint16_t>();

                    for (const auto& card : stack) {
                        moving_cards_buffer[pos].push_back(card.getID());
                    }
                }
            }
        }
        else if (m_availableColumns.contains(coord.first)) {
            moving_line = coord.first;
            line_choice = LineChoice::Column;
            m_setStart = true;

            for (const auto& [pos, stack] : m_board) {
                if (m_availableColumns[coord.first].contains(stack.back().getID())) {
                    moving_cards_buffer[pos] = std::vector<uint16_t>();

                    for (const auto& card : stack) {
                        moving_cards_buffer[pos].push_back(card.getID());
                    }
                }
            }
        }
    }

    void MasterOfWaterBack::setEnd(const Coord& coord) {
        m_end = coord;
        m_setEnd = true;
    }

    std::unordered_set<Coord, utils::CoordFunctor> MasterOfWaterBack::getAvailableSpaces() {
        Config& config{ Config::getInstance() };
        uint16_t x_step{ config.getCardSpacingX() };
        uint16_t y_step{ config.getCardSpacingY() };

        auto [min_corner, max_corner] {m_board.getBoudingRect()};

        std::unordered_set<Coord, utils::CoordFunctor> available_spaces;

        if (line_choice == LineChoice::Row) {
            uint16_t available_y;

            if (moving_line == min_corner.second) {
                available_y = max_corner.second + y_step;
            }
            else {
                available_y = min_corner.second - y_step;
            }

            for (const auto& [coord, _] : moving_cards_buffer) {
                available_spaces.emplace(coord.first, available_y);
            }
        }
        else if (line_choice == LineChoice::Column) {
            uint16_t available_x;

            if (moving_line == min_corner.first) {
                available_x = max_corner.first + x_step;
            }
            else {
                available_x = min_corner.first - x_step;
            }

            for (const auto& [coord, _] : moving_cards_buffer) {
                available_spaces.emplace(available_x, coord.second);
            }
        }

        return available_spaces;
    }

    /*void MasterOfWaterBack::setSelectedBorder(uint16_t index) {
        m_selectedFrom = index;
        m_hasSelectedBorder = true;
    }

    void MasterOfWaterBack::setSelectedDestination(int destination) {
        m_selectedTo = destination;
        m_hasSelectedDestination = true;
    }*/
    
    void MasterOfWaterBack::resetBuffer() {
        moving_cards_buffer.clear();
        line_choice = LineChoice::None;
        moving_line = 0;
        m_setStart = false;
        m_setEnd = false;
    }
}