#include "GpuApi/Dx12/Dx12ResourceManager.h"

#include "Core/Error.h"
#include "Core/Logger.h"
#include "Core/Utility.h"

namespace dxe
{
	void Dx12ResourceManager::Initialize()
	{
	}
	void Dx12ResourceManager::Terminate()
	{
		meshes.Clear();
		rootSignatures.Clear();
		graphicsPSOs.Clear();
	}
}