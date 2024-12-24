#include "GpuApi/Dx12/Dx12App.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

using namespace dxe;

int main()
{
	Dx12App app{};

	try
	{
		app.Initialize();
		app.Run();
	}
	catch (std::exception& re)
	{
		std::cerr << re.what() << std::endl;
		app.Terminate();
		return EXIT_FAILURE;
	}

	app.Terminate();
	return EXIT_SUCCESS;
}