import Logger;

import <iostream>;
import <cstdint>;
import <string_view>;
import <format>;

using namespace logger;

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

Logger::Logger() {

}

template<typename... Args>
void Logger::log(Level level, std::string_view message, Args&&... args) {
	std::string formatted_message = std::vformat(message, std::make_format_args(args...));

	std::cout << std::format("[{}] {}\n", "INFO", formatted_message);
}