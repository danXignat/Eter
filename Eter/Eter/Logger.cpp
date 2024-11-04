import Logger;

import <iostream>;
import <cstdint>;
import <string_view>;
import <format>;
import <fstream>;

namespace logger {

	std::string_view getLevelToString(Level level) {
		switch (level)
		{
			using enum Level;

		case Level::INFO:
			return "INFO";
			break;
		case Level::WARNING:
			return "WARNING";
			break;
		case Level::ERROR:
			return "ERROR";
			break;
		default:
			return "";
			break;
		}
	}

	Level Logger::m_min_priority{ Level::INFO };

	std::ofstream Logger::m_log_file{ "log.txt", std::ios::out | std::ios::trunc };

	void Logger::setMinPriority(Level level) {
		m_min_priority = level;
	}
}