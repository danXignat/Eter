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
            bool is_player_card = stack[stack.size() - 1].getColor() == player.getColor();
            if(is_player_card && !stack.back().isIllusion()){
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
            Logger::log(Level::INFO, "Card found at ({}, {})", coord.first,coord.second);
            Logger::log(Level::INFO, "Card:{}",combatCardToChar(card));
        }
        Logger::log(Level::INFO, "It's your turn");
        Coord coord_from;
        Coord new_coord;
        char card_type_char;

        std::cout << "Move from coordinates:" << "\n";
        std::cin >> coord_from.first >> coord_from.second;

        std::cout << "Covered card:"<<"\n";
        std::cin >> card_type_char;

        std::cout << "To coordinates:"<<"\n";
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
        if (valid_card&& valid_coord) {
            board.removeCardFromStackAt(coord_from, card);
            board.appendMove(new_coord, std::move(card));
        }
        if(!valid_card) {
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
            if (available_coord!=coord) {
                Logger::log(Level::WARNING, "No visible card at these coordinates");
            }
            else {
                player.addCard(std::move(card));
                board.removeTopCardAt(coord);
                Logger::log(Level::INFO, "Squall ability: Returned a visible card to the opponent's hand.");
            }
        }
    }

    std::vector<std::pair<Coord, CombatCard>> Squall::opponentCards( Board& board,const Player& player) const{
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
      
        for ( auto& [coord, stack] : board) {
            if (stack.size() > 1) {
                for (int i = 0;i < stack.size() - 1;i++) {
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

    void Hurricane::apply(Board& board, Player& player) {
    }


    ////------------------------------------------ Gust -------------------------------------------
    Gust::Gust() {
        m_ability = PowerCardType::Gust;
    }

    void Gust::apply(Board& board, Player& player) {
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
            if (card.isIllusion() &&card.getColor()==player.getColor()) {
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
      //  if (illusionService.hasPlayerIllusion(playerColor)) {
         //   Logger::log(Level::WARNING, "Player already has an illusion");
         //   return false;
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
    }

    ////------------------------------------------ Whirlpool -------------------------------------------
    Whirlpool::Whirlpool() {
        m_ability = PowerCardType::Whirlpool;
    }

    void Whirlpool::apply(Board& board, Player& player) {
    }

    ////------------------------------------------ Blizzard -------------------------------------------
    Blizzard::Blizzard() {
        m_ability = PowerCardType::Blizzard;
    }

    void Blizzard::apply(Board& board, Player& player) {
    }

    ////------------------------------------------ Waterfall -------------------------------------------
    Waterfall::Waterfall() {
        m_ability = PowerCardType::Waterfall;
    }

    void Waterfall::apply(Board& board, Player& player) {
    }


    ////------------------------------------------ Support -------------------------------------------
    Support::Support() {
        m_ability = PowerCardType::Support;
    }

    void Support::apply(Board& board, Player& player) {
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

    void Avalanche::apply(Board& board, Player& player) {
    }


    ////------------------------------------------ Rock -------------------------------------------
    Rock::Rock() {
        m_ability = PowerCardType::Rock;
    }

    void Rock::apply(Board& board, Player& player) {
        std::vector<Coord>illusionCoord = getIllusionCoords(board);
        if (!illusionCoord.empty()&& player.hasCards()) {

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
