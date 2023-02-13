#include "Engine/Log.hpp"

#define LOG_LEVEL_OUTPUT 3

namespace Gaun
{
	void Log::PrintImpl(LogLevel level, const std::string& message)
	{
		switch (level)
		{
#if LOG_LEVEL_OUTPUT > 3
		case LogLevel::DEBUG:
			fmt::print("[DEBUG] " + message);
			break;
#endif

#if LOG_LEVEL_OUTPUT > 2
		case LogLevel::INFO:
			fmt::print("[INFO] " + message);
			break;
#endif

#if LOG_LEVEL_OUTPUT > 1
		case LogLevel::WARNING:
			fmt::print("[WARNING] " + message);
			break;
#endif

#if LOG_LEVEL_OUTPUT > 0
		case LogLevel::ERROR:
			fmt::print("[ERROR] " + message);
			break;
#endif
		}
	}
}