#include "Renderer/Dx12/Dx12Vertex.h"

#include <unordered_map>

namespace dxe
{
	static std::unordered_map<VertexAttribType, std::string> semanticMap
	{
		{ { VertexAttribType::UNKNOWN  }, { "UNKNOWN" } },
		{ { VertexAttribType::POSITION }, { "POSITION" } },
		{ { VertexAttribType::NORMAL   }, { "NORMAL"   } },
		{ { VertexAttribType::TANGENT  }, { "TANGENT"  } },
		{ { VertexAttribType::COLOR    }, { "COLOR"    } },
		{ { VertexAttribType::UV       }, { "TEXCOORD" } },
	};

	DXGI_FORMAT PickDx12VertexAttribFormat(VertexAttribFormat format, uint32_t dimension)
	{
		switch (format)
		{
		case VertexAttribFormat::FLOAT32:
		{
			return PickFloat32Dx12VertexAttribFormat(dimension);
		}
		break;

		case VertexAttribFormat::UINT32:
		{
			return PickUint32Dx12VertexAttribFormat(dimension);
		}
		break;
		case VertexAttribFormat::UINT16:
		{
			return PickUint16Dx12VertexAttribFormat(dimension);
		}
		break;
		case VertexAttribFormat::UINT8:
		{
			return PickUint8Dx12VertexAttribFormat(dimension);
		}
		break;

		case VertexAttribFormat::INT32:
		{
			return PickInt32Dx12VertexAttribFormat(dimension);
		}
		break;
		case VertexAttribFormat::INT16:
		{
			return PickInt16Dx12VertexAttribFormat(dimension);
		}
		break;
		case VertexAttribFormat::INT8:
		{
			return PickInt8Dx12VertexAttribFormat(dimension);
		}
		break;

		default:
		{
			assert(true && "Unknown vertex attribute format provided!");
			return DXGI_FORMAT{};
		}
		break;
		}
	}

	DXGI_FORMAT PickFloat32Dx12VertexAttribFormat(uint32_t dimension)
	{
		switch (dimension)
		{
		case 1:
		{
			return DXGI_FORMAT_R32_FLOAT;
		}
		break;
		case 2:
		{
			return DXGI_FORMAT_R32G32_FLOAT;
		}
		break;
		case 3:
		{
			return DXGI_FORMAT_R32G32B32_FLOAT;
		}
		break;
		case 4:
		{
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		break;

		default:
		{
			assert(true && "Vertex attribute dimension is not supported!");
			return DXGI_FORMAT{};
		}
		break;
		}
	}

	DXGI_FORMAT PickUint32Dx12VertexAttribFormat(uint32_t dimension)
	{
		switch (dimension)
		{
		case 1:
		{
			return DXGI_FORMAT_R32_UINT;
		}
		break;
		case 2:
		{
			return DXGI_FORMAT_R32G32_UINT;
		}
		break;
		case 3:
		{
			return DXGI_FORMAT_R32G32B32_UINT;
		}
		break;
		case 4:
		{
			return DXGI_FORMAT_R32G32B32A32_UINT;
		}
		break;

		default:
		{
			assert(true && "Vertex attribute dimension is not supported!");
			return DXGI_FORMAT{};
		}
		break;
		}
	}
	DXGI_FORMAT PickUint16Dx12VertexAttribFormat(uint32_t dimension)
	{
		switch (dimension)
		{
		case 1:
		{
			return DXGI_FORMAT_R16_UINT;
		}
		break;
		case 2:
		{
			return DXGI_FORMAT_R16G16_UINT;
		}
		break;
		case 3:
		{
			assert(true && "Vertex attribute dimension is not supported! "
				"Use the respective 4D type and ignore the 4th component!");
			return DXGI_FORMAT_R16G16B16A16_UINT;
		}
		break;
		case 4:
		{
			return DXGI_FORMAT_R16G16B16A16_UINT;
		}
		break;

		default:
		{
			assert(true && "Vertex attribute dimension is not supported!");
			return DXGI_FORMAT{};
		}
		break;
		}
	}
	DXGI_FORMAT PickUint8Dx12VertexAttribFormat(uint32_t dimension)
	{
		switch (dimension)
		{
		case 1:
		{
			return DXGI_FORMAT_R8_UINT;
		}
		break;
		case 2:
		{
			return DXGI_FORMAT_R8G8_UINT;
		}
		break;
		case 3:
		{
			assert(true && "Vertex attribute dimension is not supported! "
				"Use the respective 4D type and ignore the 4th component!");
			return DXGI_FORMAT_R8G8B8A8_UINT;
		}
		break;
		case 4:
		{
			return DXGI_FORMAT_R8G8B8A8_UINT;
		}
		break;

		default:
		{
			assert(true && "Vertex attribute dimension is not supported!");
			return DXGI_FORMAT{};
		}
		break;
		}
	}

	DXGI_FORMAT PickInt32Dx12VertexAttribFormat(uint32_t dimension)
	{
		switch (dimension)
		{
		case 1:
		{
			return DXGI_FORMAT_R32_SINT;
		}
		break;
		case 2:
		{
			return DXGI_FORMAT_R32G32_SINT;
		}
		break;
		case 3:
		{
			return DXGI_FORMAT_R32G32B32_SINT;
		}
		break;
		case 4:
		{
			return DXGI_FORMAT_R32G32B32A32_SINT;
		}
		break;

		default:
		{
			assert(true && "Vertex attribute dimension is not supported!");
			return DXGI_FORMAT{};
		}
		break;
		}
	}
	DXGI_FORMAT PickInt16Dx12VertexAttribFormat(uint32_t dimension)
	{
		switch (dimension)
		{
		case 1:
		{
			return DXGI_FORMAT_R16_SINT;
		}
		break;
		case 2:
		{
			return DXGI_FORMAT_R16G16_SINT;
		}
		break;
		case 3:
		{
			assert(true && "Vertex attribute dimension is not supported! "
				"Use the respective 4D type and ignore the 4th component!");
			return DXGI_FORMAT_R16G16B16A16_SINT;
		}
		break;
		case 4:
		{
			return DXGI_FORMAT_R16G16B16A16_SINT;
		}
		break;

		default:
		{
			assert(true && "Vertex attribute dimension is not supported!");
			return DXGI_FORMAT{};
		}
		break;
		}
	}
	DXGI_FORMAT PickInt8Dx12VertexAttribFormat(uint32_t dimension)
	{
		switch (dimension)
		{
		case 1:
		{
			return DXGI_FORMAT_R8_SINT;
		}
		break;
		case 2:
		{
			return DXGI_FORMAT_R8G8_SINT;
		}
		break;
		case 3:
		{
			assert(true && "Vertex attribute dimension is not supported! "
				"Use the respective 4D type and ignore the 4th component!");
			return DXGI_FORMAT_R8G8B8A8_SINT;
		}
		break;
		case 4:
		{
			return DXGI_FORMAT_R16G16B16A16_SINT;
		}
		break;

		default:
		{
			assert(true && "Vertex attribute dimension is not supported!");
			return DXGI_FORMAT{};
		}
		break;
		}
	}

	const std::string& GetSemanticsName(VertexAttribType attribType)
	{
		auto searchResult = semanticMap.find(attribType);
		if (searchResult == semanticMap.end())
		{
			return semanticMap[VertexAttribType::UNKNOWN];
		}
		return searchResult->second;
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> VertexAttribLayoutToDx12AttribLayout(
		const std::vector<VertexAttribDescriptor>& attribLayout)
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> vulkanVertexLayout;
		vulkanVertexLayout.resize(attribLayout.size());

		for (uint32_t i = 0; i < attribLayout.size(); i++)
		{
			vulkanVertexLayout[i].SemanticName = GetSemanticsName(attribLayout[i].type).c_str();
			vulkanVertexLayout[i].SemanticIndex = 0;
			vulkanVertexLayout[i].Format = PickDx12VertexAttribFormat(attribLayout[i].format, attribLayout[i].dimension);
			vulkanVertexLayout[i].InputSlot = 0;
			vulkanVertexLayout[i].AlignedByteOffset = attribLayout[i].offset;
			vulkanVertexLayout[i].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			vulkanVertexLayout[i].InstanceDataStepRate = 0;
		}

		return vulkanVertexLayout;
	}

	std::vector<VertexAttribDescriptor> Dx12AttribLayoutToVertexAttribLayout(
		const std::vector<D3D12_INPUT_ELEMENT_DESC>& dx12AttribLayout)
	{
		std::vector<VertexAttribDescriptor> vertexLayout;

		// TODO

		return vertexLayout;
	}
}