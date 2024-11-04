import Game;
import Logger;

import <iostream>;
import <memory>;

using namespace game;
using namespace logger;

std::unique_ptr<Game> Game::m_instance = nullptr;

Game& Game::getInstance() {
    if (m_instance == nullptr) {
        m_instance = std::unique_ptr<Game>(new Game());
        Logger::log(Level::INFO, "game instance created");
    }
    else {
        Logger::log(Level::INFO, "game instance already created accessed");
    }

    return *m_instance;
}

void Game::run() {
    std::cout << "running";
}