﻿#include"Masters.h"

#include <format>

using namespace logger;

namespace base {



    ///----------------------------------------------------MasterOfFire----------------------------------------
    ///---------Burn------------

    MasterOfFireFront::MasterOfFireFront() {
        m_type = MageType::Fire;
        m_ability = MageTypeAbility::Burn;
    }

    
    bool MasterOfFireFront::apply(Board& board, Player& player) {
        auto choices = getChoices(board, player);
        if (choices.empty())
        {
            Logger::log(Level::INFO, "You can't use this mage right now");
            return false;
        }

        std::cout << "Available choices:\n";
        for (auto& [x, y] : choices) {
            std::cout << std::format("({}, {})", x, y);
        }
        std::cout << '\n';

        Coord choice;
        std::cin >> choice.first >> choice.second;
        if (std::find(choices.begin(), choices.end(), choice) == choices.end()) {
            Logger::log(Level::INFO, "Impossible choice");
            return false;
        }
        board.removeTopCardAt(choice);
        Logger::log(Level::INFO, "Mage fire ability remove top card used");
        return true;
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

    bool MasterOfFireBack::apply(Board& board, Player& player) {
        auto choices = getChoices(board, player);
        if (choices.first.empty() && choices.second.empty())
        {
            Logger::log(Level::INFO, "You can't use this mage right now");
            return false;
        }
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
            if (std::find(choices.first.begin(), choices.first.end(), choice) != choices.first.end()) {
                board.removeRow(choice);
            }
            else {
                Logger::log(Level::INFO, "invalid input");
                return false;
            }
        }
        else if (row_or_col == 'c') {
            if (std::find(choices.second.begin(), choices.second.end(), choice) != choices.second.end()) {
                board.removeColumn(choice);
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

    std::pair<std::vector<uint16_t>, std::vector<uint16_t>> MasterOfFireBack::getChoices(const Board& board, const Player& player) {
        std::vector<uint16_t> line_choices;
        std::vector<uint16_t> column_choices;

        std::unordered_map<uint16_t, std::pair<uint16_t, bool>> rows;
        std::unordered_map<uint16_t, std::pair<uint16_t, bool>> cols;

        for (const auto& [coord, stack] : board) {
            auto& [x, y] = coord;

            rows[y].first ++;
            cols[x].first ++;

            if (stack.back().getColor()==player.getColor()) {
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

    std::vector<Coord> MasterOfEarthFront::getChoices(Board& board, const Player& player) const  {

        std::vector<Coord>choices;
        for (const auto& iterator : board) {
            if (validPosition(iterator.first, board, player)) {
                choices.emplace_back(iterator.first);
            }
        }
        return choices;


    }

    bool MasterOfEarthFront::apply(Board& board, Player& player) {

        auto choices = getChoices(board, player);
        if (choices.empty()) {
            Logger::log(Level::INFO, "No valid position for using this Mage!");
            return false;
        }
        std::cout << "Your available choices are: \n";
        for (const auto& [x, y] : choices) {
            std::cout << std::format("({},{})", x, y) << "\n";
        }
        Coord choice;
        std::cin >> choice.first >> choice.second;
        if (std::find(choices.begin(), choices.end(), choice) == choices.end()) {
            Logger::log(Level::WARNING, "Invalid choice!\n");
            return false;
        }
        char card_type;
        std::cin >> card_type;
        auto card = player.getCard(charToCombatCard(card_type));

        if (card.getType() >= board[choice].back().getType()) {
            Logger::log(Level::WARNING, "Invalid card choice!\n");
            return false;
        }
        auto& cards = board.getCombatCards();
        cards[choice].emplace_back(std::move(card)); 
        Logger::log(Level::INFO, "Mage Earth Bury ability card used");
        return true;
    }
    ///---------Hole----------

    MasterOfEarthBack::MasterOfEarthBack() {
        m_type = MageType::Earth;
        m_ability = MageTypeAbility::Hole;
    }

    bool MasterOfEarthBack::apply(Board& board, Player& player) {
        auto& choices = board.availableSpaces();
        if (!choices.empty()) {
            std::cout << "Your choices are: ";
            for (const auto& choice : choices) {
                std::cout << "(" << choice.first << "," << choice.second << ") ";
            }
            std::cout << "\n Pick one: ";
            Coord input;
            std::cin >> input.first >> input.second;

            if (std::find(choices.begin(), choices.end(), input) == choices.end()) {
                Logger::log(Level::INFO, "You entered an invalid option for this mage");
                return false;
            }
            CombatCard card(CombatCardType::HOLE, player.getColor());
            board.appendMove(input, std::move(card));
            Logger::log(Level::INFO, "Mage Earth Hole ability card used");
            return true;
        }
        else {
            Logger::log(Level::INFO, "No available choices for this mage rn");
            return false;
        }
    }


    /// ---------------------------------------MasterOfAir----------------------------------------
    /// ---------BlowAway--------
    MasterOfAirFront::MasterOfAirFront() {
        m_type = MageType::Air;
        m_ability = MageTypeAbility::BlowAway;
    }

    bool MasterOfAirFront::apply(Board& board, Player& player) {

        std::vector<Coord> choices = getChoices(board, player);
        if (choices.empty()) {
            Logger::log(Level::WARNING, "You can't use this mage right now!\n");
            return false;
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
            return false;
        }
        if (board.isValidMoveStack(coord_from, coord_to)) {
            board.moveStack(coord_from, coord_to);
            Logger::log(Level::INFO, "Mage Air BlowAway ability card used");
        }
        else {
            Logger::log(Level::INFO, "Wrong choice");
            return false;
        }
        return true;
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

    bool MasterOfAirBack::apply(Board& board, Player& player) {
        auto& choices = board.availableSpaces();
        if (!choices.empty()) {
            std::cout << "Your choices for additional Eter card are: \n";
            for (const auto& choice : choices) {
                std::cout << "(" << choice.first << ", " << choice.second << ") ";
            }
            std::cout << std::endl;
            Coord input;
            std::cin >> input.first >> input.second;

            if (std::find(choices.begin(), choices.end(), input) == choices.end()) {
                Logger::log(Level::INFO, "Invalid option");
                return false;
            }
            CombatCard eter(CombatCardType::ETER, player.getColor());
            board.appendMove(input, std::move(eter));
            Logger::log(Level::INFO, "Mage Air BlowEter ability card used");
        }
        else {
            Logger::log(Level::INFO, "No available options for this mage rn");
            return false;

        }
        return true;
    }

    //----------------------------------------- MasterOfWater---------------------------------------

    //---------Boat------------
    MasterOfWaterFront::MasterOfWaterFront() {
        m_type = MageType::Water;
        m_ability = MageTypeAbility::Boat;
    }

    bool MasterOfWaterFront::apply(Board& board, Player& player) {
        std::vector<Coord>choices = getChoices(board, player);
        if (choices.empty()) {
            Logger::log(Level::WARNING, "Invalid choice!\n");
            return false;
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
            return false;
        }
      
        if (board.isValidMoveStack(coord_from, coord_to)) {
            board.moveStack(coord_from, coord_to);
            Logger::log(Level::INFO, "Mage Water Boat ability card used");
        }
        else {
            Logger::log(Level::INFO, "Wrong choice");
            return false;
        }
        return true;
    }

    std::vector<Coord> MasterOfWaterFront::getChoices(Board& board, Player& player) 
    {
        std::vector<Coord>choices; 
        for (const auto& [coord, stack] : board) {
            if (stack.back().getColor() != player.getColor()) {
                choices.emplace_back(coord);
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

   static std::string_view borderToString(BorderType border) {

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
            return { 0, '/0' };

        }
        auto selectedBorderIt = std::next(borders->begin(), selectedBorderIndex);
        const auto& selectedBorder = selectedBorderIt->second;

        char direction = (selectedBorderIt->first == BorderType::Left || selectedBorderIt->first == BorderType::Right) ? 'c' : 'r';
        uint16_t from_move = (direction == 'r') ? selectedBorder.front().second : selectedBorder.front().first;
        
        auto [corner1, corner2] = board.getBoudingRect();
        std::cout << "Valid moves for the " << (direction == 'r' ? "rowBorder" : "columnBorder") << " are:\n";
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

    bool MasterOfWaterBack::apply(Board& board, Player& player) {
        auto borders = getBorders(board, player);
        if (!borders || borders->empty()) {
            Logger::log(Level::INFO, "There are no borders to move yet");
            return false;
        }
        auto result = selectBorders(borders, board);
        if (result.first != 0) {
            int to_move;
            std::cout << "Enter the destination: ";
            std::cin >> to_move;
            auto [corner1, corner2] = board.getBoudingRect(); 
            if (result.second == 'r') {
                if (to_move != corner2.second + 1 && to_move != corner1.second - 1) {
                    Logger::log(Level::WARNING, "Invalid choice");
                    return false;
                }
                board.moveRow(result.first, to_move); 
                  
            }
            else { 
                if (to_move != corner2.first + 2 && to_move != corner1.first - 2) {
                    Logger::log(Level::WARNING, "Invalid choice");  
                    return false;
                }
                board.moveColumn(result.first, to_move); 
            }
            Logger::log(Level::INFO, "Mage Water BoatRowOrColumn ability card used");
        }
        else {
            Logger::log(Level::WARNING, "Wrong choice");
            return false;
        }
        return true;
    }
}