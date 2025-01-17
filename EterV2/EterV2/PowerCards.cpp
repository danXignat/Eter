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
    }

    void Ash::apply(Board& board, Player& player) {
        if (player.hasUsedCards()) {
            player.renderUsedCards();
            std::cout << "Enter the coordinates and the card you want to play:";
            Coord coord;
            std::cin >> coord.first >> coord.second;

            char card_type;
            std::cin >> card_type;
            auto card = player.getUsedCard(charToCombatCard(card_type));
            if (board.isValidPlaceCard(coord, card)) {
                board.appendMove(coord, std::move(card));
            }
            Logger::log(Level::INFO, "Ash power card was used");

        }
        else {
            Logger::log(Level::WARNING, "You don't have any removed card");

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
                break;
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
                for (int i = 0; i < stack.size() - 1; i++) {
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

    std::unordered_map<Orientation, std::vector<uint16_t>>Hurricane::getOptions(Board& board) {
        std::unordered_map<Orientation, std::vector<uint16_t>>options;

        if (board.fixedWidth()) {
            auto rows = board.getFixedRows();
            std::cout << "Row options: ";
            for (const auto row : rows) {
                std::cout << row << ", ";
                options[Orientation::Row].push_back(row);
            }

        }
        if (board.fixedHeight()) {
            auto columns = board.getFixedColumns();
            std::cout << "\nColumn options: ";
            for (const auto column : columns) {
                std::cout << column << ", ";
                options[Orientation::Column].push_back(column);
            }
        }
        return options;
    }

    std::tuple<Orientation, uint16_t, MoveDirection>  Hurricane::input(Board& board) {

        auto options = getOptions(board);
        if (options.empty()) {
            Logger::log(Level::WARNING, "You can't use this mage right now");
            return { Orientation::Unknown,0,MoveDirection::Unknown };
        }
        std::cout << "Enter Row/Column and the index you want to shift\n";
        std::string orientation;
        uint16_t index;
        std::cin >> orientation >> index;
        auto orient = stringToOrientation(orientation);
        auto it = options.find(orient);

        if (it == options.end() || std::find(it->second.begin(), it->second.end(), index) == it->second.end()) {
            Logger::log(Level::WARNING, "Wrong choice");
            return { Orientation::Unknown,0,MoveDirection::Unknown };
        }

        std::cout << "Enter the direction you want to shift (Right/Left for rows, Up/Down for cols)";
        std::string shift_direction;
        std::cin >> shift_direction;
        auto shift_dir = stringToMoveDir(shift_direction);
        auto dirIt = validDirections.find(orient);

        if (std::find(dirIt->second.begin(), dirIt->second.end(), shift_dir) == dirIt->second.end()) {
            Logger::log(Level::WARNING, "Wrong choice");
            return { Orientation::Unknown,0,MoveDirection::Unknown };
        }

        return { orient,index,shift_dir };

    }


    void Hurricane::apply(Board& board, Player& player) {

        auto choice = input(board);
        auto orient = std::get<0>(choice);
        if (orient == Orientation::Unknown) {
            return;
        }
        auto index = std::get<1>(choice);
        auto shift_dir = std::get<2>(choice);
        std::vector<std::pair<Coord, Coord>>to_shift;
        if (orient == Orientation::Row) {

            if (shift_dir == MoveDirection::Right) {
                Coord coord = board.getRightMostOnRow(index);
                board.removeStack(coord);
                auto vect_coords = board.getCoordsOnRow(index);
                board.shiftRowToRight(vect_coords);
            }
            else {
                Coord coord = board.getLeftMostOnRow(index);
                board.removeStack(coord);
                auto vect_coords = board.getCoordsOnRow(index);
                board.shiftRowToLeft(vect_coords);
            }
        }
        else {

            if (shift_dir == MoveDirection::Up) {
                Coord coord = board.getTopMostOnColumn(index);
                board.removeStack(coord);
                auto vect_coord = board.getCoordsOnColumn(index);
                board.shiftColumnUp(vect_coord);
            }
            else {
                Coord coord = board.getBottomMostOnColumn(index);
                board.removeStack(coord);
                auto vect_coord = board.getCoordsOnColumn(index);
                board.shiftColumnDown(vect_coord);
            }
        }
    }



    ////------------------------------------------ Gust -------------------------------------------
    Gust::Gust() {
        m_ability = PowerCardType::Gust;
    }

    void Gust::apply(Board& board, Player& player) {
        std::vector<Coord>cardsCoord = getCardsCoord(board);
        std::cout << "Choose the card to move:" << std::endl;
        std::for_each(cardsCoord.begin(), cardsCoord.end(),
            [](const Coord& coord) {
                std::cout << "Card at " << coord.first << ", " << coord.second << std::endl;
            });
        std::cout << "Enter the coodinates of the card you want to move:";
        Coord cardCoord;
        std::cin >> cardCoord.first >> cardCoord.second;

        if (std::find(cardsCoord.begin(), cardsCoord.end(), cardCoord) == cardsCoord.end()) {
            Logger::log(Level::WARNING, "Invalid card coordinates");
            return;
        }
        /*CombatCard& card = board.getCombatCards()[cardCoord].back(); //n ai voie sa modifici asa boardul trebuie sa folosesti metode existente musai am facut getCombatCards const ca sa nu mai poti face asta
        std::vector<Coord>validMoves;
        std::vector<Coord> neighbors{
       {cardCoord.first - 2, cardCoord.second},
       {cardCoord.first + 2, cardCoord.second},
       {cardCoord.first, cardCoord.second - 1},
       {cardCoord.first, cardCoord.second + 1}
        };
        for (const auto& neighbor : neighbors) {
            if (board.getCombatCards().contains(neighbor) && !board.getCombatCards()[neighbor].back().isIllusion()) {
                CombatCard& neighborCard = board.getCombatCards()[neighbor].back();
                if (neighborCard.getType() < card.getType()) {
                    validMoves.push_back(neighbor);
                }
            }
        }*/

        /*if (!validMoves.empty()) {
            for (const auto& move : validMoves) {
                std::cout << "Valid moves:" << move.first << ", " << move.second << std::endl;
            }
            std::cout << "Enter the coodinates where you want to move the card:";
            Coord new_coord;
            std::cin >> new_coord.first >> new_coord.second;
            if (std::find(validMoves.begin(), validMoves.end(), new_coord) == validMoves.end()) {
                Logger::log(Level::WARNING, "Invalid coordinates to move the card");
                return;
            }
            if (board.isValidPlaceCard(new_coord, card)) {
                board.appendMove(new_coord, std::move(card));
                board.removeTopCardAt(cardCoord);
                Logger::log(Level::INFO, "Gust power card was used");
            }
        }
        else {
            Logger::log(Level::WARNING, "No valid coordinates to move the card");
        }*/
    }

    std::vector<Coord> Gust::getCardsCoord(const Board& board)const {
        std::vector<Coord>coords;
        for (const auto& [coord, stack] : board) {
            if (!stack.empty() && !stack.back().isIllusion()) {
                coords.emplace_back(coord);
            }
        }
        return coords;
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
    Wave::Wave() {
        m_ability = PowerCardType::Wave;
    }

    void Wave::apply(Board& board, Player& player) {
        if (board.availableSpaces().empty()) {
            Logger::log(Level::WARNING, "No empty spaces to move the stack");
        }
        std::vector<Coord>coordStack = validStacks(board);
        if (coordStack.empty()) {
            Logger::log(Level::WARNING, "No stacks on the board");
        }
        else {
            for (const auto& coord : coordStack) {
                Coord new_coord;
                std::cout << "Enter the coordinates where you want to move the stack:" << std::endl;
                std::cin >> new_coord.first >> new_coord.second;
                if (board.isValidMoveStack(coord, new_coord)) {
                    board.moveStack(coord, new_coord);
                }
                char card_type;
                std::cout << "Enter the card you want to play on the new empty space" << std::endl;
                std::cin >> card_type;
                auto card = player.getCard(charToCombatCard(card_type));
                if (board.isValidPlaceCard(coord, card)) {
                    board.appendMove(coord, std::move(card));
                }
            }
        }
        Logger::log(Level::INFO, "Wave power card was used");
    }

    std::vector<Coord>Wave::validStacks(const Board& board) const {
        std::vector<Coord>coordStack;
        for (const auto& [coord, stack] : board) {
            if (stack.size() > 1) {
                coordStack.emplace_back(coord);
            }
        }
        return coordStack;
    }


    ////------------------------------------------ Whirlpool -------------------------------------------
    Whirlpool::Whirlpool() {
        m_ability = PowerCardType::Whirlpool;
    }

    void Whirlpool::apply(Board& board, Player& player) {
        auto coord_pairs = getPairs(board);
        if (coord_pairs.empty()) {
            Logger::log(Level::WARNING, "No valid row to use power card");
        }

        std::cout << "Options:";
        for (const auto& pair : coord_pairs) {
            std::cout << " Pair: (" << pair.first.first << ", " << pair.second.second << ") and (" << pair.second.first << ", " << pair.second.second << ")" << std::endl;
            }
        std::cout << "Choose the coodinate pairs for which you want to use the power card " << std::endl;
        Coord first_coord, second_coord;
        std::cout << "First coord:";
        std::cin >> first_coord.first >> first_coord.second;
        std::cout << "Second coord:";
        std::cin >> second_coord.first >> second_coord.second;

        std::pair<Coord, Coord>input_coords{first_coord, second_coord};
        if (std::find(coord_pairs.begin(), coord_pairs.end(), input_coords) != coord_pairs.end()) {
          /* CombatCard& firstCard = board.getCombatCards()[first_coord].back();
           CombatCard& secondCard = board.getCombatCards()[second_coord].back();*/

           /*if (firstCard.getType() < secondCard.getType()) { //aceeasi problema ca mai sus
               board.appendMove({ first_coord.first + 2,first_coord.second }, std::move(firstCard));
               board.appendMove({ second_coord.first - 2,second_coord.second }, std::move(secondCard));
           }
           else if (firstCard.getType() == secondCard.getType()) {
               std::cout << "Choose the coordinates of the card you want to place on top:";
               Coord equal_card_cood;
               std::cin >> equal_card_cood.first>>equal_card_cood.second;
               if (equal_card_cood != first_coord || equal_card_cood != second_coord) {
                   Logger::log(Level::WARNING, "Invalid coordinates");
               }
               if (equal_card_cood == first_coord) {
                   board.appendAnyCard({ second_coord.first - 2,second_coord.second }, std::move(secondCard));
                   board.appendAnyCard({ first_coord.first + 2,first_coord.second }, std::move(firstCard));
               }
               else {
                   board.appendAnyCard({ first_coord.first + 2,first_coord.second }, std::move(firstCard));
                   board.appendAnyCard({ second_coord.first - 2,second_coord.second }, std::move(secondCard));
               }
           }
           else {
               board.appendMove({ second_coord.first - 2,second_coord.second }, std::move(secondCard));
               board.appendMove({ first_coord.first + 2,first_coord.second }, std::move(firstCard));
           }
           */
           board.popTopCardAt(first_coord);
           board.popTopCardAt(second_coord);
           Logger::log(Level::INFO, "Whirlpool power card was played");
        }
        else {
            Logger::log(Level::WARNING, "Invalid coords");
        }

    }

    std::vector< std::pair<Coord, Coord>>Whirlpool::getPairs(Board& board) {
        std::vector< std::pair<Coord, Coord>> row_pairs;

        for (const auto& [coord, stack] : board) {
            if (board.hasStack({ coord.first + 4, coord.second }) && !board.hasStack({ coord.first + 2, coord.second })) {
                row_pairs.emplace_back(std::make_pair(coord, Coord{ coord.first + 4, coord.second }));
            }
        }
        return row_pairs;
    }


    ////------------------------------------------ Blizzard -------------------------------------------
    Blizzard::Blizzard() {
        m_ability = PowerCardType::Blizzard;
    }

    void Blizzard::apply(Board& board, Player& player) {
        
        if (board.availableSpaces().empty()) {
            Logger::log(Level::WARNING, "No available spaces for opponent's next turn");
            return;
        }

        std::cout << "Choose what to block (R for Row, C for Column): ";
        char choice;
        std::cin >> choice;

        if (toupper(choice) == 'R') {
          
            std::unordered_set<uint16_t> rows;
            for (const auto& [coord, stack] : board.getCombatCards()) {
                rows.insert(coord.second);
            }
            for (const auto& y : board.availableSpaces()) {
                rows.insert(y.second); 
            }

            std::cout << "Available rows to block: ";
            for (uint16_t row : rows) {
                std::cout << row << " ";
            }

            std::cout << "\nEnter the row number you want to block: ";
            uint16_t chosen_row;
            std::cin >> chosen_row;

            if (rows.find(chosen_row) == rows.end()) {
                Logger::log(Level::WARNING, "Invalid row selection");
                return;
            }

            board.blockRow(chosen_row, player.getColor());
            Logger::log(Level::INFO, "Row {} has been blocked for the opponent's next turn", chosen_row);
        }
        else if (toupper(choice) == 'C') {
            
            std::unordered_set<uint16_t> columns;
            for (const auto& [coord, stack] : board.getCombatCards()) {
                columns.insert(coord.first);
            }
            for (const auto& x : board.availableSpaces()) {
                columns.insert(x.first);
            }

            std::cout << "Available columns to block: ";
            for (uint16_t col : columns) {
                std::cout << col << " ";
            }

            std::cout << "\nEnter the column number you want to block: ";
            uint16_t chosen_column;
            std::cin >> chosen_column;

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

    std::unordered_map<Orientation, std::vector<int16_t>> Waterfall::getOptions(Board& board) {
        int16_t size = static_cast<int16_t>(board.getSize() - 1);
        std::unordered_map<Orientation, std::vector<int16_t>> choices;
        std::unordered_set<int16_t> processedRows;
        std::unordered_set<int16_t> processedCols;

        std::cout << "Your options are: ";

        for (const auto& [coord, _] : board) {
            if (processedRows.find(coord.second) == processedRows.end()) {
                auto countRowElements = board.getCoordsOnRow(coord.second);
                if (countRowElements.size() >= size) {
                    std::cout << "index row: " << coord.second << " ";
                    choices[Orientation::Row].emplace_back(coord.second);
                    processedRows.insert(coord.second);
                }
            }

            if (processedCols.find(coord.first) == processedCols.end()) {
                auto countColElements = board.getCoordsOnColumn(coord.first);
                if (countColElements.size() >= size) {
                    std::cout << "index column: " << coord.first << " ";
                    choices[Orientation::Column].emplace_back(coord.first);
                    processedCols.insert(coord.first);
                }
            }
        }
        std::cout << std::endl;
        return choices;
    }


    std::tuple< Orientation, int16_t, MoveDirection> Waterfall::input(Board& board) {
        auto options = getOptions(board);

        if (options.empty()) {
            Logger::log(Level::WARNING, "You can't use this mage right now");
            return { Orientation::Unknown,0,MoveDirection::Unknown };
        }

        std::cout << "Enter 'Row/Column', the index and the direction you want to move the cards\n";

        int16_t index;
        std::string orientation, move_direction;

        std::cin >> orientation >> index >> move_direction;

        auto orient = stringToOrientation(orientation);
        auto move_dir = stringToMoveDir(move_direction);
        auto it = options.find(orient);

        if (it == options.end() || std::find(it->second.begin(), it->second.end(), index) == it->second.end()) {
            Logger::log(Level::WARNING, "Wrong choice");
            return { Orientation::Unknown,0,MoveDirection::Unknown };
        }

        auto dirIt = validDirections.find(orient);
        if (std::find(dirIt->second.begin(), dirIt->second.end(), move_dir) == dirIt->second.end()) {
            Logger::log(Level::WARNING, "Wrong choice");
            return { Orientation::Unknown,0,MoveDirection::Unknown };
        }
        Logger::log(Level::INFO, "Valid choice");
        return { orient,index,move_dir };
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
                (card.getType() == CombatCardType::TWO ||
                    card.getType() == CombatCardType::THREE ||
                    card.getType() == CombatCardType::FOUR)) {
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
        for (const auto& [coord, stack] : board) {
            if (!stack.empty()) {
                auto top_card = board.getTopCard(coord);
                const CombatCard& card = top_card->get();

                if (card.getType() == CombatCardType::ONE) {
                    Logger::log(Level::INFO, "Earthquake power removed a visible card with number 1.");
                    board.removeTopCardAt(coord);
                    board.availableSpaces();
                    break;
                }
                else {
                    Logger::log(Level::WARNING, "No visible cards with number 1");
                }
            }
        }
    }


    ////------------------------------------------ Crumble -------------------------------------------
    Crumble::Crumble() {
        m_ability = PowerCardType::Crumble;
    }

    void Crumble::apply(Board& board, Player& player) {
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
        case CombatCardType::TWO:
            newType = CombatCardType::ONE;
            break;
        case CombatCardType::THREE:
            newType = CombatCardType::TWO;
            break;
        case CombatCardType::FOUR:
            newType = CombatCardType::THREE;
            break;
        default:
            Logger::log(Level::WARNING, "Card cannot be incremented beyond FOUR");
            return;
        }
        CombatCard new_card(newType, card.getColor());
        board.popTopCardAt(new_coord);
        board.appendMove(new_coord, std::move(new_card));
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
            Logger::log(Level::WARNING, "You can't use this card rn");
            return;
        }
        auto choices = checkShifting(pairs, board);
        std::cout << "Ur options are: ";
        for (const auto& [type, pair] : choices) {
            std::cout << MoveDirToString(type) << " " << pair.first.first << " " << pair.first.second << " , "
                << pair.second.first << " " << pair.second.second << " | ";
        }

        const std::unordered_map<MoveDirection, Coord> directionDeltas = {
       {MoveDirection::Down, {0, 1}},
       {MoveDirection::Up, {0, -1}},
       {MoveDirection::Right, {2, 0}},
       {MoveDirection::Left, {-2, 0}}
        };

        std::cout << "Enter the direction you want to move the stacks (Left or Right for rows / Up or Down for columns) and the pair of coords\n";

        std::string direction;
        std::cin >> direction;
        std::pair<Coord, Coord>pair;

        std::cin >> pair.first.first >> pair.first.second >> pair.second.first >> pair.second.second;

        auto choice = std::pair{ stringToMoveDir(direction),pair };
        auto it = std::find(choices.begin(), choices.end(), choice);

        if (it != choices.end()) {
            const auto& [type, selectedPair] = *it;
            const auto& delta = directionDeltas.at(type);
            std::vector<std::pair<Coord, Coord>>stacks;
            Coord newCoord1, newCoord2;

            if (type == MoveDirection::Down || type == MoveDirection::Up) {
                newCoord1 = { selectedPair.second.first, selectedPair.second.second + delta.second };
                newCoord2 = { selectedPair.first.first, selectedPair.first.second + delta.second };

                stacks.push_back({ selectedPair.second, newCoord1 });
                stacks.push_back({ selectedPair.first, newCoord2 });

                if (board.isValidMoveStacks(stacks)) {
                    board.moveStacks(stacks);
                    Logger::log(Level::INFO, "Move applied");
                }
                else {
                    Logger::log(Level::WARNING, "Invalid choice!");
                }
            }
            else {
                newCoord1 = { selectedPair.first.first + delta.first, selectedPair.first.second };
                newCoord2 = { selectedPair.second.first + delta.first, selectedPair.second.second };

                stacks.push_back({ selectedPair.first, newCoord1 });
                stacks.push_back({ selectedPair.second, newCoord2 });

                if (board.isValidMoveStacks(stacks)) {
                    board.moveStacks(stacks);
                    Logger::log(Level::INFO, "Move applied");
                }
                else {
                    Logger::log(Level::WARNING, "Invalid choice!\n");
                }
            }
        }
        else {
            Logger::log(Level::WARNING, "Invalid choice!\n");
        }
        Logger::log(Level::INFO, "Avalanche card used");
    }



    ////------------------------------------------ Rock -------------------------------------------
    Rock::Rock() {
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
