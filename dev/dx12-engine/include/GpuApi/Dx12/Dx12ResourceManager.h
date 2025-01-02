#pragma once

#include "GpuApi/Dx12/Dx12Mesh.h"
#include "GpuApi/Dx12/Dx12RootSignature.h"
#include "GpuApi/Dx12/Dx12PSO.h"

#include "Core/Utility.h"

#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>

namespace dxe
{
	template <typename ResourceType, uint32_t ResourceContainerSize>
	class Dx12ResourceContainer
	{
	public:

		using IdType = uint32_t;

		Dx12ResourceContainer()
		{
			resources.resize(ResourceContainerSize);
		}

		void Clear()
		{
			resources.clear();
			resourceIds.Reset();
		}

		IdType AddResource(std::shared_ptr<ResourceType> resource)
		{
			uint32_t resourceId = resourceIds.GenerateUniqueId();

			if (resourceId == ResourceContainerSize)
			{
				// TODO: container resize policy?
				throw std::runtime_error{ "Not enough memory to store the Resource!" };
			}

			resources[resourceId] = resource;

			return resourceId;
		}
		void RemoveResource(IdType resourceId)
		{
			AssertIfInvalidId(resourceId);
			resources[resourceId].reset();
		}
		std::shared_ptr<ResourceType> GetResource(IdType resourceId)
		{
			AssertIfInvalidId(resourceId);
			return resources[resourceId];
		}

		uint32_t GetContainerSize() const
		{
			return ResourceContainerSize;
		}

	private:

		void AssertIfInvalidId(uint32_t id) const
		{
			assert(resourceIds.IdValid(id) && "Invalid Resource ID provided!");
		}

		SeqIdGenerator<uint32_t> resourceIds{};
		std::vector<std::shared_ptr<ResourceType>> resources;

		// TODO: add thread safety mechanisms such as std::mutex? 
	};

	class Dx12ResourceManager
	{
	public:

		void Initialize();
		void Terminate();

		Dx12ResourceContainer< Dx12Mesh,          64 > meshes;
		Dx12ResourceContainer< Dx12RootSignature, 16 > rootSignatures;
		Dx12ResourceContainer< Dx12GraphicsPSO,   16 > graphicsPSOs;
	};
}