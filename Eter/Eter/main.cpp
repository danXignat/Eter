import Game;
import Logger;

int main() {
	/*game::Game::getInstance().run();*/

	logger::Logger logger = logger::Logger();
	
	logger.log(logger::Level::INFO, "miau {} bau {}", "hau", "bau");
}