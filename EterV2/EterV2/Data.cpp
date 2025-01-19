//#include "Data.h"
//#include <fstream>
//#include <chrono>
//#include <iomanip>
//#include <sstream>
//
//using json = nlohmann::json;
//namespace fs = std::filesystem;
//
//namespace base {
//    Data::Data(const std::string& savesDirectory)
//        : m_savesDirectory(savesDirectory) {
//        createSavesDirectory();
//    }
//
//    bool Data::quickSave(const Board& board, const Player& player1, const Player& player2,
//        BaseGameMode* gameMode, ElementalService* elementalService,
//        MageService* mageService, ExplosionService* explosionService) {
//        std::string filename = generateTimestampedFilename();
//        return saveToFile(filename, board, player1, player2,
//            gameMode, elementalService, mageService, explosionService);
//    }
//
//    bool Data::saveToFile(const std::string& filename, const Board& board,
//        const Player& player1, const Player& player2,
//        BaseGameMode* gameMode, 
//        ElementalService* elementalService,
//        MageService* mageService,
//        ExplosionService* explosionService) {
//        try {
//            GameState state = captureGameState(board, player1, player2,
//                gameMode, elementalService,
//                mageService, explosionService);
//            json gameState = serializeGameState(state);
//
//            std::string fullPath = (fs::path(m_savesDirectory) / filename).string();
//            std::ofstream file(fullPath);
//            if (!file.is_open()) {
//                return false;
//            }
//            file << gameState.dump(4);
//            return true;
//        }
//        catch (const std::exception& e) {
//            return false;
//        }
//    }
//
//    bool Data::loadFromFile(const std::string& filename, Board& board,
//        Player& player1, Player& player2,
//        IGameMode* gameMode, ElementalService* elementalService,
//        MageService* mageService, ExplosionService* explosionService) {
//        try {
//            std::string fullPath = (fs::path(m_savesDirectory) / filename).string();
//            std::ifstream file(fullPath);
//            if (!file.is_open()) {
//                return false;
//            }
//
//            json gameState;
//            file >> gameState;
//
//            GameState state = deserializeGameState(gameState);
//            restoreGameState(state, board, player1, player2,
//                gameMode, elementalService, mageService, explosionService);
//
//            return true;
//        }
//        catch (const std::exception& e) {
//            return false;
//        }
//    }
//
//    std::vector<std::string> Data::listSaveFiles() const {
//        std::vector<std::string> files;
//        for (const auto& entry : fs::directory_iterator(m_savesDirectory)) {
//            if (entry.path().extension() == ".json") {
//                files.push_back(entry.path().filename().string());
//            }
//        }
//        return files;
//    }
//
//    json Data::getSaveMetadata(const std::string& filename) const {
//        std::string fullPath = (fs::path(m_savesDirectory) / filename).string();
//        std::ifstream file(fullPath);
//        if (!file.is_open()) {
//            return json();
//        }
//
//        json gameState;
//        file >> gameState;
//
//        return {
//            {"timestamp", gameState["timestamp"]},
//            {"version", gameState["version"]},
//            {"player1", gameState["player1"]["name"]},
//            {"player2", gameState["player2"]["name"]},
//            {"gameMode", gameState["gameMode"]}
//        };
//    }
//
//    void Data::createSavesDirectory() {
//        if (!fs::exists(m_savesDirectory)) {
//            fs::create_directory(m_savesDirectory);
//        }
//    }
//
//    std::string Data::generateTimestampedFilename() {
//        auto now = std::chrono::system_clock::now();
//        auto time = std::chrono::system_clock::to_time_t(now);
//        std::stringstream ss;
//        ss << "save_" << std::put_time(std::localtime(&time), "%Y%m%d_%H%M%S") << ".json";
//        return ss.str();
//    }
//
//    GameState Data::captureGameState(const Board& board, const Player& player1, const Player& player2,
//        BaseGameMode* gameMode, ElementalService* elementalService,
//        MageService* mageService, ExplosionService* explosionService) {
//        GameState state;
//
// 
//        auto now = std::chrono::system_clock::now();
//        auto time = std::chrono::system_clock::to_time_t(now);
//        std::stringstream ss;
//        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
//        state.timestamp = ss.str();
//
//  
//        if (gameMode) {
//            state.gameMode = static_cast<GameModeType>(gameMode->getGameModeType());
//            state.services = gameMode->getServiceConfig();
//        }
//
//        if (elementalService) {
//           
//            state.elementalState.card1_available = elementalService->isCard1Available();
//            state.elementalState.card2_available = elementalService->isCard2Available();
//            state.elementalState.card1_type = elementalService->getCard1Type();
//            state.elementalState.card2_type = elementalService->getCard2Type();
//        }
//
//      
//        if (mageService) {
//            state.mageState.mage1_available = mageService->isCard1Available();
//            state.mageState.mage2_available = mageService->isCard2Available();
//            state.mageState.mage1_type = mageService->getCard1Type().value_or(MageTypeAbility::None); 
//            state.mageState.mage2_type = mageService->getCard2Type().value_or(MageTypeAbility::None); 
//            
//        }
//        state.boundingRect = board.getBoundingRectStates();
//        state.combatCards = board.getCombatCards();
//        state.availableSpaces = board.availableSpaces();
//
//        state.player1 = GameState::PlayerState{
//            player1.getName(),
//            player1.getColor(),
//            player1.getCards()
//           
//        };
//
//        state.player2 = GameState::PlayerState{
//            player2.getName(),
//            player2.getColor(),
//            player2.getCards(),
//            player2.getUsedCards()
//        };
//
//        return state;
//    }
//
//    json Data::serializeGameState(const GameState& state) {
//        json gameState;
//
//        // Metadata
//        gameState["timestamp"] = state.timestamp;
//        gameState["version"] = state.version;
//        gameState["gameMode"] = static_cast<int>(state.gameMode);
//
//        // Services
//        json servicesJson = json::array();
//        for (const auto& service : state.services) {
//            servicesJson.push_back(static_cast<int>(service));
//        }
//        gameState["services"] = servicesJson;
//
//        // Elemental Mode state
//        gameState["elementalState"] = {
//            {"card1_type", static_cast<int>(state.elementalState.card1_type)},
//            {"card2_type", static_cast<int>(state.elementalState.card2_type)},
//            {"card1_available", state.elementalState.card1_available},
//            {"card2_available", state.elementalState.card2_available}
//        };
//
//        // Mage Mode state
//        gameState["mageState"] = {
//            {"mage1_type", static_cast<int>(state.mageState.mage1_type)},
//            {"mage2_type", static_cast<int>(state.mageState.mage2_type)},
//            {"mage1_available", state.mageState.mage1_available},
//            {"mage2_available", state.mageState.mage2_available}
//        };
//
//        // Explosion state
//        json explosionCoordsJson = json::array();
//        for (const auto& coord : state.explosionState.explosionCoords) {
//            explosionCoordsJson.push_back({
//                {"x", coord.first},
//                {"y", coord.second}
//                });
//        }
//        gameState["explosionState"] = {
//            {"isActive", state.explosionState.isActive},
//            {"explosionCoords", explosionCoordsJson}
//        };
//
//        // Board state
//        gameState["boundingRect"] = {
//            {"size", state.boundingRect.},
//            {"corner1", {{"x", state.boundingRect.corner1.first},
//                        {"y", state.boundingRect.corner1.second}}},
//            {"corner2", {{"x", state.boundingRect.corner2.first},
//                        {"y", state.boundingRect.corner2.second}}},
//            {"fixed_width", state.boundingRect.fixed_width},
//            {"fixed_height", state.boundingRect.fixed_height}
//        };
//
//        // Combat cards
//        json combatCardsJson;
//        for (const auto& [coord, stack] : state.combatCards) {
//            json stackJson = json::array();
//            for (const auto& card : stack) {
//                stackJson.push_back({
//                    {"type", static_cast<int>(card.getType())},
//                    {"color", static_cast<int>(card.getColor())},
//                    {"isIllusion", card.isIllusion()}
//                    });
//            }
//            combatCardsJson[std::to_string(coord.first) + "," +
//                std::to_string(coord.second)] = stackJson;
//        }
//        gameState["combatCards"] = combatCardsJson;
//
//        // Available spaces
//        json availableSpacesJson = json::array();
//        for (const auto& space : state.availableSpaces) {
//            availableSpacesJson.push_back({
//                {"x", space.first},
//                {"y", space.second}
//                });
//        }
//        gameState["availableSpaces"] = availableSpacesJson;
//
//        // Player states
//        auto serializePlayer = [](const GameState::PlayerState& player) {
//            json playerJson;
//            playerJson["name"] = player.name;
//            playerJson["color"] = static_cast<int>(player.color);
//
//            json cardsJson = json::array();
//            for (const auto& card : player.cards) {
//                cardsJson.push_back({
//                    {"type", static_cast<int>(card.getType())},
//                    {"color", static_cast<int>(card.getColor())},
//                    {"isIllusion", card.isIllusion()}
//                    });
//            }
//            playerJson["cards"] = cardsJson;
//
//            json usedCardsJson = json::array();
//            for (const auto& card : player.usedCards) {
//                usedCardsJson.push_back({
//                    {"type", static_cast<int>(card.getType())},
//                    {"color", static_cast<int>(card.getColor())},
//                    {"isIllusion", card.isIllusion()}
//                    });
//            }
//            playerJson["usedCards"] = usedCardsJson;
//
//            return playerJson;
//            };
//
//        gameState["player1"] = serializePlayer(state.player1);
//        gameState["player2"] = serializePlayer(state.player2);
//
//        return gameState;
//    }
//
//    GameState Data::deserializeGameState(const json& gameState) {
//        GameState state;
//
//        state.timestamp = gameState["timestamp"];
//        state.version = gameState["version"];
//        state.gameMode = static_cast<GameModeType>(gameState["gameMode"].get<int>());
//
//        for (const auto& service : gameState["services"]) {
//            state.services.push_back(static_cast<ServiceType>(service.get<int>()));
//        }
//
//   
//        auto& elementalJson = gameState["elementalState"];
//        state.elementalState = {
//            static_cast<PowerCardType>(elementalJson["card1_type"].get<int>()),
//            static_cast<PowerCardType>(elementalJson["card2_type"].get<int>()),
//            elementalJson["card1_available"].get<bool>(),
//            elementalJson["card2_available"].get<bool>()
//        };
//
//        auto& mageJson = gameState["mageState"];
//        state.mageState = {
//            static_cast<MageTypeAbility>(mageJson["mage1_type"].get<int>()),
//            static_cast<MageTypeAbility>(mageJson["mage2_type"].get<int>()),
//            mageJson["mage1_available"].get<bool>(),
//            mageJson["mage2_available"].get<bool>()
//        };
//
//        auto& explosionJson = gameState["explosionState"];
//        state.explosionState.isActive = explosionJson["isActive"].get<bool>();
//        for (const auto& coordJson : explosionJson["explosionCoords"]) {
//            state.explosionState.explosionCoords.push_back({
//                coordJson["x"].get<int>(),
//                coordJson["y"].get<int>()
//                });
//        }
//
//        auto& boundingRectJson = gameState["boundingRect"];
//        state.boundingRect = {
//            boundingRectJson["size"].get<uint16_t>(),
//            {boundingRectJson["corner1"]["x"].get<int>(),
//             boundingRectJson["corner1"]["y"].get<int>()},
//            {boundingRectJson["corner2"]["x"].get<int>(),
//             boundingRectJson["corner2"]["y"].get<int>()},
//            boundingRectJson["fixed_width"].get<bool>(),
//            boundingRectJson["fixed_height"].get<bool>()
//        };
//
//        for (const auto& [coordStr, stackJson] : gameState["combatCards"].items()) {
//            size_t comma = coordStr.find(',');
//            int x = std::stoi(coordStr.substr(0, comma));
//            int y = std::stoi(coordStr.substr(comma + 1));
//            Coord coord{ x, y };
//
//            std::vector<CombatCard> stack;
//            for (const auto& cardJson : stackJson) {
//                CombatCard card(
//                    static_cast<CombatCardType>(cardJson["type"].get<int>()),
//                    static_cast<color::ColorType>(cardJson["color"].get<int>())
//                );
//                if (cardJson["isIllusion"].get<bool>()) {
//                    card.flip();
//                }
//                stack.push_back(std::move(card));
//            }
//            state.combatCards[coord] = std::move(stack);
//        }
//
//        for (const auto& spaceJson : gameState["availableSpaces"]) {
//            state.availableSpaces.insert({
//                spaceJson["x"].get<int>(),
//                spaceJson["y"].get<int>()
//                });
//        }
//
//        auto deserializePlayer = [](const json& playerJson) -> GameState::PlayerState {
//            GameState::PlayerState player;
//            player.name = playerJson["name"].get<std::string>();
//            player.color = static_cast<color::ColorType>(playerJson["color"].get<int>());
//
//            for (const auto& cardJson : playerJson["cards"]) {
//                CombatCard card(
//                    static_cast<CombatCardType>(cardJson["type"].get<int>()),
//                    static_cast<color::ColorType>(cardJson["color"].get<int>())
//                );
//                if (cardJson["isIllusion"].get<bool>()) {
//                    card.flip();
//                }
//                player.cards.push_back(std::move(card));
//            }
//
//            for (const auto& cardJson : playerJson["usedCards"]) {
//                CombatCard card(
//                    static_cast<CombatCardType>(cardJson["type"].get<int>()),
//                    static_cast<color::ColorType>(cardJson["color"].get<int>())
//                );
//                if (cardJson["isIllusion"].get<bool>()) {
//                    card.flip();
//                }
//                player.usedCards.push_back(std::move(card));
//            }
//
//            return player;
//            };
//
//        state.player1 = deserializePlayer(gameState["player1"]);
//        state.player2 = deserializePlayer(gameState["player2"]);
//
//        return state;
//    }
//
//    void Data::restoreGameState(const GameState& state, Board& board,
//        Player& player1, Player& player2,
//        IGameMode* gameMode, ElementalService* elementalService,
//        MageService* mageService, ExplosionService* explosionService) {
//
//        board.clear();
//        board.setState(state.boundingRect.toBoundingRect(),
//            state.combatCards,
//            state.availableSpaces);
//
//
//        player1.clear();
//        player2.clear();
//
//        player1.setState(state.player1.cards, state.player1.usedCards);
//        player2.setState(state.player2.cards, state.player2.usedCards);
//        if (elementalService) {
//            elementalService->setState(
//                state.elementalState.card1_type,
//                state.elementalState.card2_type,
//                state.elementalState.card1_available,
//                state.elementalState.card2_available
//            );
//        }
//
//        if (mageService) {
//            mageService->setState(
//                state.mageState.mage1_type,
//                state.mageState.mage2_type,
//                state.mageState.mage1_available,
//                state.mageState.mage2_available
//            );
//        }
//
//        if (explosionService) {
//            explosionService->setState(
//                state.explosionState.isActive,
//                state.explosionState.explosionCoords
//            );
//        }
//    }
//
//} 