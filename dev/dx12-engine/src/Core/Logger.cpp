#include "Core/Logger.h"

namespace dxe
{
	void Logger::Initialize()
	{
		Logger& logger = GetInstance();
	}

	void Logger::Terminate()
	{
		// TODO
	}

	Logger& Logger::GetInstance()
	{
		static Logger logger{};
		return logger;
	}
}