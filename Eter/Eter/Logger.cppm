export module Logger;

import <iostream>;
import <cstdint>;
import <string_view>;
import <format>;

namespace logger {
	export enum class Level : uint8_t {
		INFO,
		WARNING, 
		ERROR
	};

	export std::string_view getLevelToString(Level level);

	export class Logger {
	public:
		Logger();

		template<typename... Args>
		void log(Level level, std::string_view message, Args&&... args);

	private:

	};

	template<typename... Args>
	void Logger::log(Level level, std::string_view message, Args&&... args) {
		std::string formatted_message = std::vformat(message, std::make_format_args(args...));

		std::cout << std::format("[{}] {}\n", "INFO", formatted_message);
	}
}