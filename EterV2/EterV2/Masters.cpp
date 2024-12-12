#include"Masters.h"

#include <format>

using namespace logger;

namespace base {



    ///----------------------------------------------------MasterOfFire----------------------------------------
    ///---------Burn------------

    MasterOfFireFront::MasterOfFireFront() {
        m_type = MageType::Fire;
        m_ability = MageTypeAbility::Burn;
    }

    void MasterOfFireFront::apply(Board& board, Player& player) {
        auto choices = getChoices(board, player);

        std::cout << "Available choices:\n";
        for (auto [x, y] : choices) {
            std::cout << std::format("({}, {})", x, y);
        }
        std::cout << '\n';

        Coord choice;
        std::cin >> choice.first >> choice.second;

        board.removeTopCardAt(choice);
        Logger::log(Level::INFO, "Mage fire ability remove top card used");
    }

    std::vector<Coord> MasterOfFireFront::getChoices(const Board& board, const Player& player) {
        std::vector<Coord> choices;

        for (const auto& [coord, stack] : board) {
            auto& [x, y] = coord;

            if (stack.size() < 2) {
                continue;
            }

            bool is_player_card = stack[stack.size() - 2].getColor() == player.getColor();
            bool is_covered = stack.back().getColor() != player.getColor();
            if (is_player_card && is_covered) {
                choices.emplace_back(x, y);
            }
        }

        return choices;
    }

    ///--------Burn Row and Col-----------


    MasterOfFireBack::MasterOfFireBack() {
        m_type = MageType::Fire;
        m_ability = MageTypeAbility::BurnRowOrColumn;
    }

    void MasterOfFireBack::apply(Board& board, Player& player) {
        auto choices = getChoices(board, player);
        std::cout << "Available choices:\n";

        std::cout << "Available rows: ";
        for (uint16_t row : choices.first) {
            std::cout << row << " ";
        }
        std::cout << '\n';

        std::cout << "Available cols: ";
        for (uint16_t col : choices.second) {
            std::cout << col << " ";
        }
        std::cout << '\n';

        uint16_t choice;
        char row_or_col;
        std::cin >> choice >> row_or_col;

        if (row_or_col == 'r') {
            board.removeRow(choice);
        }
        else if (row_or_col == 'c') {
            board.removeColumn(choice);
        }
        else {
            Logger::log(Level::INFO, "invalid input");
        }

        Logger::log(Level::INFO, "Mage fire ability remove top card used");
    }

    std::pair<std::vector<uint16_t>, std::vector<uint16_t>> MasterOfFireBack::getChoices(const Board& board, const Player& player) {
        std::vector<uint16_t> line_choices;
        std::vector<uint16_t> column_choices;

        std::unordered_map<uint16_t, std::pair<uint16_t, bool>> rows;
        std::unordered_map<uint16_t, std::pair<uint16_t, bool>> cols;

        for (const auto& [coord, stack] : board) {
            auto& [x, y] = coord;

            rows[y].first += uint16_t(stack.size());
            cols[x].first += uint16_t(stack.size());

            if (board.isCardOfColorAt(player.getColor(), coord)) {
                rows[y].second = true;
                cols[x].second = true;
            }
        }

        for (const auto& [pos, val] : rows) {
            if (val.first >= 3 && val.second) {
                line_choices.push_back(pos);
            }
        }
        for (const auto& [pos, val] : cols) {
            if (val.first >= 3 && val.second) {
                column_choices.push_back(pos);
            }
        }

        return { line_choices, column_choices };
    }


    ///------------------------------------------ MasterOfEarth-------------------------------------------
    ///-------------Bury-------------
    MasterOfEarthFront::MasterOfEarthFront() {
        m_type = MageType::Earth;
        m_ability = MageTypeAbility::Bury;
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

        std::vector<Coord>choices;
        for (const auto& iterator : board) {
            if (validPosition(iterator.first, board, player)) {
                choices.emplace_back(iterator.first);
            }
        }
        return choices;


    }

    void MasterOfEarthFront::apply(Board& board, Player& player) {

        auto choices = getChoices(board, player);
        if (choices.empty()) {
            std::cout << "No valid position for using this Mage!\n";
            return;
        }
        std::cout << "Your available choices are: \n";
        for (const auto& [x, y] : choices) {
            std::cout << std::format("({},{})", x, y) << "\n";
        }
        Coord choice;
        std::cin >> choice.first >> choice.second;
        if (std::find(choices.begin(), choices.end(), choice) == choices.end()) {
            Logger::log(Level::WARNING, "Invalid choice!\n");
            return;
        }
        char card_type;
        std::cin >> card_type;
        auto card = player.getCard(charToCombatCard(card_type));

        if (card.getType() >= board[choice].back().getType()) {
            Logger::log(Level::WARNING, "Invalid card choice!\n");
            return;
        }

        board.appendMove(choice, std::move(card));
        Logger::log(Level::INFO, "Mage Earth Bury ability card used");
    }
    ///---------Hole----------

    MasterOfEarthBack::MasterOfEarthBack() {
        m_type = MageType::Earth;
        m_ability = MageTypeAbility::Hole;
    }

    void MasterOfEarthBack::apply(Board& board, Player& player) {
        auto choices = board.availableSpaces();
        if (!choices.empty()) {
            std::cout << "Your choices are: ";
            for (const auto& choice : choices) {
                std::cout << "(" << choice.first << "," << choice.second << ") ";
            }
            std::cout << "\n Pick one: ";
            Coord input;
            std::cin >> input.first >> input.second;
            CombatCard card(CombatCardType::HOLE, player.getColor());
            if (board.isValidPlaceCard(input, card)) {
                board.appendMove(input, std::move(card));
                Logger::log(Level::INFO, "Mage Earth Hole ability card used");
            }
        }
        else {
            std::cout << "No available choices for this mage rn";
        }
    }


    /// ---------------------------------------MasterOfAir----------------------------------------
    /// ---------BlowAway--------
    MasterOfAirFront::MasterOfAirFront() {
        m_type = MageType::Air;
        m_ability = MageTypeAbility::BlowAway;
    }

    void MasterOfAirFront::apply(Board& board, Player& player) {

        std::vector<Coord> choices = getChoices(board, player);
        if (choices.empty()) {
            std::cout << "No options available for this mage!\n";
            Logger::log(Level::WARNING, "You can't use this mage right now!\n");
            return;
        }
        std::cout << "Your choices are: \n";
        for (const auto& [x, y] : choices) {
            std::cout << std::format("({},{})", x, y) << "\n";
        }
        Coord coord_from;
        Coord coord_to;
        std::cout << "Move from coordinates: \n";
        std::cin >> coord_from.first >> coord_from.second;
        std::cout << "To coordinates: \n";
        std::cin >> coord_to.first >> coord_to.second;

        if (std::find(choices.begin(), choices.end(), coord_from) == choices.end()) {
            Logger::log(Level::WARNING, "Invalid choice!\n");
            return;
        }
        /*if (board.moveStack(coord_from, coord_to)) {// acuma nu mai e bool e void si e alta fucntie care verifica
            Logger::log(Level::INFO, "Mage Air BlowAway ability card used");
        }*/
    }

    std::vector<Coord> MasterOfAirFront::getChoices(Board& board, const Player& player) {
        std::vector<Coord> choices;
        for (const auto& [coord, stack] : board) {
            if (stack.back().getColor() == player.getColor()) {
                choices.emplace_back(coord);
            }
        }
        return choices;
    }

    // ---------BlowEter---------

    MasterOfAirBack::MasterOfAirBack() {
        m_type = MageType::Air;
        m_ability = MageTypeAbility::BlowEter;
    }

    void MasterOfAirBack::apply(Board& board, Player& player) {
        auto choices = board.availableSpaces();
        if (!choices.empty()) {
            std::cout << "Your choices for additional Eter card are: \n";
            for (const auto& choice : choices) {
                std::cout << "(" << choice.first << ", " << choice.second << ") ";
            }
            std::cout << std::endl;
            Coord input;
            std::cin >> input.first >> input.second;
            CombatCard eter(CombatCardType::ETER, player.getColor());
            if (board.isValidPlaceCard(input, eter)) {
                board.appendMove(input, std::move(eter));
                Logger::log(Level::INFO, "Mage Air BlowEter ability card used");
            }
        }
        else {
            std::cout << "No available options for this mage rn";
        }

    }

    //----------------------------------------- MasterOfWater---------------------------------------

    //---------Boat------------
    MasterOfWaterFront::MasterOfWaterFront() {
        m_type = MageType::Water;
        m_ability = MageTypeAbility::Boat;
    }

    bool MasterOfWaterFront::areAdjacentCards(const Coord& coord, Board& board) {
        std::vector<Coord> directions = {
            {coord.first - 1, coord.second},
            {coord.first + 1, coord.second},
            {coord.first, coord.second - 2},
            {coord.first, coord.second + 2}
        };

        for (const auto& dir : directions) {
            if (!board[dir].empty()) {
                return true;
            }
        }

        return false;
    }

    bool MasterOfWaterFront::allCardsHaveNeighbors( Board&board) {
        for (const auto& card : board) {
            if (!areAdjacentCards(card.first,board)) {
                return false;
            }
        }
        return true;
    }

    void MasterOfWaterFront::apply(Board& board, Player& player) {
        std::vector<Coord>choices = getChoices(board, player);
        if (choices.empty()) {
            std::cout << "No options available for this mage!\n";
            Logger::log(Level::WARNING, "Invalid choice!\n");
            return;
        }
        std::cout << "Your choices are: \n";
        for (const auto& [x, y] : choices) {
            std::cout << std::format("({},{})", x, y) << "\n";
        }
        Coord coord_from;
        Coord coord_to;
        std::cout << "Move from coordinates: \n";
        std::cin >> coord_from.first >> coord_from.second;
        std::cout << "To coordinates: \n";
        std::cin >> coord_to.first >> coord_to.second;

        if (std::find(choices.begin(), choices.end(), coord_from) == choices.end()) {
            Logger::log(Level::WARNING, "Invalid choice!\n");
            std::cout << "Invalid choice";
            return;
        }
        /*if (board.moveStack(coord_from, coord_to)) {
            Logger::log(Level::INFO, "Mage Water Boat ability card used");
        }*/

        std::cout << board.isValidMoveStack(coord_from, coord_to);
        int n;
        std::cin >> n;
    }

    std::vector<Coord> MasterOfWaterFront::getChoices(Board& board, Player& player)
    {
        std::vector<Coord>choices;
        for (const auto& [coord, stack] : board) {
            bool opponent_card = false;
            if (!stack.empty()) {
                for (const auto& combat_card : stack) {
                    if (combat_card.getColor() != player.getColor()) {
                        opponent_card = true;
                        break;
                    }
                }
                if (opponent_card) {
                    choices.emplace_back(coord);
                }
            }
        }
        return choices;
    }

    // -----------BoatRowOrColumn--------------

    MasterOfWaterBack::MasterOfWaterBack() {
        m_type = MageType::Water;
        m_ability = MageTypeAbility::BoatRowOrColumn;
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

        auto addBorder = [&borders](std::vector<Coord>& border, BorderType type) {
            if (border.size() >= 3) {
                borders[type] = std::move(border);
            }
            };

        addBorder(top_border, BorderType::Top);
        addBorder(bottom_border, BorderType::Bottom);
        addBorder(left_border, BorderType::Left);
        addBorder(right_border, BorderType::Right);

        return borders.empty() ? std::nullopt : std::make_optional(borders);
    }

    std::string_view borderToString(BorderType border) {

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

    }

    std::pair<uint16_t, char> MasterOfWaterBack::selectBorders(const std::optional<std::unordered_map<BorderType, std::vector<Coord>>>& borders, Board& board) const {
        if (!borders || borders->empty()) {
            std::cout << "No borders found.\n";
            Logger::log(Level::WARNING, "Can't use thid card rn");
            return { -1, '\0' };
        }
        int index = 0;
        for (const auto& [border, coords] : *borders) {
            std::cout << "Border number " << index++ << "->" << borderToString(border) << ", at coords: ";
            for (const auto& coord : coords) {
                std::cout << "(" << coord.first << ", " << coord.second << ") ";
            }
            std::cout << "\n";
        }
        std::cout << std::endl;

        int selectedBorderIndex;
        std::cout << "Please select a border index to move: ";
        std::cin >> selectedBorderIndex;
        if (selectedBorderIndex < 0 || selectedBorderIndex >= borders->size()) {
            std::cout << "Invalid index!";
            Logger::log(Level::WARNING, "Invalid index for border");
            return { -1, '\0' };

        }
        auto selectedBorderIt = std::next(borders->begin(), selectedBorderIndex);
        const auto& selectedBorder = selectedBorderIt->second;

        char direction = (selectedBorderIt->first == BorderType::Left || selectedBorderIt->first == BorderType::Right) ? 'c' : 'r';
        uint16_t from_move = (direction == 'r') ? selectedBorder.front().second : selectedBorder.front().first;
        uint16_t to_move;

        auto [corner1, corner2] = board.getBoudingRect();
        std::cout << "Valid destinations for the " << (direction == 'r' ? "row" : "column") << " are:\n";
        if (direction == 'r') {
            if (selectedBorderIt->first == BorderType::Top) {
                std::cout << "Move to the bottom border:" << corner2.second + 1 << "\n";
            }
            else {
                std::cout << "Move to the top border:" << corner1.second - 1 << "\n";
            }
        }
        else {
            if (selectedBorderIt->first == BorderType::Left) {
                std::cout << "Move to the right border: " << corner2.first + 2 << "\n";
            }
            else {
                std::cout << "Move to the left border: " << corner1.first - 2 << "\n";
            }
        }
        return { from_move,direction };
    }

    void MasterOfWaterBack::apply(Board& board, Player& player) {
        auto borders = getBorders(board, player);

        auto result = selectBorders(borders, board);
        if (result.first != -1) {
            int to_move;
            std::cout << "Enter the destination: ";
            std::cin >> to_move;

            if (result.second == 'r') {
                board.moveRow(result.first, to_move);
            }
            else {
                board.moveColumn(result.first, to_move);
            }

            Logger::log(Level::INFO, "Mage Water BoatRowOrColumn ability card used");
        }
        return;
    }
}