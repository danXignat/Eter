export module Board;

import <stack>;
import <unordered_map>;
import <unordered_set>;
import <memory>;


import CombatCard;

namespace base {
	class Board {
	private:
		std::unordered_map<
			std::pair<uint16_t, uint16_t>, std::stack<std::unique_ptr<CombatCard>>
		> m_cards;

		
	};
}