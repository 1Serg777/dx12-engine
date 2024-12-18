#pragma once

#include "Core/Utility.h"

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>

namespace dxe
{
#define WINAPI_ZERO_VALUE_ERROR(x, message)\
	if (x == 0)\
		throw WinAPIError(THIS_FILE, THIS_FUNCTION, THIS_LINE, message)

#define WINAPI_NULL_VALUE_ERROR(x, message)\
	if (x == NULL)\
		throw WinAPIError(THIS_FILE, THIS_FUNCTION, THIS_LINE, message)

#define DX12_THROW_IF_NOT_SUCCESS(hr, message)\
	if (!SUCCEEDED(hr))\
		throw DX12Error(THIS_FILE, THIS_FUNCTION, THIS_LINE, message)

	class Error : public std::exception
	{
	public:

		Error(const char* message)
			: errorMessage(message) {}
		Error(const std::string& message)
			: errorMessage(message) {}

		NODISCARD char const* what() const override
		{
			return errorMessage.c_str();
		}

	private:

		std::string errorMessage{};
	};

	class WinAPIError : public Error
	{
	public:

		WinAPIError();
		WinAPIError(const char* message);
		WinAPIError(const char* file, const char* function, int line, const char* message);

		virtual ~WinAPIError() NOEXCEPT = default;

	private:

		std::string CreateWinAPIError(
			const char* file, const char* function, uint32_t line, const char* message) const;

		std::string RetrieveSystemErrorMessage() const;
	};

	class DX12Error : public Error
	{
	public:

		DX12Error();
		DX12Error(const char* message);
		DX12Error(const char* file, const char* function, int line, const char* message);

		virtual ~DX12Error() NOEXCEPT = default;

	private:

		std::string CreateDX12Error(
			const char* file, const char* function, uint32_t line, const char* message) const;
	};
}