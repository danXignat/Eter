export module Game;

import <iostream>;
import <memory>;
namespace game {
	export class Game {
	public:
		Game(const Game&) = delete;					//delete copy constructor
		Game& operator=(const Game&) = delete;		//delete assignment operator
		static Game& getInstance();					//method to access game instance by refference

		void run();

	private:
		Game() = default;							//private constructor -> no outside access to it

	private:
		static std::unique_ptr<Game> m_instance;	//pointer to instance
	};
}
