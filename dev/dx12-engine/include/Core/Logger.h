#pragma once

#include <string>
#include <string_view>

#define SPDLOG_WCHAR_TO_UTF8_SUPPORT
#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/color.h>

// #ifdef WINDOWS_PLATFORM
#undef CreateWindow
#undef min
#undef max
// #endif

namespace dxe
{
	class Logger
	{
	public:

		static void Initialize();
		static void Terminate();

		template <typename ...Args>
		static void Error(std::string_view format, Args&& ...args)
		{
			spdlog::error(format.data(), std::forward<Args>(args)...);
		}
		static void Error(std::string_view msg)
		{
			spdlog::error(msg.data());
		}
		static void Error(std::wstring_view msg)
		{
			spdlog::error(msg.data());
		}

		template <typename ...Args>
		static void Warn(std::string_view format, Args&& ...args)
		{
			spdlog::warn(format.data(), std::forward<Args>(args)...);
		}
		static void Warn(std::string_view msg)
		{
			spdlog::warn(msg.data());
		}
		static void Warn(std::wstring_view msg)
		{
			spdlog::warn(msg.data());
		}

		template <typename ...Args>
		static void Info(std::string_view format, Args&& ...args)
		{
			spdlog::info(format.data(), std::forward<Args>(args)...);
		}
		static void Info(std::string_view msg)
		{
			spdlog::info(msg.data());
		}
		static void Info(std::wstring_view msg)
		{
			spdlog::info(msg.data());
		}

	private:

		static Logger& GetInstance();
	};
}