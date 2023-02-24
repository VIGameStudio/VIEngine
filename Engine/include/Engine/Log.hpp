#pragma once

#include <fmt/core.h>

namespace vi
{
	enum struct LogLevel { DEBUG, INFO, WARNING, ERROR };

	class Log
	{
	public:
		template <typename... Args>
		static void Debug(const std::string& message, Args&&... args)
		{
			Print(LogLevel::DEBUG, message, std::forward<Args>(args)...);
		}

		template <typename... Args>
		static void Info(const std::string& message, Args&&... args)
		{
			Print(LogLevel::INFO, message, std::forward<Args>(args)...);
		}

		template <typename... Args>
		static void Warning(const std::string& message, Args&&... args)
		{
			Print(LogLevel::WARNING, message, std::forward<Args>(args)...);
		}

		template <typename... Args>
		static void Error(const std::string& message, Args&&... args)
		{
			Print(LogLevel::ERROR, message, std::forward<Args>(args)...);
		}

		template <typename... Args>
		static void Print(LogLevel level, const std::string& message, Args&&... args)
		{
			PrintImpl(level, fmt::format(message + "\n", std::forward<Args>(args)...));
		}

	private:
		static void PrintImpl(LogLevel level, const std::string& message);
	};
}