export module Game;

import <iostream>;
import <memory>;
import <format>;

export class Game {
public:
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	static Game& getInstance();

private:
	Game() = default;

private:
	static std::unique_ptr<Game> m_instance;
};