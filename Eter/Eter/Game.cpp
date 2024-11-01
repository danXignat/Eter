import Game;

import <iostream>;
import <memory>;
import <format>;

std::unique_ptr<Game> Game::m_instance = nullptr;

Game::Game() = default;

Game& Game::getInstance() {
    if (Game::m_instance == nullptr) {
        Game::m_instance = std::make_unique<Game>();
        std::cout << "[SUCCES] game created" << std::endl;
    }
    else {
        std::cout << "[INFO] game already created" << std::endl;
    }

    return *m_instance;
}