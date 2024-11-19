#pragma once
#include <cstdint>
#include <string_view>
#include <format>
#include <fstream>

namespace logger {
	enum class Level : uint8_t {
		INFO,
		WARNING,
		ERROR
	};

	std::string_view getLevelToString(Level level);

	class Logger {
	public:
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;

		static void setMinPriority(Level level);
		template<typename... Args>
		static void log(Level level, std::string_view message, Args&&... args);

	private:
		Logger() = default;

	private:
		static Level m_min_priority;
		static std::ofstream m_log_file;
	};

	template<typename... Args>
	void Logger::log(Level level, std::string_view message, Args&&... args) {
		std::string formatted_message = std::vformat(message, std::make_format_args(args...));

		if (level >= m_min_priority) {
			m_log_file << std::format("[{}] {}\n", getLevelToString(level), formatted_message);
			m_log_file.flush();
		}
	}
}