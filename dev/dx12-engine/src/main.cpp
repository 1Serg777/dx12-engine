#include "Core/Logger.h"

#include <cstdlib>

int main()
{
	dxe::Logger::Initialize();

	// dxe::Logger::PrintTestLog();
	dxe::Logger::Info("Hello DX12 Engine!");

	dxe::Logger::Terminate();
	return EXIT_SUCCESS;
}