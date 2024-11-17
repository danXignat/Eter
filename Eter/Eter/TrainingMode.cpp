module TrainingMode;

import <iostream>;
import <memory>;
import <unordered_map>;
import <format>;
import <string>;

import CombatCard;
import CombatCardType;
import Board;
import GameModeTypes;
import utils;
import Teams;
import Logger;
import Player;
import Inventory;

using namespace logger;

namespace base {
	//---------------------------------------Constructor-------------------------------------

	TrainingMode::TrainingMode(bool illusion, bool explosion, const std::string& player1_name, const std::string& player2_name) :
		m_board{ 3 } {

		if (illusion) {
			m_illusion_service = IllusionService();
		}

		if (explosion) {
			m_explosion_service = ExplosionService();
		}
	}
	//	m_player1{ player1_name, GameModeTypes::Training, teams::Team::RED },
	//	m_player2{ player2_name, GameModeTypes::Training, teams::Team::BLUE },
	//	m_win_manager{ 3 } 

	//---------------------------------------Events----------------------------------------------

	//void TrainingMode::gameLoop() {
	//	bool win = false;
	//	m_board.renderBoard();

	//	while (win == false) {
	//		Input input; //asta da handle la inputu de la tastatura

	//		CombatCardType type = _fromCharToType(input.card_type); 
	//		Player& current_player = _curentPlayer();

	//		auto card = current_player.getCard(type, input.illusion); //optional cu cartea se extrage cartea din invetaru playerului

	//		if (card) {
	//			m_board.appendMove(
	//				{ input.x, input.y }, std::move(*card)
	//			);

	//			win = m_win_manager.won(
	//				{ input.x, input.y }, current_player, m_board
	//			);

	//			_switchPlayer();
	//		}
	//		else {
	//			Logger::log(Level::WARNING, "no cards left of type '{}'", input.card_type);
	//		}

	//		system("cls");
	//		m_board.renderBoard();
	//	}

	//	std::cout << std::format("Player {}",
	//		(_curentPlayer().getTeam() == teams::Team::BLUE) ? "BLUE" : "RED"
	//	);
	//	std::cin.get();
	//}

	////------------------------------------------------Methods-------------------------------------------------

	//CombatCardType TrainingMode::_fromCharToType(char ch) {
	//	using enum CombatCardType;

	//	std::unordered_map<char, CombatCardType> switch_map{
	//			{'1', ONE},
	//			{'2', TWO},
	//			{'3', THREE},
	//			{'4', FOUR}
	//	};

	//	return switch_map[ch];
	//}

	//void TrainingMode::_switchPlayer() {
	//	m_current_player = !m_current_player;
	//}

	//Player& TrainingMode::_curentPlayer() {
	//	return (m_current_player) ? m_player1 : m_player2;
	//}

	////------------------------------------------------Win Manager-----------------------------------------------

	//TrainingMode::WinManager::WinManager(uint16_t size) : size{ size } {
	//	
	//}

	//bool TrainingMode::WinManager::won(const Coord& coord, const Player& player, const Board& board) {
	//	const auto& [x, y] = coord;

	//	int16_t incrementor = (player.getTeam() == teams::Team::BLUE) ? 1 : -1;

	//	rows[y] += incrementor;
	//	cols[x] += incrementor;

	//	if (abs(rows[y]) == size || abs(cols[x]) == size) {
	//		return true;
	//	}

	//	if (board.isFixed()) {
	//		//board.diagsCount() -> pair <nr elemente diag1, nr element diag2>
	//	}

	//	return false;
	//}

	////-------------------------------------------Inner Classes---------------------------------------------

	//TrainingMode::Input::Input() {
	//	std::unordered_set<char> available_moves{ '1', '2', '3', '4' };

	//	std::string input;
	//	std::getline(std::cin, input);

	//	std::istringstream stream{ input };
	//	std::string token;
	//	char del{ ' ' };

	//	std::getline(stream, token, del);

	//	std::cout << token;
	//	if (utils::isNumber(token)) {
	//		x = std::stoi(token);
	//	}
	//	else {
	//		throw std::invalid_argument("number is needed");
	//	}

	//	std::getline(stream, token, del);
	//	if (utils::isNumber(token)) {
	//		y = std::stoi(token);
	//	}
	//	else {
	//		throw std::invalid_argument("number is needed");
	//	}

	//	std::getline(stream, token, del);
	//	if (token.size() == 1 && available_moves.contains(token[0])) {
	//		card_type = token[0];
	//	}
	//	else {
	//		throw std::invalid_argument("not valid choice");
	//	}

	//	std::getline(stream, token, del);
	//	if (!token.empty() && token.size() == 1 && token[0] == 'i') {
	//		illusion = true;
	//	}
	//	else {
	//		illusion = false;
	//	}
	//}
}

