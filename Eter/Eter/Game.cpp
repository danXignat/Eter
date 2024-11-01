import Game;

import <iostream>;
import <memory>;
import <format>;

std::unique_ptr<Game> Game::m_instance = nullptr;

Game& Game::getInstance() {
    if (m_instance == nullptr) {
        m_instance = std::unique_ptr<Game>(new Game());
        std::cout << "[SUCCES] game created" << std::endl;
    }
    else {
        std::cout << "[INFO] game already created" << std::endl;
    }

    return *m_instance;
}