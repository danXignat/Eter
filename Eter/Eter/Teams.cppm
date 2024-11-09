export module Teams;

import <iostream>;

namespace teams {
	export enum class Team : uint8_t {
		RED,
		BLUE
	};

	export std::string to_string(Team team) {
		switch (team)
		{
		case teams::Team::RED:
			return "\033[31m";
			break;
		case teams::Team::BLUE:
			return "\033[34m";
			break;
		default:
			break;
		}
	}
}


