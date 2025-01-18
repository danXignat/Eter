#include"Masters.h"

#include <format>

using namespace logger;

namespace base {



    ///----------------------------------------------------MasterOfFire----------------------------------------
    ///---------Burn------------

    MasterOfFireFront::MasterOfFireFront() {
        m_type = MageType::Fire;
        m_ability = MageTypeAbility::Burn;
        m_hasUserSelected = false;
    }

    void MasterOfFireFront::setSelectedCoord(const Coord& coord) {
        m_selectedCoord = coord;
        m_hasUserSelected = true;
    }

    const std::vector<Coord>& MasterOfFireFront::getAvailableChoices() const {
        return m_availableChoices;
    }

    std::vector<Coord> MasterOfFireFront::getChoices(const Board& board, const Player& player) {
        m_availableChoices.clear();
        int minimumSize = 2;

        for (const auto& [coord, stack] : board) {
            auto& [x, y] = coord;
            if (stack.size() < minimumSize) {
                continue;
            }

            bool is_player_card = stack[stack.size() - 2].getColor() == player.getColor();
            bool is_covered = stack.back().getColor() != player.getColor();

            if (is_player_card && is_covered) {
                m_availableChoices.emplace_back(x, y);
            }
        }

        return m_availableChoices;
    }

    bool MasterOfFireFront::apply(Board& board, Player& player) {
        getChoices(board, player);
        if (m_availableChoices.empty()) {
            Logger::log(Level::INFO, "You can't use this mage right now");
            return false;
        }

        if (!m_hasUserSelected) {
            Logger::log(Level::INFO, "No selection made yet");
            return false;
        }

        if (std::find(m_availableChoices.begin(), m_availableChoices.end(), m_selectedCoord)
            == m_availableChoices.end()) {
            Logger::log(Level::INFO, "Impossible choice");
            return false;
        }

        board.removeTopCardAt(m_selectedCoord);
        Logger::log(Level::INFO, "Mage fire ability remove top card used");

        m_hasUserSelected = false;
        return true;
    }


    ///--------Burn Row and Col-----------


    MasterOfFireBack::MasterOfFireBack() {
        m_type = MageType::Fire;
        m_ability = MageTypeAbility::BurnRowOrColumn;
    }
    const std::vector<uint16_t>& MasterOfFireBack::getAvailableRows() const {
        return m_availableRows;
    }

    const std::vector<uint16_t>& MasterOfFireBack::getAvailableColumns() const {
        return m_availableColumns;
    }
    bool MasterOfFireBack::hasUserSelected() const {
        return m_hasUserSelected;
    }
    void MasterOfFireBack::setSelectedChoice(uint16_t choice, char type) {
        m_selectedChoice = choice;
        m_selectedType = type;
        m_hasUserSelected = true;
    }

    void MasterOfFireBack::clearSelection() {
        m_hasUserSelected = false;
    }
    std::pair<std::vector<uint16_t>, std::vector<uint16_t>> MasterOfFireBack::getChoices(const Board& board, const Player& player) {
        std::vector<uint16_t> line_choices;
        std::vector<uint16_t> column_choices;

        std::unordered_map<uint16_t, std::pair<uint16_t, bool>> rows;
        std::unordered_map<uint16_t, std::pair<uint16_t, bool>> cols;

        for (const auto& [coord, stack] : board) {
            auto& [x, y] = coord;

            rows[y].first++;
            cols[x].first++;

            if (stack.back().getColor() == player.getColor()) {
                rows[y].second = true;
                cols[x].second = true;
            }
        }

        for (const auto& [pos, val] : rows) {
            if (val.first >= board.getSize() - 1 && val.second) {  // 
                m_availableRows.push_back(pos);
            }
        }
        for (const auto& [pos, val] : cols) {
            if (val.first >= board.getSize() - 1 && val.second) {  //
                m_availableColumns.push_back(pos);
            }
        }

        return { m_availableRows, m_availableColumns };
    }


    bool MasterOfFireBack::apply(Board& board, Player& player) {
        getChoices(board, player);
        if (m_availableRows.empty() && m_availableColumns.empty())
        {
            Logger::log(Level::INFO, "You can't use this mage right now");
            return false;
        }
        std::cout << "Available choices:\n";

        if (!m_hasUserSelected) {
            Logger::log(Level::INFO, "No selection made yet");
            return false;
        }
        if (m_selectedType == 'r') {
            if (std::find(m_availableRows.begin(), m_availableRows.end(), m_selectedChoice) != m_availableRows.end()) {
                if (!board.isValidRemoveRow(m_selectedChoice)) {
                    Logger::log(Level::ERROR, "You can't remove that row");
                    return false;
                }
                board.removeRow(m_selectedChoice);
            }
            else {
                Logger::log(Level::INFO, "invalid input");
                return false;
            }
        }
        else if (m_selectedType == 'c') {
            if (std::find(m_availableColumns.begin(), m_availableColumns.end(), m_selectedChoice) != m_availableColumns.end()) {
                if (!board.isValidRemoveColumn(m_selectedChoice)) {
                    Logger::log(Level::ERROR, "You can't remove that column");
                    return false;
                }
                board.removeColumn(m_selectedChoice);
            }
            else {
                Logger::log(Level::INFO, "invalid input");
                return false;
            }
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
    MasterOfEarthFront::MasterOfEarthFront() {
        m_type = MageType::Earth;
        m_ability = MageTypeAbility::Bury;
        m_hasUserSelected = false;
        m_hasCardTypeSelected = false;
    }

    bool MasterOfEarthFront::validPosition(const Coord& coord, Board& board, const Player& player) const {

        bool specialCards = board[coord].back().getType() == CombatCardType::ONE ||
            board[coord].back().getType() == CombatCardType::ETER ||
            board[coord].back().getType() == CombatCardType::HOLE ? true : false;

        if (board[coord].back().getColor() != player.getColor() && !specialCards) {
            return true;
        }

        return false;
    }

    std::vector<Coord> MasterOfEarthFront::getChoices(Board& board, const Player& player) {

        m_availableChoices.clear();
        for (const auto& iterator : board) {
            if (validPosition(iterator.first, board, player)) {
                m_availableChoices.emplace_back(iterator.first);
            }
        }
        return m_availableChoices;


    }

    void MasterOfEarthFront::setSelectedCoord(const Coord& coord) {
        m_selectedCoord = coord;
        m_hasUserSelected = true;
    }

    void MasterOfEarthFront::setSelectedCardType(CombatCardType cardType) {
        m_selectedCardType = cardType;
        m_hasCardTypeSelected = true;
    }


    bool MasterOfEarthFront::apply(Board& board, Player& player) {
        getChoices(board, player);

        if (m_availableChoices.empty()) {
            Logger::log(Level::INFO, "No valid position for using this Mage!");
            return false;
        }

        if (!m_hasUserSelected) {
            Logger::log(Level::WARNING, "No selection made yet!");
            return false;
        }
        if (!m_hasCardTypeSelected) {
            Logger::log(Level::WARNING, "No card type selected!");
            return false;
        }

        if (std::find(m_availableChoices.begin(), m_availableChoices.end(), m_selectedCoord) == m_availableChoices.end()) {
            Logger::log(Level::WARNING, "Invalid choice!");
            return false;
        }

        auto card = player.getCard(m_selectedCardType);
        if (card.getType() >= board[m_selectedCoord].back().getType()) {
            Logger::log(Level::WARNING, "Invalid card choice!");
            return false;
        }

        board.appendSpecialCard(m_selectedCoord, std::move(card));
        Logger::log(Level::INFO, "Mage Earth Bury ability card used");
        m_hasUserSelected = false;
        return true;
    }
    ///---------Hole----------

    MasterOfEarthBack::MasterOfEarthBack() {
        m_type = MageType::Earth;
        m_ability = MageTypeAbility::Hole;
        m_hasUserSelected = false;

    }
    void MasterOfEarthBack::setSelectedCoord(const Coord& coord) {
        m_selectedCoord = coord;
        m_hasUserSelected = true;
    }

    const std::vector<Coord>& MasterOfEarthBack::getAvailableChoices(const Board& board) const {
        m_availableChoices.assign(board.availableSpaces().begin(), board.availableSpaces().end());
        return m_availableChoices;
    }

    bool MasterOfEarthBack::apply(Board& board, Player& player) {
        getAvailableChoices(board);

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

        CombatCard card(CombatCardType::HOLE, player.getColor());
        board.appendMove(m_selectedCoord, std::move(card));
        Logger::log(Level::INFO, "Mage Earth Hole ability card used");
        m_hasUserSelected = false;
        return true;
    }


    /// ---------------------------------------MasterOfAir----------------------------------------
    /// ---------BlowAway--------
    MasterOfAirFront::MasterOfAirFront()
        : m_hasUserSelectedFrom(false), m_hasUserSelectedTo(false) {
        m_type = MageType::Air;
        m_ability = MageTypeAbility::BlowAway;
    }


    bool MasterOfAirFront::apply(Board& board, Player& player) {

        getAvailableChoices(board, player);

        if (m_availableChoices.empty()) {
            Logger::log(Level::WARNING, "You can't use this mage right now!");
            return false;
        }

        if (!m_hasUserSelectedFrom || !m_hasUserSelectedTo) {
            Logger::log(Level::WARNING, "Move coordinates have not been fully selected!");
            return false;
        }

        if (std::find(m_availableChoices.begin(), m_availableChoices.end(), m_moveFrom) == m_availableChoices.end()) {
            Logger::log(Level::WARNING, "Invalid source coordinate!");
            return false;
        }
        if (board.isValidMoveStack(m_moveFrom, m_moveTo)) {
            board.moveStack(m_moveFrom, m_moveTo);
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
    }

    void MasterOfAirFront::setMoveTo(const Coord& coord) {
        m_moveTo = coord;
        m_hasUserSelectedTo = true;
    }


    const std::vector<Coord>& MasterOfAirFront::getAvailableChoices(const Board& board, const Player& player) {
        m_availableChoices.clear();
        for (const auto& [coord, stack] : board) {
            if (stack.back().getColor() == player.getColor()) {
                m_availableChoices.emplace_back(coord);
            }
        }
        return m_availableChoices;
    }

    // ---------BlowEter---------

    MasterOfAirBack::MasterOfAirBack() {
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
    bool MasterOfAirBack::apply(Board& board, Player& player) {
        getAvailableChoices(board);
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
        CombatCard eter(CombatCardType::ETER, player.getColor());
        board.appendMove(m_selectedCoord, std::move(eter));
        Logger::log(Level::INFO, "Mage Air BlowEter ability card used");

        m_hasUserSelected = false; // reset for next use
        return true;
    }



    //----------------------------------------- MasterOfWater---------------------------------------

    //---------Boat------------
    MasterOfWaterFront::MasterOfWaterFront() {
        m_type = MageType::Water;
        m_ability = MageTypeAbility::Boat;
        m_hasUserSelectedFrom = false;
        m_hasUserSelectedTo = false;
    }
    void MasterOfWaterFront::setMoveFrom(const Coord& coord) {
        m_moveFrom = coord;
        m_hasUserSelectedFrom = true;
    }

    void MasterOfWaterFront::setMoveTo(const Coord& coord) {
        m_moveTo = coord;
        m_hasUserSelectedTo = true;
    }
    const std::vector<Coord>& MasterOfWaterFront::getAvailableChoices(const Board& board, const Player& player) {
        m_availableChoices.clear();
        for (const auto& [coord, stack] : board) {
            if (stack.back().getColor() != player.getColor()) {
                m_availableChoices.emplace_back(coord);
            }
        }
        return m_availableChoices;
    }
    bool MasterOfWaterFront::apply(Board& board, Player& player) {
        getAvailableChoices(board, player);
        if (m_availableChoices.empty()) {
            Logger::log(Level::WARNING, "Invalid choice!");
            return false;
        }

        if (!m_hasUserSelectedFrom || !m_hasUserSelectedTo) {
            Logger::log(Level::WARNING, "Move coordinates have not been fully selected!");
            return false;
        }

        if (std::find(m_availableChoices.begin(), m_availableChoices.end(), m_moveFrom) == m_availableChoices.end()) {
            Logger::log(Level::WARNING, "Invalid source coordinate!");
            return false;
        }


        if (board.isValidMoveStack(m_moveFrom, m_moveTo)) {
            board.moveStack(m_moveFrom, m_moveTo);
            Logger::log(Level::INFO, "Mage Water Boat ability card used");
        }
        else {
            Logger::log(Level::INFO, "Wrong choice");
            return false;
        }
        return true;
    }



    // -----------BoatRowOrColumn--------------

    MasterOfWaterBack::MasterOfWaterBack() {
        m_type = MageType::Water;
        m_ability = MageTypeAbility::BoatRowOrColumn;
        m_hasSelectedBorder = false;
        m_hasSelectedDestination = false;
    }


    std::optional<std::unordered_map<BorderType, std::vector<Coord>>> MasterOfWaterBack::getBorders(Board& board, Player& player) const {
        std::unordered_map<BorderType, std::vector<Coord>> borders;

        auto [corner1, corner2] = board.getBoudingRect();
        auto [width, height] = board.getBoundingRectSize();

        std::vector<Coord> top_border, bottom_border, left_border, right_border;


        for (uint16_t x = corner1.first; x <= corner2.first; x += 2) {
            if (board.hasStack({ x, corner1.second })) {
                top_border.push_back({ x, corner1.second });
            }
            if (height > 1 && board.hasStack({ x, corner2.second })) {
                bottom_border.push_back({ x, corner2.second });
            }
        }
        for (uint16_t y = corner1.second; y <= corner2.second; y++) {
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


    void MasterOfWaterBack::setSelectedBorder(uint16_t index) {
        m_selectedBorderIndex = index;
        m_hasSelectedBorder = true;
    }

    void MasterOfWaterBack::setSelectedDestination(int destination) {
        m_selectedDestination = destination;
        m_hasSelectedDestination = true;
    }

    bool MasterOfWaterBack::apply(Board& board, Player& player) {
        auto borders = getBorders(board, player);
        if (!borders || borders->empty()) {
            Logger::log(Level::INFO, "There are no borders to move yet");
            return false;
        }

        if (!m_hasSelectedBorder || !m_hasSelectedDestination) {
            Logger::log(Level::WARNING, "Border or destination not selected!");
            return false;
        }

        if (m_selectedBorderIndex >= borders->size()) {
            Logger::log(Level::WARNING, "Invalid border index!");
            return false;
        }

        auto selectedBorderIt = std::next(borders->begin(), m_selectedBorderIndex);
        const auto& selectedBorder = selectedBorderIt->second;

        char direction = (selectedBorderIt->first == BorderType::Left || selectedBorderIt->first == BorderType::Right) ? 'c' : 'r';
        uint16_t from_move = (direction == 'r') ? selectedBorder.front().second : selectedBorder.front().first;

        auto [corner1, corner2] = board.getBoudingRect();
        if (direction == 'r') {
            if (m_selectedDestination != corner2.second + 1 && m_selectedDestination != corner1.second - 1) {
                Logger::log(Level::WARNING, "Invalid row destination!");
                return false;
            }
            board.moveRow(from_move, m_selectedDestination);
        }
        else {
            if (m_selectedDestination != corner2.first + 2 && m_selectedDestination != corner1.first - 2) {
                Logger::log(Level::WARNING, "Invalid column destination!");
                return false;
            }
            board.moveColumn(from_move, m_selectedDestination);
        }

        Logger::log(Level::INFO, "Mage Water BoatRowOrColumn ability card used");
        m_hasSelectedBorder = false;
        m_hasSelectedDestination = false;
        return true;
    }
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
