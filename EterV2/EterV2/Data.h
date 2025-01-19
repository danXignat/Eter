//#pragma once
//#pragma once
//#include <string>
//#include <filesystem>
//#include <vector>
//#include <nlohmann/json.hpp>
//#include "Board.h"
//#include "Player.h"
//#include "ElementalService.h"
//#include "MageService.h"
//#include "ExplosionService.h"
//#include "GameModeType.h"
//#include "ServiceType.h"
//#include "GameModeFactory.h"
//namespace base {
//    class Data {
//    public:
//        struct GameState {
//            std::string timestamp;
//            std::string version = "1.0";
//            GameModeType gameMode;
//            std::vector<ServiceType> services;
//
//            struct ElementalState {
//                PowerCardType card1_type;
//                PowerCardType card2_type;
//                bool card1_available;
//                bool card2_available;
//            } elementalState;
//
//            struct MageState {
//                MageTypeAbility mage1_type;
//                MageTypeAbility mage2_type;
//                bool mage1_available;
//                bool mage2_available;
//            } mageState;
//
//           
//            struct ExplosionState {
//                bool isActive;
//                std::vector<Coord> explosionCoords;
//            } explosionState;
//
//          
//
//            std::unordered_map<Coord, std::vector<CombatCard>, utils::CoordFunctor> combatCards;
//            std::unordered_set<Coord, utils::CoordFunctor> availableSpaces;
//
//            struct PlayerState {
//                std::string name;
//                color::ColorType color;
//                std::vector<CombatCard> cards;
//                std::vector<CombatCard> usedCards;
//            };
//            GameState::PlayerState player1;
//            GameState::PlayerState player2;
//            BoundingRectState boundingRect; 
//        };
//
//        explicit Data(const std::string& savesDirectory = "saves");
//
//       
//        bool quickSave(const Board& board, const Player& player1, const Player& player2,
//            BaseGameMode* gameMode = nullptr,
//            ElementalService* elementalService = nullptr,
//            MageService* mageService = nullptr,
//            ExplosionService* explosionService = nullptr);
//
//        bool saveToFile(const std::string& filename, const Board& board,
//            const Player& player1, const Player& player2,
//            BaseGameMode* gameMode = nullptr,
//            ElementalService* elementalService = nullptr,
//            MageService* mageService = nullptr,
//            ExplosionService* explosionService = nullptr);
//
//        bool loadFromFile(const std::string& filename, Board& board,
//            Player& player1, Player& player2,
//            IGameMode* gameMode = nullptr,
//            ElementalService* elementalService = nullptr,
//            MageService* mageService = nullptr,
//            ExplosionService* explosionService = nullptr);
//
//       
//        std::vector<std::string> listSaveFiles() const;
//        nlohmann::json getSaveMetadata(const std::string& filename) const;
//
//    private:
//        std::string m_savesDirectory;
//
//        void createSavesDirectory();
//        std::string generateTimestampedFilename();
//        GameState captureGameState(const Board& board, const Player& player1, const Player& player2,
//            BaseGameMode* gameMode, ElementalService* elementalService,
//            MageService* mageService, ExplosionService* explosionService);
//        nlohmann::json serializeGameState(const GameState& state);
//        GameState deserializeGameState(const nlohmann::json& gameState);
//        void restoreGameState(const GameState& state, Board& board,
//            Player& player1, Player& player2,
//            IGameMode* gameMode = nullptr,
//            ElementalService* elementalService = nullptr,
//            MageService* mageService = nullptr,
//            ExplosionService* explosionService = nullptr);
//    };
//}