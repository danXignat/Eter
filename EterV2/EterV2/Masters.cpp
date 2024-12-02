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
        char line_or_col;
        std::cin >> choice >> line_or_col;

        if (line_or_col == 'l') {
            board.removeRow(choice);
        }
        else if (line_or_col == 'c') {
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

    std::vector<Coord> MasterOfEarthFront::getChoices(Board& board, const Player& player) {
        std::vector<Coord>choices;
        for (const auto& [coord, stack] : board) {
            if (!stack.empty() && stack.back().getColor() != player.getColor()) {
                choices.emplace_back(coord);
            }
        }
        return choices;
    }
    void MasterOfEarthFront::apply(Board& board, Player& player) {
        bool bury = true;
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
        if (board.isValidMove(choice, *card, true)) {
            board.appendMove(choice, std::move(*card));
        }
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
                std::cout << choice.first << " " << choice.second << '\n';
            }
            std::cout << "Pick one";
            Coord input;
            std::cin >> input.first >> input.second;
            CombatCard card  (CombatCardType::HOLE, player.getColor());
            if (board.isValidMove(input,card)) {
                board.appendMove(input, std::move(card));
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
        board.moveStack(coord_from, coord_to);

    }

    std::vector<Coord> MasterOfAirFront::getChoices(Board& board, const Player& player) {
        std::vector<Coord> choices;
        for (const auto& [coord, stack] : board) {
            bool player_card_found = false;
            if (stack.size() >= 1) {
                for (const auto& combat_card : stack) {
                    if (combat_card.getColor() == player.getColor()) {
                        player_card_found = true;
                        break;
                    }
                }
                if (player_card_found) {
                    choices.emplace_back(coord);
                }
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
        std::vector<Coord>choices = board.availableSpaces();
        if (!choices.empty()) {
            std::cout << "Your choices for additional Eter card are: \n";
            for (const auto& choice : choices) {
                std::cout << choice.first << " " << choice.second << '\n';
            }
            Coord input;
            std::cin >> input.first >> input.second;
            CombatCard eter(CombatCardType::ETER, player.getColor());
            if (board.isValidMove(input, eter)) {
                board.appendMove(input, std::move(eter));
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

    void MasterOfWaterFront::apply(Board& board, Player& player) {
        std::vector<Coord>choices = getChoices(board, player);
        if (choices.empty()) {
            std::cout << "No options available for this mage!\n";
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
        board.moveStack(coord_from, coord_to);
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

    void MasterOfWaterBack::apply(Board& board, Player& player) {
        std::optional<std::vector<std::vector<Coord>>> borders = board.getBorders();

        if (borders) {
            for (const auto& border : *borders) {
                std::cout << "Border: ";
                for (const auto& coord : border) {
                    std::cout << "(" << coord.first << ", " << coord.second << ") ";
                }
                std::cout << "\n";
            }
        }
        else {
            std::cout << "No borders found.\n";
        }
        for (const auto& [coord, stack] : board) {

        }

    }
}