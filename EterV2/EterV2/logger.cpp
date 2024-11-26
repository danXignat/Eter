#include "Logger.h"

namespace logger {
	std::string_view getLevelToString(Level level) {
		switch (level)
		{
			using enum Level;
		case INFO:
			return "INFO";
		case WARNING:
			return "WARNING";
		case ERROR:
			return "ERROR";
		default:
			return "";
		}
	}

	Level Logger::m_min_priority{ Level::INFO };

	std::ofstream Logger::m_log_file{ "log.txt", std::ios::out | std::ios::trunc };

	void Logger::setMinPriority(Level level) {
		m_min_priority = level;
	}
}