#include "Core/Error.h"

#include <Windows.h>

#include <sstream>

namespace dxe
{
	// WinAPIError

	WinAPIError::WinAPIError()
		: Error("Unknown Win32 API error!")
	{
	}

	WinAPIError::WinAPIError(const char* message)
		: Error(message)
	{
	}

	WinAPIError::WinAPIError(const char* file, const char* function, int line, const char* message)
		: Error(CreateWinAPIError(file, function, line, message))
	{
	}

	std::string WinAPIError::CreateWinAPIError(
		const char* file, const char* function, uint32_t line, const char* message) const
	{
		std::stringstream errorStream{};

		errorStream << "********** Win32 API ERROR **********\n"
			<< "[FILE]: " << file << "\n"
			<< "[FUNCTION]: " << function << "\n"
			<< "[LINE]: " << line << "\n"
			<< "[USER MESSAGE]: " << message << "\n"
			<< "[SYSTEM MESSAGE]: " << RetrieveSystemErrorMessage();

		return errorStream.str();
	}

	std::string WinAPIError::RetrieveSystemErrorMessage() const
	{
		DWORD dwError = GetLastError();

		HLOCAL hLocalBuffer{};

		BOOL fOk = FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			NULL, dwError, 0, (LPSTR)&hLocalBuffer, 0, NULL);

		std::string errMsg;

		if (hLocalBuffer != NULL)
		{
			LPVOID msgPtr = LocalLock(hLocalBuffer);
			if (msgPtr != NULL)
			{
				errMsg = (LPSTR)msgPtr;
			}
			LocalFree(hLocalBuffer);
		}

		return errMsg;
	}
	
	// DX12Error
	
	DX12Error::DX12Error()
		: Error("Unknown DX12 error!")
	{
	}
	DX12Error::DX12Error(const char* message)
		: Error(message)
	{
	}
	DX12Error::DX12Error(const char* file, const char* function, int line, const char* message)
		: Error(CreateDX12Error(file, function, line, message))
	{
	}

	std::string DX12Error::CreateDX12Error(const char* file, const char* function, uint32_t line, const char* message) const
	{
		std::stringstream errorStream{};

		errorStream << "********** DX12 API ERROR **********\n"
			<< "[FILE]: " << file << "\n"
			<< "[FUNCTION]: " << function << "\n"
			<< "[LINE]: " << line << "\n"
			<< "[USER MESSAGE]: " << message << "\n";

		return errorStream.str();
	}
}