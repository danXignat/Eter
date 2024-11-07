module TrainingMode;
import <iostream>;
import CombatCard;
import CombatCardType;
import Board;
namespace modes {
	

	TrainingMode::TrainingMode(std::string player1_name, std::string player2_name) {
		
		m_player1.setName(player1_name);
		m_player2.setName(player2_name);

	}
	void TrainingMode::gameLoop(bool iSaySo){
		base::Board board(3);
		while (iSaySo) {
			int posX, posY;
			std::cout << m_player1.getName();
			std::cin >> posX >> posY >> iSaySo;
			board.appendMove(
				{ posX, posY }, std::make_unique<base::CombatCard>(base::CombatCardType::FOUR)
			);
			board.renderBoard();
		}
	}
	
	/*bool TrainingMode::notWon(bool meow){
		return 0;
	}*/

	//-----------------------------------------------------------Win Manager---------------------------------------------------------
	/*
	
	struct Fields {
		uint16_t _size = 3;									//marimea boardului
		std::vector<uint16_t> rows, columns;				//numarul de X sau de O de pe cele size linii sau size colaone
		uint16_t first_diagonal = 0, second_diagonal = 0;	//numarul de X sau de O de pe cele diag. si diag. sec.

		Fields() = default;

		Fields(uint16_t size) :
			_size{size},
			rows( _size, 0 ), columns( _size, 0 ),
			first_diagonal{ 0 }, second_diagonal{ 0 }
		{}

		void add(uint16_t x, uint16_t y) { // cand adaug o carte se actualizeaza nuamrul de carti de pe fiecare linie
			rows[x]++;
			columns[y]++;
			if (x == y) {
				first_diagonal++;
			}
			if (x + y == rows.size() - 1) {
				second_diagonal++;
			}
		}

		bool win(uint16_t x, uint16_t y) const { // daca numarul de carti de pe linie este cat boardu inseamna ca gg
			return (rows[x] == _size || columns[y] == _size || first_diagonal == _size || second_diagonal == _size);
		}
	}p0, p1;

	*/
	TrainingMode::WinManager::WinManager() {

	}
}